/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Add to readline cmdline-editing by
 * (C) Copyright 2005
 * JinHua Luo, GuangDong Linux Center, <luo.jinhua@gd-linux.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/* #define	DEBUG	*/

#include <common.h>
#include <watchdog.h>
#include <command.h>
#include <version.h>
#ifdef CONFIG_MODEM_SUPPORT
#include <malloc.h>		/* for free() prototype */
#endif

#ifdef CONFIG_SYS_HUSH_PARSER
#include <hush.h>
#endif

#include <post.h>
#include <linux/ctype.h>
#include <turnkey/sysinfo.h>
#include <rtk/port.h>
#include <uboot/cmd/uboot_cmd.h>
#if defined (CONFIG_CMD_IMMORTAL) || defined (CONFIG_SENAO_CUSTOMER_UBOOT)
#include <hwp/hw_profiles/gpioboard.c>
#endif

#if defined(CONFIG_SILENT_CONSOLE) || defined(CONFIG_POST) || defined(CONFIG_CMDLINE_EDITING)
DECLARE_GLOBAL_DATA_PTR;
#endif

/*
 * Board-specific Platform code can reimplement show_boot_progress () if needed
 */
void inline __show_boot_progress (int val) {}
void show_boot_progress (int val) __attribute__((weak, alias("__show_boot_progress")));

#if defined(CONFIG_UPDATE_TFTP)
int update_tftp (ulong addr);
#endif /* CONFIG_UPDATE_TFTP */

#define MAX_DELAY_STOP_STR 32

#undef DEBUG_PARSER

char        console_buffer[CONFIG_SYS_CBSIZE + 1];	/* console I/O buffer	*/

static char * delete_char (char *buffer, char *p, int *colp, int *np, int plen);
static const char erase_seq[] = "\b \b";		/* erase sequence	*/
static const char   tab_seq[] = "        ";		/* used to expand TABs	*/

#ifdef CONFIG_BOOT_RETRY_TIME
static uint64_t endtime = 0;  /* must be set, default is instant timeout */
static int      retry_time = -1; /* -1 so can call readline before main_loop */
#endif

#define	endtick(seconds) (get_ticks() + (uint64_t)(seconds) * get_tbclk())

#ifndef CONFIG_BOOT_RETRY_MIN
#define CONFIG_BOOT_RETRY_MIN CONFIG_BOOT_RETRY_TIME
#endif

#ifdef CONFIG_MODEM_SUPPORT
int do_mdm_init = 0;
extern void mdm_init(void); /* defined in board.c */
#endif

#ifdef CONFIG_SENAO_BOOT_VERSION_UPDATE
void SnCheckAndUpdateBootVer(void);
#endif

void SnUpdateRescueFlag(void);

/***************************************************************************
 * Watch for 'delay' seconds for autoboot stop or autoboot delay string.
 * returns: 0 -  no key string, allow autoboot 1 - got key string, abort
 */
#if defined(CONFIG_BOOTDELAY)
# if defined(CONFIG_AUTOBOOT_KEYED)
#ifndef CONFIG_MENU
static inline
#endif
int abortboot(int bootdelay)
{
	int abort = 0;
	uint64_t etime = endtick(bootdelay);
	struct {
		char* str;
		u_int len;
		int retry;
	}
	delaykey [] = {
		{ str: getenv ("bootdelaykey"),  retry: 1 },
		{ str: getenv ("bootdelaykey2"), retry: 1 },
		{ str: getenv ("bootstopkey"),   retry: 0 },
		{ str: getenv ("bootstopkey2"),  retry: 0 },
	};

	char presskey [MAX_DELAY_STOP_STR];
	u_int presskey_len = 0;
	u_int presskey_max = 0;
	u_int i;

#  ifdef CONFIG_AUTOBOOT_PROMPT
	printf(CONFIG_AUTOBOOT_PROMPT);
#  endif

#  ifdef CONFIG_AUTOBOOT_DELAY_STR
	if (delaykey[0].str == NULL)
		delaykey[0].str = CONFIG_AUTOBOOT_DELAY_STR;
#  endif
#  ifdef CONFIG_AUTOBOOT_DELAY_STR2
	if (delaykey[1].str == NULL)
		delaykey[1].str = CONFIG_AUTOBOOT_DELAY_STR2;
#  endif
#  ifdef CONFIG_AUTOBOOT_STOP_STR
	if (delaykey[2].str == NULL)
		delaykey[2].str = CONFIG_AUTOBOOT_STOP_STR;
#  endif
#  ifdef CONFIG_AUTOBOOT_STOP_STR2
	if (delaykey[3].str == NULL)
		delaykey[3].str = CONFIG_AUTOBOOT_STOP_STR2;
#  endif

	for (i = 0; i < sizeof(delaykey) / sizeof(delaykey[0]); i ++) {
		delaykey[i].len = delaykey[i].str == NULL ?
				    0 : strlen (delaykey[i].str);
		delaykey[i].len = delaykey[i].len > MAX_DELAY_STOP_STR ?
				    MAX_DELAY_STOP_STR : delaykey[i].len;

		presskey_max = presskey_max > delaykey[i].len ?
				    presskey_max : delaykey[i].len;

#  if DEBUG_BOOTKEYS
		printf("%s key:<%s>\n",
		       delaykey[i].retry ? "delay" : "stop",
		       delaykey[i].str ? delaykey[i].str : "NULL");
#  endif
	}

	/* In order to keep up with incoming data, check timeout only
	 * when catch up.
	 */
	do {
		if (tstc()) {
			if (presskey_len < presskey_max) {
				presskey [presskey_len ++] = getc();
			}
			else {
				for (i = 0; i < presskey_max - 1; i ++)
					presskey [i] = presskey [i + 1];

				presskey [i] = getc();
			}
		}

		for (i = 0; i < sizeof(delaykey) / sizeof(delaykey[0]); i ++) {
			if (delaykey[i].len > 0 &&
			    presskey_len >= delaykey[i].len &&
			    memcmp (presskey + presskey_len - delaykey[i].len,
				    delaykey[i].str,
				    delaykey[i].len) == 0) {
#  if DEBUG_BOOTKEYS
				printf("got %skey\n",
				       delaykey[i].retry ? "delay" : "stop");
#  endif

#  ifdef CONFIG_BOOT_RETRY_TIME
				/* don't retry auto boot */
				if (! delaykey[i].retry)
					retry_time = -1;
#  endif
				abort = 1;
			}
		}
	} while (!abort && get_ticks() <= etime);

#  if DEBUG_BOOTKEYS
	if (!abort)
		puts("key timeout\n");
#  endif

#ifdef CONFIG_SILENT_CONSOLE
	if (abort)
		gd->flags &= ~GD_FLG_SILENT;
#endif

	return abort;
}

# else	/* !defined(CONFIG_AUTOBOOT_KEYED) */

#ifdef CONFIG_MENUKEY
static int menukey = 0;
#endif

#ifndef CONFIG_MENU
static inline
#endif

#define RTK_ABOUT_AUTOBOOT_CHAR     0x1B  /* key to abort the auto boot process, 0x20 - space, 0x1B - ESC */

int abortboot(int bootdelay)
{
	int abort = 0;
#ifndef CONFIG_SENAO_MENUKEY
	char tkey = 0;
#endif
#ifdef CONFIG_SENAO_MENUKEY
    int inputc=0;
#ifdef CONFIG_SENAO_CUSTOMER_UBOOT
    int enter=0;
#else
	int enterc=0;
    int entera=0;
    int enterp=0;
#endif

#ifdef CONFIG_SENAO_CUSTOMER_UBOOT
    printf("Enter Esc key to stop autoboot: %2d ", bootdelay);
#else
	printf("Enter correct key to stop autoboot: %2d ", bootdelay);
#endif

#else
#ifdef CONFIG_MENUPROMPT
	printf(CONFIG_MENUPROMPT);
#else
	printf("Hit Esc key to stop autoboot: %2d ", bootdelay);

#endif
#endif  /* CONFIG_SENAO_MENUKEY */

#ifndef CONFIG_SENAO_MENUKEY
#if defined CONFIG_ZERO_BOOTDELAY_CHECK
	/*
	 * Check if key already pressed
	 * Don't check if bootdelay < 0
	 */
	if (bootdelay >= 0) {
		if (tstc()) {	/* we got a key press	*/
			tkey = getc();
			if(tkey == RTK_ABOUT_AUTOBOOT_CHAR) /*Equal to space key*/
			{
			    puts ("\b\b\b 0");
			    abort = 1;	/* don't auto boot	*/
            }
		}
	}
	}
#endif
#endif

	while ((bootdelay > 0) && (!abort)) {
		int i;

		--bootdelay;
		/* delay 100 * 10ms */
		for (i=0; !abort && i<100; ++i) {
			if (tstc()) {	/* we got a key press	*/
#ifdef CONFIG_SENAO_MENUKEY
/*SENAO:only allow a, p, c and enter key*/
                inputc=0;
                inputc= getc();  /* consume input    */

#ifdef CONFIG_SENAO_CUSTOMER_UBOOT
                if(inputc==27) /*Esc*/
                {
                    enter=1;
                }
                else if(inputc==13) /*enter*/
                {
                    /*ignore enter key, do nothing when input enter key*/
                }
                else
                {
                    /*clear below variable when enter key is not a, p, c or enter*/
                    enter=0;
                }

                if(enter) /*enter key*/
                {
                    abort  = 1; /* don't auto boot  */
                    bootdelay = 0;	/* no more delay	*/
                    break;
                }
#else
                if(inputc==97) /*a*/
                {
                    entera=1;
                }
                else if (inputc==112) /*p*/
                {
                    enterp=1;
                }
                else if(inputc==99 ) /*c*/
                {
                    enterc=1;
                }
                else if(inputc==13) /*enter*/
                {
                    /*ignore enter key, do nothing when input enter key*/
                }
                else
                {
                    /*clear below variable when enter key is not a, p, c or enter*/
                    entera=0;
                    enterp=0;
                    enterc=0;
                }

                if(entera && enterp && enterc) /*enter key*/
                {
                    abort  = 1; /* don't auto boot  */
                    bootdelay = 0;	/* no more delay	*/
                    break;
                }
#endif
#else  /* CONFIG_SENAO_MENUKEY */
				//abort  = 1;	/* don't auto boot	*/
				//bootdelay = 0;	/* no more delay	*/
# ifdef CONFIG_MENUKEY
				menukey = getc();
# else
				tkey = getc();  /* consume input	*/
				/* Only ESC key will enter loader */
				if (tkey == RTK_ABOUT_AUTOBOOT_CHAR) /* ESC key */
# endif
				{
					abort  = 1;	/* don't auto boot	*/
					bootdelay = 0;	/* no more delay	*/
					break;
				}
#endif /* CONFIG_SENAO_MENUKEY */
			}
			udelay(10000);
		}

		printf("\b\b\b%2d ", bootdelay);
	}

	putc('\n');

#ifdef CONFIG_SILENT_CONSOLE
	if (abort)
		gd->flags &= ~GD_FLG_SILENT;
#endif

	return abort;
}
# endif	/* CONFIG_AUTOBOOT_KEYED */
#endif	/* CONFIG_BOOTDELAY >= 0  */

/*
 * Return 0 on success, or != 0 on error.
 */
#ifndef CONFIG_CMD_PXE
static inline
#endif
int run_command2(const char *cmd, int flag)
{
#ifndef CONFIG_SYS_HUSH_PARSER
	/*
	 * run_command can return 0 or 1 for success, so clean up its result.
	 */
	if (run_command(cmd, flag) == -1)
		return 1;

	return 0;
#else
	return parse_string_outer(cmd,
			FLAG_PARSE_SEMICOLON | FLAG_EXIT_FROM_LOOP);
#endif
}

/****************************************************************************/
static unsigned char prompt_str[30];
unsigned char *board_prompt(void);

int uboot_handle_command(char *, char *);
#ifdef CONFIG_CMD_IMMORTAL
void firmware_immortal(void);
#endif
#ifdef CONFIG_SENAO_CUSTOMER_UBOOT
void senao_uboot_cli(void);
#endif

unsigned char *board_prompt()
{
	return prompt_str;
}

void main_loop (void)
{
#ifndef CONFIG_SYS_HUSH_PARSER
	static char lastcommand[CONFIG_SYS_CBSIZE] = { 0, };
	int len;
	int rc = 1;
	int flag;
#ifdef CONFIG_CMD_IMMORTAL
	int ret = 0;
#endif
#endif

#if defined(CONFIG_BOOTDELAY)
	char *s;
	int bootdelay;
#endif
#ifdef CONFIG_PREBOOT
	char *p;
#endif
#ifdef CONFIG_BOOTCOUNT_LIMIT
	unsigned long bootcount = 0;
	unsigned long bootlimit = 0;
	char *bcs;
	char bcs_set[16];
#endif /* CONFIG_BOOTCOUNT_LIMIT */

#ifdef CONFIG_BOOTCOUNT_LIMIT
	bootcount = bootcount_load();
	bootcount++;
	bootcount_store (bootcount);
	sprintf (bcs_set, "%lu", bootcount);
	setenv ("bootcount", bcs_set);
	bcs = getenv ("bootlimit");
	bootlimit = bcs ? simple_strtoul (bcs, NULL, 10) : 0;
#endif /* CONFIG_BOOTCOUNT_LIMIT */

#ifdef CONFIG_MODEM_SUPPORT
	debug ("DEBUG: main_loop:   do_mdm_init=%d\n", do_mdm_init);
	if (do_mdm_init) {
		char *str = strdup(getenv("mdm_cmd"));
		setenv ("preboot", str);  /* set or delete definition */
		if (str != NULL)
			free (str);
		mdm_init(); /* wait for modem connection */
	}
#endif  /* CONFIG_MODEM_SUPPORT */

#ifdef CONFIG_VERSION_VARIABLE
	{
		setenv ("ver", version_string);  /* set version variable */
	}
#endif /* CONFIG_VERSION_VARIABLE */

#ifdef CONFIG_SYS_HUSH_PARSER
	u_boot_hush_start ();
#endif

#if defined(CONFIG_HUSH_INIT_VAR)
	hush_init_var ();
#endif
#ifdef CONFIG_SENAO_BOOT_VERSION_UPDATE
        SnCheckAndUpdateBootVer();
#endif
        /* Rescue flag*/
        SnUpdateRescueFlag();

#ifdef CONFIG_PREBOOT
	if ((p = getenv ("preboot")) != NULL) {
# ifdef CONFIG_AUTOBOOT_KEYED
		int prev = disable_ctrlc(1);	/* disable Control C checking */
# endif

		run_command2(p, 0);

# ifdef CONFIG_AUTOBOOT_KEYED
		disable_ctrlc(prev);	/* restore Control C checking */
# endif
	}
#endif /* CONFIG_PREBOOT */

#if defined(CONFIG_UPDATE_TFTP)
	update_tftp (0UL);
#endif /* CONFIG_UPDATE_TFTP */

#if defined(CONFIG_BOOTDELAY)
	s = getenv ("bootdelay");
	bootdelay = s ? (int)simple_strtol(s, NULL, 10) : CONFIG_BOOTDELAY;

	debug ("### main_loop entered: bootdelay=%d\n\n", bootdelay);

# ifdef CONFIG_BOOT_RETRY_TIME
	init_cmd_timeout ();
# endif	/* CONFIG_BOOT_RETRY_TIME */

#ifdef CONFIG_POST
	if (gd->flags & GD_FLG_POSTFAIL) {
		s = getenv("failbootcmd");
	}
	else
#endif /* CONFIG_POST */
#ifdef CONFIG_BOOTCOUNT_LIMIT
	if (bootlimit && (bootcount > bootlimit)) {
		printf ("Warning: Bootlimit (%u) exceeded. Using altbootcmd.\n",
		        (unsigned)bootlimit);
		s = getenv ("altbootcmd");
	}
	else
#endif /* CONFIG_BOOTCOUNT_LIMIT */
		s = getenv ("bootcmd");

	debug ("### main_loop: bootcmd=\"%s\"\n", s ? s : "<UNDEFINED>");

	if (bootdelay >= 0 && s && !abortboot (bootdelay)) {
# ifdef CONFIG_AUTOBOOT_KEYED
		int prev = disable_ctrlc(1);	/* disable Control C checking */
# endif

#ifdef CONFIG_CMD_IMMORTAL
		ret = run_command2(s, 0);
#else
		run_command2(s, 0);
#endif

# ifdef CONFIG_AUTOBOOT_KEYED
		disable_ctrlc(prev);	/* restore Control C checking */
# endif
	}

# ifdef CONFIG_MENUKEY
	if (menukey == CONFIG_MENUKEY) {
		s = getenv("menucmd");
		if (s)
			run_command2(s, 0);
	}
#endif /* CONFIG_MENUKEY */
#endif /* CONFIG_BOOTDELAY */

#ifdef CONFIG_CMD_IMMORTAL
	if(ret == 1)
		firmware_immortal();
#endif

#ifdef CONFIG_SENAO_CUSTOMER_UBOOT
	senao_uboot_cli();
#endif

#ifdef CONFIG_CUSTOMER_BOARD
	sprintf((char *)prompt_str,"%s# ", getenv("boardmodel"));
	rtk_network_on();
#else
	sprintf((char *)prompt_str,"%s# ",CONFIG_SYS_PROMPT);
#endif

  // Starry@20221212 dbgmsg
  { 
    if (0 > bootdelay)  {
      printf("%s() force rtk_network_on. \n", __func__);
      rtk_network_on();
    }
  }


	/*
	 * Main Loop for Monitor Command Processing
	 */
#ifdef CONFIG_SYS_HUSH_PARSER
	parse_file_outer();
	/* This point is never reached */
	for (;;);
#else
	for (;;) {
#ifdef CONFIG_BOOT_RETRY_TIME
		if (rc >= 0) {
			/* Saw enough of a valid command to
			 * restart the timeout.
			 */
			reset_cmd_timeout();
		}
#endif
		len = readline ((const char *)prompt_str);
		flag = 0;	/* assume no special flags for now */
		if (len > 0)
			strcpy (lastcommand, console_buffer);
		else if (len == 0)
			flag |= CMD_FLAG_REPEAT;
#ifdef CONFIG_BOOT_RETRY_TIME
		else if (len == -2) {
			/* -2 means timed out, retry autoboot
			 */
			puts ("\nTimed out waiting for command\n");
# ifdef CONFIG_RESET_TO_RETRY
			/* Reinit board to run initialization code again */
			do_reset (NULL, 0, 0, NULL);
# else
			return;		/* retry autoboot */
# endif
		}
#endif

		if (len == -1)
			puts ("<INTERRUPT>\n");
		else
			rc = run_command (lastcommand, flag);

		if (rc <= 0) {
			/* invalid command or not repeatable, forget it */
			lastcommand[0] = 0;
		}
	}
#endif /*CONFIG_SYS_HUSH_PARSER*/
}

#ifdef CONFIG_BOOT_RETRY_TIME
/***************************************************************************
 * initialize command line timeout
 */
void init_cmd_timeout(void)
{
	char *s = getenv ("bootretry");

	if (s != NULL)
		retry_time = (int)simple_strtol(s, NULL, 10);
	else
		retry_time =  CONFIG_BOOT_RETRY_TIME;

	if (retry_time >= 0 && retry_time < CONFIG_BOOT_RETRY_MIN)
		retry_time = CONFIG_BOOT_RETRY_MIN;
}

/***************************************************************************
 * reset command line timeout to retry_time seconds
 */
void reset_cmd_timeout(void)
{
	endtime = endtick(retry_time);
}
#endif

#ifdef CONFIG_CMDLINE_EDITING

/*
 * cmdline-editing related codes from vivi.
 * Author: Janghoon Lyu <nandy@mizi.com>
 */

#define putnstr(str,n)	do {			\
		printf ("%.*s", (int)n, str);	\
	} while (0)

#define CTL_CH(c)		((c) - 'a' + 1)
#define CTL_BACKSPACE		('\b')
#define DEL			((char)255)
#define DEL7			((char)127)
#define CREAD_HIST_CHAR		('!')

#define getcmd_putch(ch)	putc(ch)
#define getcmd_getch()		getc()
#define getcmd_cbeep()		getcmd_putch('\a')

#define HIST_MAX		20
#define HIST_SIZE		CONFIG_SYS_CBSIZE

static int hist_max = 0;
static int hist_add_idx = 0;
static int hist_cur = -1;
unsigned hist_num = 0;

char* hist_list[HIST_MAX];
char hist_lines[HIST_MAX][HIST_SIZE + 1];	 /* Save room for NULL */

#define add_idx_minus_one() ((hist_add_idx == 0) ? hist_max : hist_add_idx-1)

static void hist_init(void)
{
	int i;

	hist_max = 0;
	hist_add_idx = 0;
	hist_cur = -1;
	hist_num = 0;

	for (i = 0; i < HIST_MAX; i++) {
		hist_list[i] = hist_lines[i];
		hist_list[i][0] = '\0';
	}
}

static void cread_add_to_hist(char *line)
{
	strcpy(hist_list[hist_add_idx], line);

	if (++hist_add_idx >= HIST_MAX)
		hist_add_idx = 0;

	if (hist_add_idx > hist_max)
		hist_max = hist_add_idx;

	hist_num++;
}

static char* hist_prev(void)
{
	char *ret;
	int old_cur;

	if (hist_cur < 0)
		return NULL;

	old_cur = hist_cur;
	if (--hist_cur < 0)
		hist_cur = hist_max;

	if (hist_cur == hist_add_idx) {
		hist_cur = old_cur;
		ret = NULL;
	} else
		ret = hist_list[hist_cur];

	return (ret);
}

static char* hist_next(void)
{
	char *ret;

	if (hist_cur < 0)
		return NULL;

	if (hist_cur == hist_add_idx)
		return NULL;

	if (++hist_cur > hist_max)
		hist_cur = 0;

	if (hist_cur == hist_add_idx) {
		ret = "";
	} else
		ret = hist_list[hist_cur];

	return (ret);
}

#ifndef CONFIG_CMDLINE_EDITING
static void cread_print_hist_list(void)
{
	int i;
	unsigned long n;

	n = hist_num - hist_max;

	i = hist_add_idx + 1;
	while (1) {
		if (i > hist_max)
			i = 0;
		if (i == hist_add_idx)
			break;
		printf("%s\n", hist_list[i]);
		n++;
		i++;
	}
}
#endif /* CONFIG_CMDLINE_EDITING */

#define BEGINNING_OF_LINE() {			\
	while (num) {				\
		getcmd_putch(CTL_BACKSPACE);	\
		num--;				\
	}					\
}

#define ERASE_TO_EOL() {				\
	if (num < eol_num) {				\
		printf("%*s", (int)(eol_num - num), ""); \
		do {					\
			getcmd_putch(CTL_BACKSPACE);	\
		} while (--eol_num > num);		\
	}						\
}

#define REFRESH_TO_EOL() {			\
	if (num < eol_num) {			\
		wlen = eol_num - num;		\
		putnstr(buf + num, wlen);	\
		num = eol_num;			\
	}					\
}

static void cread_add_char(char ichar, int insert, unsigned long *num,
	       unsigned long *eol_num, char *buf, unsigned long len)
{
	unsigned long wlen;

	/* room ??? */
	if (insert || *num == *eol_num) {
		if (*eol_num > len - 1) {
			getcmd_cbeep();
			return;
		}
		(*eol_num)++;
	}

	if (insert) {
		wlen = *eol_num - *num;
		if (wlen > 1) {
			memmove(&buf[*num+1], &buf[*num], wlen-1);
		}

		buf[*num] = ichar;
		putnstr(buf + *num, wlen);
		(*num)++;
		while (--wlen) {
			getcmd_putch(CTL_BACKSPACE);
		}
	} else {
		/* echo the character */
		wlen = 1;
		buf[*num] = ichar;
		putnstr(buf + *num, wlen);
		(*num)++;
	}
}

static void cread_add_str(char *str, int strsize, int insert, unsigned long *num,
	      unsigned long *eol_num, char *buf, unsigned long len)
{
	while (strsize--) {
		cread_add_char(*str, insert, num, eol_num, buf, len);
		str++;
	}
}

static int cread_line(const char *const prompt, char *buf, unsigned int *len)
{
	unsigned long num = 0;
	unsigned long eol_num = 0;
	unsigned long wlen;
	char ichar;
	int insert = 1;
	int esc_len = 0;
	char esc_save[8];
	int init_len = strlen(buf);

	if (init_len)
		cread_add_str(buf, init_len, 1, &num, &eol_num, buf, *len);

	while (1) {
#ifdef CONFIG_BOOT_RETRY_TIME
		while (!tstc()) {	/* while no incoming data */
			if (retry_time >= 0 && get_ticks() > endtime)
				return (-2);	/* timed out */
			WATCHDOG_RESET();
		}
#endif

		ichar = getcmd_getch();

		if ((ichar == '\n') || (ichar == '\r')) {
			putc('\n');
			break;
		}

		/*
		 * handle standard linux xterm esc sequences for arrow key, etc.
		 */
		if (esc_len != 0) {
			if (esc_len == 1) {
				if (ichar == '[') {
					esc_save[esc_len] = ichar;
					esc_len = 2;
				} else {
					cread_add_str(esc_save, esc_len, insert,
						      &num, &eol_num, buf, *len);
					esc_len = 0;
				}
				continue;
			}

			switch (ichar) {

			case 'D':	/* <- key */
				ichar = CTL_CH('b');
				esc_len = 0;
				break;
			case 'C':	/* -> key */
				ichar = CTL_CH('f');
				esc_len = 0;
				break;	/* pass off to ^F handler */
			case 'H':	/* Home key */
				ichar = CTL_CH('a');
				esc_len = 0;
				break;	/* pass off to ^A handler */
			case 'A':	/* up arrow */
				ichar = CTL_CH('p');
				esc_len = 0;
				break;	/* pass off to ^P handler */
			case 'B':	/* down arrow */
				ichar = CTL_CH('n');
				esc_len = 0;
				break;	/* pass off to ^N handler */
			default:
				esc_save[esc_len++] = ichar;
				cread_add_str(esc_save, esc_len, insert,
					      &num, &eol_num, buf, *len);
				esc_len = 0;
				continue;
			}
		}

		switch (ichar) {
		case 0x1b:
			if (esc_len == 0) {
				esc_save[esc_len] = ichar;
				esc_len = 1;
			} else {
				puts("impossible condition #876\n");
				esc_len = 0;
			}
			break;

		case CTL_CH('a'):
			BEGINNING_OF_LINE();
			break;
		case CTL_CH('c'):	/* ^C - break */
			*buf = '\0';	/* discard input */
			return (-1);
		case CTL_CH('f'):
			if (num < eol_num) {
				getcmd_putch(buf[num]);
				num++;
			}
			break;
		case CTL_CH('b'):
			if (num) {
				getcmd_putch(CTL_BACKSPACE);
				num--;
			}
			break;
		case CTL_CH('d'):
			if (num < eol_num) {
				wlen = eol_num - num - 1;
				if (wlen) {
					memmove(&buf[num], &buf[num+1], wlen);
					putnstr(buf + num, wlen);
				}

				getcmd_putch(' ');
				do {
					getcmd_putch(CTL_BACKSPACE);
				} while (wlen--);
				eol_num--;
			}
			break;
		case CTL_CH('k'):
			ERASE_TO_EOL();
			break;
		case CTL_CH('e'):
			REFRESH_TO_EOL();
			break;
		case CTL_CH('o'):
			insert = !insert;
			break;
		case CTL_CH('x'):
		case CTL_CH('u'):
			BEGINNING_OF_LINE();
			ERASE_TO_EOL();
			break;
		case DEL:
		case DEL7:
		case 8:
			if (num) {
				wlen = eol_num - num;
				num--;
				memmove(&buf[num], &buf[num+1], wlen);
				getcmd_putch(CTL_BACKSPACE);
				putnstr(buf + num, wlen);
				getcmd_putch(' ');
				do {
					getcmd_putch(CTL_BACKSPACE);
				} while (wlen--);
				eol_num--;
			}
			break;
		case CTL_CH('p'):
		case CTL_CH('n'):
		{
			char * hline;

			esc_len = 0;

			if (ichar == CTL_CH('p'))
				hline = hist_prev();
			else
				hline = hist_next();

			if (!hline) {
				getcmd_cbeep();
				continue;
			}

			/* nuke the current line */
			/* first, go home */
			BEGINNING_OF_LINE();

			/* erase to end of line */
			ERASE_TO_EOL();

			/* copy new line into place and display */
			strcpy(buf, hline);
			eol_num = strlen(buf);
			REFRESH_TO_EOL();
			continue;
		}
#ifdef CONFIG_AUTO_COMPLETE
		case '\t': {
			int num2, col;

			/* do not autocomplete when in the middle */
			if (num < eol_num) {
				getcmd_cbeep();
				break;
			}

			buf[num] = '\0';
			col = strlen(prompt) + eol_num;
			num2 = num;
			if (cmd_auto_complete(prompt, buf, &num2, &col)) {
				col = num2 - num;
				num += col;
				eol_num += col;
			}
			break;
		}
#endif
		default:
			cread_add_char(ichar, insert, &num, &eol_num, buf, *len);
			break;
		}
	}
	*len = eol_num;
	buf[eol_num] = '\0';	/* lose the newline */

	if (buf[0] && buf[0] != CREAD_HIST_CHAR)
		cread_add_to_hist(buf);
	hist_cur = hist_add_idx;

	return 0;
}

#endif /* CONFIG_CMDLINE_EDITING */

/****************************************************************************/

/*
 * Prompt for input and read a line.
 * If  CONFIG_BOOT_RETRY_TIME is defined and retry_time >= 0,
 * time out when time goes past endtime (timebase time in ticks).
 * Return:	number of read characters
 *		-1 if break
 *		-2 if timed out
 */
int readline (const char *const prompt)
{
	/*
	 * If console_buffer isn't 0-length the user will be prompted to modify
	 * it instead of entering it from scratch as desired.
	 */
	console_buffer[0] = '\0';

	return readline_into_buffer(prompt, console_buffer);
}


int readline_into_buffer (const char *const prompt, char * buffer)
{
	char *p = buffer;
#ifdef CONFIG_CMDLINE_EDITING
	unsigned int len = CONFIG_SYS_CBSIZE;
	int rc;
	static int initted = 0;

	/*
	 * History uses a global array which is not
	 * writable until after relocation to RAM.
	 * Revert to non-history version if still
	 * running from flash.
	 */
	if (gd->flags & GD_FLG_RELOC) {
		if (!initted) {
			hist_init();
			initted = 1;
		}

		if (prompt)
			puts (prompt);

		rc = cread_line(prompt, p, &len);
		return rc < 0 ? rc : len;

	} else {
#endif	/* CONFIG_CMDLINE_EDITING */
	char * p_buf = p;
	int	n = 0;				/* buffer index		*/
	int	plen = 0;			/* prompt length	*/
	int	col;				/* output column cnt	*/
	char	c;

	/* print prompt */
	if (prompt) {
		plen = strlen (prompt);
		puts (prompt);
	}
	col = plen;

	for (;;) {
#ifdef CONFIG_BOOT_RETRY_TIME
		while (!tstc()) {	/* while no incoming data */
			if (retry_time >= 0 && get_ticks() > endtime)
				return (-2);	/* timed out */
			WATCHDOG_RESET();
		}
#endif
		WATCHDOG_RESET();		/* Trigger watchdog, if needed */

#ifdef CONFIG_SHOW_ACTIVITY
		while (!tstc()) {
			extern void show_activity(int arg);
			show_activity(0);
			WATCHDOG_RESET();
		}
#endif
		c = getc();

		/*
		 * Special character handling
		 */
		switch (c) {
		case '\r':				/* Enter		*/
		case '\n':
			*p = '\0';
			puts ("\r\n");
			return (p - p_buf);

		case '\0':				/* nul			*/
			continue;

		case 0x03:				/* ^C - break		*/
			p_buf[0] = '\0';	/* discard input */
			return (-1);

		case 0x15:				/* ^U - erase line	*/
			while (col > plen) {
				puts (erase_seq);
				--col;
			}
			p = p_buf;
			n = 0;
			continue;

		case 0x17:				/* ^W - erase word	*/
			p=delete_char(p_buf, p, &col, &n, plen);
			while ((n > 0) && (*p != ' ')) {
				p=delete_char(p_buf, p, &col, &n, plen);
			}
			continue;

		case 0x08:				/* ^H  - backspace	*/
		case 0x7F:				/* DEL - backspace	*/
			p=delete_char(p_buf, p, &col, &n, plen);
			continue;

		default:
			/*
			 * Must be a normal character then
			 */
			if (n < CONFIG_SYS_CBSIZE-2) {
				if (c == '\t') {	/* expand TABs		*/
#ifdef CONFIG_AUTO_COMPLETE
					/* if auto completion triggered just continue */
					*p = '\0';
					if (cmd_auto_complete(prompt, console_buffer, &n, &col)) {
						p = p_buf + n;	/* reset */
						continue;
					}
#endif
					puts (tab_seq+(col&07));
					col += 8 - (col&07);
				} else {
					++col;		/* echo input		*/
					putc (c);
				}
				*p++ = c;
				++n;
			} else {			/* Buffer full		*/
				putc ('\a');
			}
		}
	}
#ifdef CONFIG_CMDLINE_EDITING
	}
#endif
}

/****************************************************************************/

static char * delete_char (char *buffer, char *p, int *colp, int *np, int plen)
{
	char *s;

	if (*np == 0) {
		return (p);
	}

	if (*(--p) == '\t') {			/* will retype the whole line	*/
		while (*colp > plen) {
			puts (erase_seq);
			(*colp)--;
		}
		for (s=buffer; s<p; ++s) {
			if (*s == '\t') {
				puts (tab_seq+((*colp) & 07));
				*colp += 8 - ((*colp) & 07);
			} else {
				++(*colp);
				putc (*s);
			}
		}
	} else {
		puts (erase_seq);
		(*colp)--;
	}
	(*np)--;
	return (p);
}

/****************************************************************************/

int parse_line (char *line, char *argv[])
{
	int nargs = 0;

#ifdef DEBUG_PARSER
	printf ("parse_line: \"%s\"\n", line);
#endif
	while (nargs < CONFIG_SYS_MAXARGS) {

		/* skip any white space */
		while (isblank(*line))
			++line;

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
#ifdef DEBUG_PARSER
		printf ("parse_line: nargs=%d\n", nargs);
#endif
			return (nargs);
		}

		argv[nargs++] = line;	/* begin of argument string	*/

		/* find end of string */
		while (*line && !isblank(*line))
			++line;

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
#ifdef DEBUG_PARSER
		printf ("parse_line: nargs=%d\n", nargs);
#endif
			return (nargs);
		}

		*line++ = '\0';		/* terminate current arg	 */
	}

	printf ("** Too many args (max. %d) **\n", CONFIG_SYS_MAXARGS);

#ifdef DEBUG_PARSER
	printf ("parse_line: nargs=%d\n", nargs);
#endif
	return (nargs);
}

/****************************************************************************/

static void process_macros (const char *input, char *output)
{
	char c, prev;
	const char *varname_start = NULL;
	int inputcnt = strlen (input);
	int outputcnt = CONFIG_SYS_CBSIZE;
	int state = 0;		/* 0 = waiting for '$'  */

	/* 1 = waiting for '(' or '{' */
	/* 2 = waiting for ')' or '}' */
	/* 3 = waiting for '''  */
#ifdef DEBUG_PARSER
	char *output_start = output;

	printf ("[PROCESS_MACROS] INPUT len %d: \"%s\"\n", strlen (input),
		input);
#endif

	prev = '\0';		/* previous character   */

	while (inputcnt && outputcnt) {
		c = *input++;
		inputcnt--;

		if (state != 3) {
			/* remove one level of escape characters */
			if ((c == '\\') && (prev != '\\')) {
				if (inputcnt-- == 0)
					break;
				prev = c;
				c = *input++;
			}
		}

		switch (state) {
		case 0:	/* Waiting for (unescaped) $    */
			if ((c == '\'') && (prev != '\\')) {
				state = 3;
				break;
			}
			if ((c == '$') && (prev != '\\')) {
				state++;
			} else {
				*(output++) = c;
				outputcnt--;
			}
			break;
		case 1:	/* Waiting for (        */
			if (c == '(' || c == '{') {
				state++;
				varname_start = input;
			} else {
				state = 0;
				*(output++) = '$';
				outputcnt--;

				if (outputcnt) {
					*(output++) = c;
					outputcnt--;
				}
			}
			break;
		case 2:	/* Waiting for )        */
			if (c == ')' || c == '}') {
				int i;
				char envname[CONFIG_SYS_CBSIZE], *envval;
				int envcnt = input - varname_start - 1;	/* Varname # of chars */

				/* Get the varname */
				for (i = 0; i < envcnt; i++) {
					envname[i] = varname_start[i];
				}
				envname[i] = 0;

				/* Get its value */
				envval = getenv (envname);

				/* Copy into the line if it exists */
				if (envval != NULL)
					while ((*envval) && outputcnt) {
						*(output++) = *(envval++);
						outputcnt--;
					}
				/* Look for another '$' */
				state = 0;
			}
			break;
		case 3:	/* Waiting for '        */
			if ((c == '\'') && (prev != '\\')) {
				state = 0;
			} else {
				*(output++) = c;
				outputcnt--;
			}
			break;
		}
		prev = c;
	}

	if (outputcnt)
		*output = 0;
	else
		*(output - 1) = 0;

#ifdef DEBUG_PARSER
	printf ("[PROCESS_MACROS] OUTPUT len %d: \"%s\"\n",
		strlen (output_start), output_start);
#endif
}

/****************************************************************************
 * returns:
 *	1  - command executed, repeatable
 *	0  - command executed but not repeatable, interrupted commands are
 *	     always considered not repeatable
 *	-1 - not executed (unrecognized, bootd recursion or too many args)
 *           (If cmd is NULL or "" or longer than CONFIG_SYS_CBSIZE-1 it is
 *           considered unrecognized)
 *
 * WARNING:
 *
 * We must create a temporary copy of the command since the command we get
 * may be the result from getenv(), which returns a pointer directly to
 * the environment data, which may change magicly when the command we run
 * creates or modifies environment variables (like "bootp" does).
 */

int run_command (const char *cmd, int flag)
{
	cmd_tbl_t *cmdtp;
	char cmdbuf[CONFIG_SYS_CBSIZE];	/* working copy of cmd		*/
	char *token;			/* start of token in cmdbuf	*/
	char *sep;			/* end of token (separator) in cmdbuf */
	char finaltoken[CONFIG_SYS_CBSIZE];
	char *str = cmdbuf;
	char *argv[CONFIG_SYS_MAXARGS + 1];	/* NULL terminated	*/
	int argc, inquotes;
	int repeatable = 1;
	int rc = 0;

#ifdef DEBUG_PARSER
	printf ("[RUN_COMMAND] cmd[%p]=\"", cmd);
	puts (cmd ? cmd : "NULL");	/* use puts - string may be loooong */
	puts ("\"\n");
#endif

	clear_ctrlc();		/* forget any previous Control C */

	if (!cmd || !*cmd) {
		return -1;	/* empty command */
	}

	if (strlen(cmd) >= CONFIG_SYS_CBSIZE) {
		puts ("## Command too long!\n");
		return -1;
	}

	strcpy (cmdbuf, cmd);

	/* Process separators and check for invalid
	 * repeatable commands
	 */

#ifdef DEBUG_PARSER
	printf ("[PROCESS_SEPARATORS] %s\n", cmd);
#endif
	while (*str) {

		/*
		 * Find separator, or string end
		 * Allow simple escape of ';' by writing "\;"
		 */
		for (inquotes = 0, sep = str; *sep; sep++) {
			if ((*sep=='\'') &&
			    (*(sep-1) != '\\'))
				inquotes=!inquotes;

			if (!inquotes &&
			    (*sep == ';') &&	/* separator		*/
			    ( sep != str) &&	/* past string start	*/
			    (*(sep-1) != '\\'))	/* and NOT escaped	*/
				break;
		}

		/*
		 * Limit the token to data between separators
		 */
		token = str;
		if (*sep) {
			str = sep + 1;	/* start of command for next pass */
			*sep = '\0';
		}
		else
			str = sep;	/* no more commands for next pass */
#ifdef DEBUG_PARSER
		printf ("token: \"%s\"\n", token);
#endif

		/* find macros in this token and replace them */
		process_macros (token, finaltoken);

		/* Extract arguments */
		if ((argc = parse_line (finaltoken, argv)) == 0) {
			rc = -1;	/* no command at all */
			continue;
		}

		/* Look up command in command table */
		if ((cmdtp = find_cmd(argv[0])) == NULL) {
			printf ("Unknown command '%s' - try 'help'\n", argv[0]);
			rc = -1;	/* give up after bad command */
			continue;
		}

		/* found - check max args */
		if (argc > cmdtp->maxargs) {
			cmd_usage(cmdtp);
			rc = -1;
			continue;
		}

#if defined(CONFIG_CMD_BOOTD)
		/* avoid "bootd" recursion */
		if (cmdtp->cmd == do_bootd) {
#ifdef DEBUG_PARSER
			printf ("[%s]\n", finaltoken);
#endif
			if (flag & CMD_FLAG_BOOTD) {
				puts ("'bootd' recursion detected\n");
				rc = -1;
				continue;
			} else {
				flag |= CMD_FLAG_BOOTD;
			}
		}
#endif

		/* OK - call function to do the command */
		if ((cmdtp->cmd) (cmdtp, flag, argc, argv) != 0) {
			rc = -1;
		}

		repeatable &= cmdtp->repeatable;

		/* Did the user stop this? */
		if (had_ctrlc ())
			return -1;	/* if stopped then not repeatable */
	}

	return rc ? rc : repeatable;
}

/****************************************************************************/

#if defined(CONFIG_CMD_RUN)
int do_run (cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	int i;

	if (argc < 2)
		return cmd_usage(cmdtp);

	for (i=1; i<argc; ++i) {
		char *arg;

		if ((arg = getenv (argv[i])) == NULL) {
			printf ("## Error: \"%s\" not defined\n", argv[i]);
			return 1;
		}

		if (run_command2(arg, flag) != 0)
			return 1;
	}
	return 0;
}
#endif

#ifdef CONFIG_SENAO_BOOT_VERSION_UPDATE
void SnCheckAndUpdateBootVer(void)
{
    char           *pEnvVer = NULL;
    char            CodeVer[64];

    pEnvVer = getenv("boot_version");
    sprintf (CodeVer,"%s" , CONFIG_SENAO_BOOT_VERSION);
    if (pEnvVer)
    {
        if (strcmp(pEnvVer, CodeVer)!=0) {
            setenv("boot_version", CodeVer);
            saveenv();
        }
    }
    else
    {
            setenv("boot_version", CodeVer);
            saveenv();
    }
}
#endif

void SnUpdateRescueFlag(void)
{
    char           *pEnvVer = NULL;
#ifdef CONFIG_SENAO_RESCUE_IMAGE
    char            CodeVer[64];
#endif

    pEnvVer = getenv("rescue_flag");
#ifdef CONFIG_SENAO_RESCUE_IMAGE
    sprintf (CodeVer,"%s" , CONFIG_SENAO_RESCUE_IMAGE_FLAG);
#endif

    if (pEnvVer)
    {
#ifdef CONFIG_SENAO_RESCUE_IMAGE
        if (strcmp(pEnvVer, CodeVer)!=0) {
            setenv("rescue_flag", CodeVer);
            saveenv();
        }
#endif
    }
    else
    {
        setenv("rescue_flag", "rescue_none");
        saveenv();
    }
}

int uboot_handle_command(char *command, char *input)
{
	static char lastcommand[CONFIG_SYS_CBSIZE] = { 0, };
	int rc = 1;
	int flag = 0;
	strcpy (lastcommand, command);
	strcat (lastcommand, input);
	rc = run_command (lastcommand, flag);
	return rc;
}

#ifdef CONFIG_CMD_IMMORTAL
void firmware_immortal(void)
{
#ifndef CONFIG_SYS_HUSH_PARSER
	int flag = 0;
	int tmp_rc = 1;
	char *tmp_file;
	char *tmp_immortal;
	char *tmp_mode;
	char *tmp_fiber;
	char *tmp_restore;
	static char tmp_filesize[CONFIG_SYS_CBSIZE] = { 0, };
#endif

	tmp_rc = run_command ("rtk network on;", flag);
	GpioboardInfoInit();
	PortInfoInit();
	rtk_extGpio_pin_set(0, 10, 0);
	rtk_extGpio_pin_set(0, 13, 0);
	rtk_extGpio_pin_set(0, 22, 0);
	rtk_extGpio_pin_set(0, 26, 0);
	tmp_immortal = getenv ("immortal");
	if (strcmp(tmp_immortal,"1") == 0)
	{
		tmp_fiber = getenv ("fiber_type");
		if(strcmp(tmp_fiber,"4") == 0)
		{
			rtk_port_10gMedia_set(0, PortMapSdkPortGet(0), PORT_10GMEDIA_FIBER_10G);
			rtk_port_10gMedia_set(0, PortMapSdkPortGet(1), PORT_10GMEDIA_FIBER_10G);
			rtk_port_10gMedia_set(0, PortMapSdkPortGet(2), PORT_10GMEDIA_FIBER_10G);
			rtk_port_10gMedia_set(0, PortMapSdkPortGet(3), PORT_10GMEDIA_FIBER_10G);
		}
		else if(strcmp(tmp_fiber,"3") == 0)
		{
			rtk_port_10gMedia_set(0, PortMapSdkPortGet(0), PORT_10GMEDIA_FIBER_1G);
			rtk_port_10gMedia_set(0, PortMapSdkPortGet(1), PORT_10GMEDIA_FIBER_1G);
			rtk_port_10gMedia_set(0, PortMapSdkPortGet(2), PORT_10GMEDIA_FIBER_1G);
			rtk_port_10gMedia_set(0, PortMapSdkPortGet(3), PORT_10GMEDIA_FIBER_1G);
		}
		tmp_mode = getenv ("mode");
		if (strcmp(tmp_mode,"STATIC") == 0)
		{
			tmp_file = getenv ("file_path");
			tmp_rc = uboot_handle_command("tftpboot 0x81000000 ", tmp_file);
			sprintf (tmp_filesize, "%lx", NetBootFileXferSize);
			if(tmp_rc != -1)
				tmp_rc = run_command ("spi_nand erase 0x4000000 0x1e00000", flag);
			if(tmp_rc != -1)
				tmp_rc = uboot_handle_command("spi_nand write 0x81000000 0x4000000 0x", tmp_filesize);
			if(tmp_rc != -1)
				tmp_rc = run_command ("spi_nand erase 0x5e00000 0x1e00000", flag);
			if(tmp_rc != -1)
				tmp_rc = uboot_handle_command("spi_nand write 0x81000000 0x5e00000 0x", tmp_filesize);
			if(tmp_rc != -1)
			{
				tmp_restore = getenv ("restore_defaults");
				if(strcmp(tmp_restore,"1") == 0)
				{
					tmp_rc = uboot_handle_command("setenv reset_flag ", "1");
					tmp_rc = run_command ("saveenv", flag);
					tmp_rc = run_command ("reset", flag);
				}
				else
					tmp_rc = run_command ("reset", flag);
			}
		}
		else if (strcmp(tmp_mode,"DHCP") == 0)
		{
			tmp_rc = run_command ("dhcp 0x81000000", flag);
			sprintf (tmp_filesize, "%lx", NetBootFileXferSize);
			if(tmp_rc != -1)
				tmp_rc = run_command ("spi_nand erase 0x4000000 0x1e00000", flag);
			if(tmp_rc != -1)
				tmp_rc = uboot_handle_command("spi_nand write 0x81000000 0x4000000 0x", tmp_filesize);
			if(tmp_rc != -1)
				tmp_rc = run_command ("spi_nand erase 0x5e00000 0x1e00000", flag);
			if(tmp_rc != -1)
				tmp_rc = uboot_handle_command("spi_nand write 0x81000000 0x5e00000 0x", tmp_filesize);
			if(tmp_rc != -1)
			{
				tmp_restore = getenv ("restore_defaults");
				if(strcmp(tmp_restore,"1") == 0)
				{
					tmp_rc = uboot_handle_command("setenv reset_flag ", "1");
					tmp_rc = run_command ("saveenv", flag);
					tmp_rc = run_command ("reset", flag);
				}
				else
					tmp_rc = run_command ("reset", flag);
			}
		}
	}
	else
		printf("!!!firmware immortal is disable.\n");
}
#endif

#ifdef CONFIG_SENAO_CUSTOMER_UBOOT
void senao_uboot_cli(void)
{
#ifndef CONFIG_SYS_HUSH_PARSER
	int len;
	int len_image;
	int rc = 1;
	int flag = 0;
	int flag_dhcp = 0;
	char *tmp;
	static char tmp_filesize[CONFIG_SYS_CBSIZE] = { 0, };
	static char tmp_partiton[CONFIG_SYS_CBSIZE] = { 0, };
	static char tmp_filename[CONFIG_SYS_CBSIZE] = { 0, };
	int inputc=0;
	int enterc=0;
    int entera=0;
    int enterp=0;
#endif

	rc = run_command ("rtk network on;", flag);
	sprintf((char *)prompt_str,"%s ",">");
	for (;;) {
#ifdef CONFIG_BOOT_RETRY_TIME
		if (rc >= 0) {
			/* Saw enough of a valid command to
			 * restart the timeout.
			 */
			reset_cmd_timeout();
		}
#endif
		printf("\nBOOT MENU : \n");
		printf("(1) server ip \n");
		printf("(2) switch ip \n");
		printf("(3) partition_upgrade \n");
		printf("(4) active_partition \n");
		printf("(5) show information \n");
		printf("(6) ping \n");
		printf("(7) fiber media \n");
		printf("(8) reboot \n");
		printf("(9) factory reset \n");
		printf("(A) upgrade all image \n\n");
		printf("Please enter option number : \n");
		flag = 0;
        inputc=0;
        inputc= getc();
        if(inputc==97) /*a*/
        	entera=1;
        else if (inputc==112) /*p*/
        	enterp=1;
        else if(inputc==99) /*c*/
            enterc=1;
		else if(inputc==49)
		{
			printf("> 1 \n");
			printf("Please enter server ip : \n");
			len = readline ((const char *)prompt_str);
			if (len > 0 && !strstr(console_buffer, ";") && !strstr(console_buffer, "$"))
				rc = uboot_handle_command("setenv serverip ", console_buffer);
			else
				printf("!!!should not enter empty or invalid string. please enter option number again.\n");
		}
		else if(inputc==50)
		{
			printf("> 2 \n\n");
			printf("(1) static \n");
			printf("(2) DHCP \n");
			len = readline ((const char *)prompt_str);
			if (strcmp(console_buffer,"1")==0)
			{
				printf("Please enter switch ip : \n");
				len = readline ((const char *)prompt_str);
				if (len > 0 && !strstr(console_buffer, ";") && !strstr(console_buffer, "$"))
				{
					rc = uboot_handle_command("setenv ipaddr ", console_buffer);
					flag_dhcp = 0;
				}
				else
					printf("!!!should not enter empty or invalid string. please enter option number again.\n");
			}
			else if (strcmp(console_buffer,"2")==0)
				flag_dhcp = 1;
			else if (len == 0)
				printf("!!!should not enter empty string. please enter option number again.\n");
		}
		else if(inputc==51)
		{
			printf("> 3 \n");
			printf("Please enter upgrade partition number (1,2) : \n");
			len = readline ((const char *)prompt_str);
			strcpy (tmp_partiton, console_buffer);
			if (len > 0)
			{
				if (strcmp(tmp_partiton,"1")==0)
				{
					printf("Please enter upgrade image name : \n");
					len_image = readline ((const char *)prompt_str);
					if (len_image > 0 && !strstr(console_buffer, ";") && !strstr(console_buffer, "$"))
					{
						if (flag_dhcp == 1)
							rc = uboot_handle_command("dhcp 0x81000000 ", console_buffer);
						else
							rc = uboot_handle_command("tftpboot 0x81000000 ", console_buffer);
						if(rc == -1)
							continue;
						sprintf (tmp_filesize, "%lx", NetBootFileXferSize);
						rc = run_command ("spi_nand erase 0x4000000 0x1e00000", flag);
						if(rc == -1)
							continue;
						rc = uboot_handle_command("spi_nand write 0x81000000 0x4000000 0x", tmp_filesize);
						if(rc == -1)
							continue;

					}
					else
						printf("!!!should not enter empty or invalid string. please enter option number again.\n");
				}
				else if (strcmp(tmp_partiton,"2")==0)
				{
					printf("Please enter upgrade image name : \n");
					len_image = readline ((const char *)prompt_str);
					if (len_image > 0 && !strstr(console_buffer, ";") && !strstr(console_buffer, "$"))
					{
						if (flag_dhcp == 1)
							rc = uboot_handle_command("dhcp 0x81000000 ", console_buffer);
						else
							rc = uboot_handle_command("tftpboot 0x81000000 ", console_buffer);
						if(rc == -1)
							continue;
						sprintf (tmp_filesize, "%lx", NetBootFileXferSize);
						rc = run_command ("spi_nand erase 0x5e00000 0x1e00000", flag);
						if(rc == -1)
							continue;
						rc = uboot_handle_command("spi_nand write 0x81000000 0x5e00000 0x", tmp_filesize);
						if(rc == -1)
							continue;

					}
					else
						printf("!!!should not enter empty or invalid string. please enter option number again.\n");
				}
				else
					printf("\n!!!wrong partition number. please enter partition number 1 or 2.\n");
			}
			else if (len == 0)
				printf("!!!should not enter empty string. please enter option number again.\n");
		}
		else if(inputc==52)
		{
			printf("> 4 \n");
			printf("Please enter partition number (1,2) : \n");
			len = readline ((const char *)prompt_str);
			if (len > 0)
			{
				if (strcmp(console_buffer,"1")==0)
				{
					rc = uboot_handle_command("setsys bootpartition ", "0");
					rc = run_command ("savesys", flag);
				}
				else if (strcmp(console_buffer,"2")==0)
				{
					rc = uboot_handle_command("setsys bootpartition ", "1");
					rc = run_command ("savesys", flag);
				}
				else
					printf("\n!!!wrong partition number. please enter partition number 1 or 2.\n");
			}
			else if (len == 0)
				printf("!!!should not enter empty string. please enter option number again.\n");
		}
		else if(inputc==53)
		{
			printf("> 5 \n");
			if ((tmp = getenv ("serverip")) != NULL) {
				printf ("\nserver ip=%s \n", tmp);
			}
			if ((tmp = getenv ("ipaddr")) != NULL) {
				printf ("switch ip=%s \n", tmp);
			}
			if ((tmp = getsys ("bootpartition")) != NULL) {
				if (strcmp(tmp,"0")==0)
					printf ("boot partition=%d \n", 1);
				else if (strcmp(tmp,"1")==0)
					printf ("boot partition=%d \n", 2);
			}
		}
		else if(inputc==54)
		{
			printf("> 6 \n");
			printf("Please enter ping ip : \n");
			len = readline ((const char *)prompt_str);
			if (len > 0 && !strstr(console_buffer, ";") && !strstr(console_buffer, "$"))
				rc = uboot_handle_command("ping ", console_buffer);
			else
				printf("!!!should not enter empty or invalid string. please enter option number again.\n");
		}
		else if(inputc==55)
		{
			printf("> 7 \n\n");
			printf("(1) FIBER_10G \n");
			printf("(2) FIBER_1G \n");
			printf("Please enter fiber media type : \n");
			len = readline ((const char *)prompt_str);
			GpioboardInfoInit();
			PortInfoInit();
			rtk_extGpio_pin_set(0, 10, 0);
			rtk_extGpio_pin_set(0, 13, 0);
			rtk_extGpio_pin_set(0, 22, 0);
			rtk_extGpio_pin_set(0, 26, 0);
			if (len > 0)
			{
				if (strcmp(console_buffer,"1")==0)
				{
					rtk_port_10gMedia_set(0, PortMapSdkPortGet(0), PORT_10GMEDIA_FIBER_10G);;
					rtk_port_10gMedia_set(0, PortMapSdkPortGet(1), PORT_10GMEDIA_FIBER_10G);
					rtk_port_10gMedia_set(0, PortMapSdkPortGet(2), PORT_10GMEDIA_FIBER_10G);
					rtk_port_10gMedia_set(0, PortMapSdkPortGet(3), PORT_10GMEDIA_FIBER_10G);
				}
				else if (strcmp(console_buffer,"2")==0)
				{
					rtk_port_10gMedia_set(0, PortMapSdkPortGet(0), PORT_10GMEDIA_FIBER_1G);
					rtk_port_10gMedia_set(0, PortMapSdkPortGet(1), PORT_10GMEDIA_FIBER_1G);
					rtk_port_10gMedia_set(0, PortMapSdkPortGet(2), PORT_10GMEDIA_FIBER_1G);
					rtk_port_10gMedia_set(0, PortMapSdkPortGet(3), PORT_10GMEDIA_FIBER_1G);
				}
				else
				{
					printf("\n!!!wrong fiber meadia type. please enter option number again.\n");
				}
			}
			else if (len == 0)
				printf("!!!should not enter empty string. please enter option number again.\n");
		}
		else if(inputc==56)
		{
			printf("> 8 \n");
			printf("rebooting . . . \n");
			rc = run_command ("reset", flag);
		}
		else if(inputc==57)
		{
			printf("> 9 \n");
			printf("restore to defaults . . . \n");
			rc = uboot_handle_command("setenv reset_flag ", "1");
			rc = run_command ("saveenv", flag);
			rc = run_command ("reset", flag);
		}
		else if(inputc==65)
		{
			printf("> A \n");
			printf("Please enter upgrade nand flash image name : \n");
			len = readline ((const char *)prompt_str);
			if (len == 0 || strstr(console_buffer, ";") || strstr(console_buffer, "$"))
			{
				printf("!!!should not enter empty or invalid string. please enter option number again.\n");
				continue;
			}
			strcpy (tmp_filename, console_buffer);
			printf("Please enter upgrade nor flash image name : \n");
			len_image = readline ((const char *)prompt_str);
			tmp = getenv ("rescue_flag");
			if (len_image > 0 && !strstr(console_buffer, ";") && !strstr(console_buffer, "$"))
			{
				rc = uboot_handle_command("tftpboot 0x86000000 ", tmp_filename);
				if(rc == -1)
					continue;
				if(NetBootFileXferSize <= 138412032 && NetBootFileXferSize >= 131518464)
				{
					rc = run_command ("spi_nand erase 0x0 0x8000000", flag);
					if(rc == -1)
						continue;
					if(NetBootFileXferSize % 2112 == 0)
					{
						sprintf (tmp_filesize, "%lx", NetBootFileXferSize/2112);
						rc = uboot_handle_command("spi_nand write.raw 0x86000000 0x0 0x", tmp_filesize);
						if(rc == -1)
							continue;
					}
				}
				else
				{
					printf("!!!filesize is not equal to 125MB.\n");
					continue;
				}
				rc = uboot_handle_command("tftpboot 0x81000000 ", console_buffer);
				if(rc == -1)
					continue;
				if (strcmp(tmp,"rescue_none")==0)
				{
					if (NetBootFileXferSize >= 2097152)
					{
						rc = run_command ("sf probe 0", flag);
						rc = run_command ("sf erase 0x0 0x200000", flag);
						if(rc == -1)
							continue;
						rc = run_command ("sf write 0x81000000 0 0x200000", flag);
						if(rc == -1)
							continue;
					}
					else
					{
						printf("!!!filesize is not bigger than 2MB.\n");
						continue;
					}
				}
				else if (strcmp(tmp,"rescue")==0)
				{
					if (NetBootFileXferSize >= 16777216)
					{
						rc = run_command ("sf probe 0", flag);
						rc = run_command ("sf erase 0x0 0x1000000", flag);
						if(rc == -1)
							continue;
						rc = run_command ("sf write 0x81000000 0 0x1000000", flag);
						if(rc == -1)
							continue;
					}
					else
					{
						printf("!!!filesize is not bigger than 16MB.\n");
						continue;
					}
				}
				else
					printf("!!!rescue flag is not be set.\n");
			}
			else
			{
				printf("!!!should not enter empty or invalid string. please enter option number again.\n");
			}
		}
		else
		{
			entera=0;
            enterp=0;
            enterc=0;
		}
		if(entera && enterp && enterc) /*enter key*/
        {
			return;
        }
	}
}
#endif
