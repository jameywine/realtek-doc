/*
 *  Copyright (c) 2014  TP-LINK Co., Ltd
 *
 *  TP-LINK Co., Ltd R&D SMB Product Line
 *
 */

#include <common.h>
#include <asm/io.h>
//#include <asm/sections.h>
#include "dt_common.h"

extern ulong uboot_end_data;
extern ulong uboot_end;
extern char uboot_start;

dramMemRange memoryMap[MapSize] =
             {{"LOWER RAM",  
                     CONFIG_DRAM_TEST_LOWER_RAM_BASE, 
                     CONFIG_DRAM_TEST_LOWER_RAM_SIZE},
              {"UPPER RAM",  
                     CONFIG_DRAM_TEST_UPER_RAM_BASE, 
                     CONFIG_DRAM_TEST_UPER_RAM_SIZE},
              {NULL, 0, 0},
              {NULL, 0, 0},
              {NULL, 0, 0},};


static int do_dram_test_help(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
static int dramTestDramSize = CONFIG_DRAM_TEST_DRAM_SIZE;
static int dramTestDataBusWidth = CONFIG_DRAM_TEST_DATABUS_WIDTH;

/*
 * "dramtest" command with subcommands
 */
static cmd_tbl_t cmd_dramtest_sub[] = {
    U_BOOT_CMD_MKENT(mtest,     6, 0, do_dram_mtest_test,   
                    "simple RAM read/write test", 
                    "dram mtest [test_type [[count [start_addr end_addr [pattern]]]] \n"
                    "     - simple RAM read/write test\n"
                    "     > test_type = 0; a quick mtest (dram_mtest_quick)\n"
                    "     > test_type = 1; a expert mtest(dram_mtest_alt)\n"
                    "     - If do not specify A test area, use the default test area\n"
                    ""),
    U_BOOT_CMD_MKENT(databus,   5, 0, do_dram_databus_test, 
                    "dram data bus test", 
                    "dram databus [count [start_addr end_addr [pattern]]] \n"
                    "     - dram data bus test\n"
                    "     - Aimed at detecting problems with the DDR data pins (shorts,\n" 
                    "       opens, and stuck at failures), it applies the patterns to the \n"
                    "       memory address, thus limiting exposure to addressing faults \n"
                    "       and/or memory device faults\n"
                    "     - If do not specify A test area, use the default test area\n"
                    ""), 
    U_BOOT_CMD_MKENT(addrbus,   5, 0, do_dram_addrbus_test, 
                    "dram address bus test", 
                    "dram addrbus [count [start_addr end_addr [pattern]]] \n"
                    "     - dram address bus test\n"
                    "     - Aimed at detecting problems with the DDR address pins (shorts, \n"
                    "       opens, and stuck at filures)"
                    "     - If do not specify A test area, use the default test area\n"
                    ""), 
    U_BOOT_CMD_MKENT(fade,      6, 0, do_dram_fade_test,    
                    "dram memory fade test", 
                    "dram fade [minutes [count [start_addr end_addr [pattern]]]] \n"
                    "     - dram memory fade test.\n"
                    "     - Check long term data retention of the store.\n"
                    "     - Writes the pattern to the entire memory and then \n"
                    "       goes to sleep for 90 minutes. After that period, the test checks\n"
                    "       all of memory looking for any changes in the pattern.\n"
                    "     - If do not specify A test area, use the default test area\n"
                    ""), 
    U_BOOT_CMD_MKENT(rmw,       5, 0, do_dram_rwm_test,     
                    "dram read-modify-write test", 
                    "dram rmw [count [start_addr end_addr [pattern]]] \n"
                    "     - dram read-modify-write test\n"
                    "     - generates read-modify-write accesses to random locations.\n"
                    "     - If do not specify A test area, use the default test area\n"
                    ""), 
                    
    U_BOOT_CMD_MKENT(read,      6, 0, do_dram_read_test,    
                    "dram read test", 
                    "dram read [count [line_type [start_addr end_addr [pattern]]]] \n"
                    "     - continuous reads to the memory locations\n"
                    "     - generate the signals for measurement by an oscilliscope\n"
                    "     > line_type = 0: test data line(default);\n"
                    "     > line_type = 1: test address line;\n"
                    "     - If do not specify A test area, use the default test area\n"
                    ""), 
                    
    U_BOOT_CMD_MKENT(write,     6, 0, do_dram_write_test,   
                    "dram write test", 
                    "dram write [count [line_type [start_addr end_addr [pattern]]]] \n"
                    "     - continuous writes from the memory locations\n"
                    "     - generate the signals for measurement by an oscilliscope\n"
                    "     > line_type = 0: test data line(default);\n"
                    "     > line_type = 1: test address line;\n"
                    "     - If do not specify A test area, use the default test area\n"
                    ""), 
    U_BOOT_CMD_MKENT(readwrite, 6, 0, do_dram_rw_test,      
                    "dram read-write test", 
                    "dram readwrite [count [line_type [start_addr end_addr [pattern]]]] \n"
                    "     - continuous read-write from the memory locations\n"
                    "     - generate the signals for measurement by an oscilliscope\n"
                    "     > line_type = 0: test data line;(default)\n"
                    "     > line_type = 1: test address line;\n"
                    "     - If do not specify A test area, use the default test area\n"
                    ""), 
    U_BOOT_CMD_MKENT(xtalk,     5, 0, do_dram_xtalk_test,   
                    "dram crosstalk test", 
                    "dram xtalk [count [[line_type=0/1] [level_type=0/1] [line_bit]]]\n"
                    "     > line_type  = 0: test data line(default);\n"
                    "     > line_type  = 1: test address line;\n"
                    "     > level_type = 0: electrical level, low  level(default);\n"
                    "     > level_type = 1: electrical level, high level;\n"
                    "     > line_bit : the bit of line(default test all lines);\n"
                    "     - If do not specify A test area, use the default test area\n"
                    ""),
    U_BOOT_CMD_MKENT(pattern,    4, 0, do_dram_pattern_test,  
                    "dram pattern test", 
                    "dram pattern [count [start_addr end_addr]] \n"
                    "     - all tests will use the default pattern.\n"
                    ""), 
    U_BOOT_CMD_MKENT(stable,    2, 0, do_dram_stable_test,  
                    "dram stable test", 
                    "dram stable [count] \n"
                    "     - excute all dram test except for fade [count] times\n"
                    "     - if do not specify [count], cycle all test until user's interruption or error occurs\n"
                    ""), 
    U_BOOT_CMD_MKENT(all,    3, 0, do_dram_all_test,  
                    "dram all test", 
                    "dram all [count [minutes]] \n"
                    "     - excute all dram test [count] times\n"
                    "     - if do not specify [count], cycle all test until user's interruption or error occurs\n"
                    "     - [minutes] specify the dram fade test time (90 default)\n"
                    ""),
    U_BOOT_CMD_MKENT(help, CONFIG_SYS_MAXARGS, 0, do_dram_test_help,    
                    "get help for dram subcommand", 
                    "dram help [subcommand ...] \n"
                    "     - get help for subcommand\n"
                    ""),
};
	

static int do_dram_test_help(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    return _do_help(&cmd_dramtest_sub[0], ARRAY_SIZE(cmd_dramtest_sub),
                    cmdtp, flag, argc, argv);
}
static int do_dram_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
    cmd_tbl_t *cp = NULL;

    if (argc < 2)
        return CMD_RET_USAGE;

    /* drop initial arg */
    argc--;
    argv++;

    cp = find_cmd_tbl(argv[0], cmd_dramtest_sub, ARRAY_SIZE(cmd_dramtest_sub));

    if (cp)
    {
		/*printf("Disable icache.....");
		icache_disable();
		printf("done\n");
		printf("Disable dcache.....");
		dcache_disable();
		printf("done\n");*/
		
        ret = cp->cmd(cmdtp, flag, argc, argv);

		/*printf("Enable icache.....");
		icache_enable();
		printf("done\n");
		printf("Enable dcache.....");
		dcache_enable();
		printf("done\n");*/
    }

    return ret;
}

static char dramtest_help_text[] =
    "mtest [test_type [[count [start_addr end_addr [pattern]]]] \n"
    "     - simple RAM read/write test\n"
    "dram databus [count [start_addr end_addr [pattern]]] \n"
    "     - dram data bus test\n"
    "dram addrbus [count [start_addr end_addr [pattern]]] \n"
    "     - dram address bus test\n"
    "dram fade [minutes [count [start_addr end_addr [pattern]]]] \n"
    "     - dram memory fade test.\n"
    "dram rmw [count [start_addr end_addr [pattern]]] \n"
    "     - dram read-modify-write test\n"
    "dram read [count [line_type [start_addr end_addr [pattern]]]] \n"
    "     - continuous reads to the memory locations\n"
    "dram write [count [line_type [start_addr end_addr [pattern]]]] \n"
    "     - continuous writes from the memory locations\n"
    "dram readwrite [count [line_type [start_addr end_addr [pattern]]]] \n"
    "     - continuous read-write from the memory locations\n"
    "dram xtalk [count [[line_type=0/1] [level_type=0/1] [line_bit]]] \n"
    "     - dram crosstalk test\n"
    "dram pattern [count [start_addr end_addr]] \n"
    "     - dram pattern test\n"
    "dram stable [run_type [start_addr end_addr]] \n"
    "     - dram stable test\n"
    "dram help [subcommand ...] \n"
    "     - get help for dram subcommand\n";

U_BOOT_CMD(
    dram, CONFIG_SYS_MAXARGS, 1, do_dram_test,
    "dramtest commands", dramtest_help_text
);


static int do_ctrlc_disable(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    extern int disable_ctrlc(int disable);
    
    uint32_t ctrlc_disable = 0;
    
    if (argc > 1)
        ctrlc_disable = (uint32_t)simple_strtoul(argv[1], NULL, 10);
    else
        ctrlc_disable = 0;
    
    disable_ctrlc(ctrlc_disable);
    
    return 0;
}

U_BOOT_CMD(
    ctrlc, CONFIG_SYS_MAXARGS, 0, do_ctrlc_disable,    
    "disable ctrl+c commands", 
    "ctrlc [ctrlc_disable] \n"
    "     > ctrlc_disable = 0 : enable ctrl+c(default)\n"
    "     > ctrlc_disable = 1 : disable ctrl+c\n"
    ""
);

/* make sure we run in right place */
int dt_probe_position( void )
{
    int run_position = POS_IN_NULL_RANGE;

#if 0
    uint32_t offset, image_size, padding_size, image_base;
    image_size = (ulong)&uboot_end_data - CONFIG_SYS_MONITOR_BASE;
    padding_size = 16 - (image_size  % 16);
    offset = image_size + padding_size;
    image_base = (ulong)&uboot_start;


    printf("CONFIG_SYS_MONITOR_BASE %p\n", CONFIG_SYS_MONITOR_BASE);
    printf("(ulong)&uboot_end_data %p\n", (ulong)&uboot_end_data);
    printf("(ulong)&uboot_start %p\n", (ulong)&uboot_start);
    printf("uboot_end %p\n", uboot_end);
    printf("image_size %p\n", image_size);
    printf("padding_size %p\n", padding_size);
    printf("offset %p\n", offset);
    printf("image_base %p\n", image_base);

    if (image_base == CONFIG_SDRAM_LOWER_RANGE_BASE)
    {
        run_position = POS_IN_LOWER_BASE;
    }
    else if (image_base == CONFIG_SDRAM_UPPER_RANGE_BASE)
    {
        run_position = POS_IN_UPPER_BASE;
    }
    else if((image_base > CONFIG_SDRAM_LOWER_RANGE_BASE) && 
            (image_base < CONFIG_SDRAM_UPPER_RANGE_BASE))
    {
        if(CONFIG_MALLOC_BASE + CONFIG_SYS_MALLOC_LEN < CONFIG_SDRAM_UPPER_RANGE_BASE)
            run_position = POS_IN_LOWER_RANGE;
        else
        {
            if(image_base - CONFIG_SDRAM_LOWER_RANGE_BASE >= image_size)
            {
                run_position = POS_IN_BOTH_RANGE;   
            }
            else
            {
                run_position = POS_IN_NULL_RANGE; 
            }
        }
            
    }
    else if((image_base > CONFIG_SDRAM_UPPER_RANGE_BASE) && 
            (image_base < (CONFIG_SYS_SDRAM_BASE + CONFIG_SDRAM_TOTAL_SIZE)))
        run_position = POS_IN_UPPER_RANGE;
    else
        run_position = POS_IN_NULL_RANGE;

    printf("Addresses 0x%08lX - 0x%08lX are reserved NOW.\n\n", 
         (ulong)(image_base), (ulong)(CONFIG_MALLOC_BASE + CONFIG_SYS_MALLOC_LEN));

    printf("run_position %p\n", run_position);

#endif
    run_position = POS_IN_LOWER_BASE;

    return run_position;
}

#ifdef CONFIG_IMAGE_COPY
static int do_image_relocation(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char   accept_char, force_accept = 0;
    char   temp_str[CONFIG_SYS_PBSIZE];
    int    run_position;
    ulong  addr = 0, reloc_addr;

    if (argc > 1) {
        if(strcmp(argv[1], "auto") == 0)
        {
            run_position = dt_probe_position();
            force_accept = 1;
        }    
        else
        {
           run_position = POS_IN_ASSIGN_ADR;
           addr = (uint32_t)simple_strtoul(argv[1], NULL, 16); 
        }
    }
    else
    {
        /* defalut auto */
        run_position = dt_probe_position();
        force_accept = 0;
    }

    if(run_position == POS_IN_NULL_RANGE)
    {
        printf("can NOT relocate image!\n");
        return -1;
    }

    
    if(run_position == POS_IN_ASSIGN_ADR)
    {
        reloc_addr = addr;
    }
    else if((run_position == POS_IN_LOWER_RANGE) || 
            (run_position == POS_IN_LOWER_BASE) )
    {
        reloc_addr = CONFIG_SDRAM_UPPER_RANGE_BASE;
    }
    else if((run_position == POS_IN_UPPER_RANGE) || 
            (run_position == POS_IN_BOTH_RANGE)  ||
            (run_position == POS_IN_UPPER_BASE) )
    {
        reloc_addr = CONFIG_SDRAM_LOWER_RANGE_BASE;
    }
    else
    {
        printf("can NOT relocate image!\n");
        return -1;
    }
    
    memset(temp_str, 0, CONFIG_SYS_PBSIZE);
    sprintf(temp_str, "cpy 0x%08lx;go 0x%08lx", reloc_addr, reloc_addr);
    printf("CMD: %s\n\n", temp_str);
    
    if(force_accept == 0)
    {
        printf("Relocate image to ASSIGN Addresses? (enter 'Y' to accept)?");
        accept_char = getc();
        if (accept_char == 'Y' || accept_char == 'y') 
        {
            puts("\n");
            run_command_list(temp_str, -1, 0);
            return 1;
        }
    }
    else
    {
        run_command_list(temp_str, -1, 0);
        return 1;
    }
    puts("\n");
    
    return 1;
}

U_BOOT_CMD(
    reloc, CONFIG_SYS_MAXARGS, 0, do_image_relocation,
    "relocate the operating position", 
    "reloc [auto | addr]\n"
    "     - relocate image at address 'addr' or relocate automatic[defalult]\n"
);
#endif

int dt_dramDataBusWidth (void)
{    
    return dramTestDataBusWidth;
}

int dt_dramAddrBusWidth (void)
{
    int size_mem = dramTestDramSize, width = 0;

    while(1)
    {
        size_mem = size_mem >> 1;
        if(size_mem > 0)
            width++;
        else
            break;
    }

    return width;
}


#define READ_TIMER (get_timer_masked())

/*
 * pseudo-random mumber generator
 */
#if 0
static long holdrand = 1L; 
void srand(unsigned long seed)
{ 
    holdrand = (long)seed;
} 

int rand(void)
{
    /* 产生整数rand的原理是：   
     * y＝(ax＋b)(mod m) 其中，m一般是一个很大的素数。   
     * a也是大素数。而且a，b，n都是常数。所以rand的产生决定于x，   
     * 被称为seed。 
     */
    return (((holdrand = holdrand * 214013L + 2531011L) >> 16) & 0x7fff);   
}
#endif
/*
 * return : return a random number by randmask
 * the max value is randmask,default  is 0xFFFFFFFF
 */


/* ***********************rand************************** */
static unsigned int dt_seed_ = 1U;
static unsigned int rand_r(unsigned int *seedp)
{
	*seedp ^= (*seedp << 13);
	*seedp ^= (*seedp >> 17);
	*seedp ^= (*seedp << 5);

	return *seedp;
}

static unsigned int rand(void)
{
	return rand_r(&dt_seed_);
}

static void srand(unsigned int seed)
{
	dt_seed_ = seed;
}
/* ***************************************************** */

int dt_random_generate(int randmask)
{
    if(randmask == 0)
        randmask = 0xFFFFFFFF;
    
    ulong now = get_timer(0);
    debug("now : 0x%08lx\n", now);
    srand(now);
    return (((rand() << 16) | rand()) & randmask);
}

#if 0
static int do_randrom_printf(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint32_t random_mask;
    
    if (argc > 1) {
        random_mask = (uint32_t)simple_strtoul(argv[1], NULL, 16);
    } else {
        random_mask = (uint32_t)0xFFFFFFFF;
    }
    
    while(1)
    {
        CHECK_CTRL_C();
        DT_UINT rand = dt_random_generate(random_mask);
        printf("    Random(Mask:0x%08x) : 0x%08x\n", random_mask, rand);
    }
    
    return 1;
}

U_BOOT_CMD(
    rdp, 6, 1, do_randrom_printf,
    "printf random num.",
    ""  
);
#endif

int dt_minutes_delay (int minutes)
{
    ulong count, seconds, mseconds;

    for(count = minutes ; count > 0; count--)
    {
        for(seconds = 60; seconds > 0; seconds--)
        {
            /* check for Interrupted.*/
            if (ctrlc()) 
            {
                putc ('\n'); 
                printf("Interrupted by user\n"); 
                return -1; 
            }
            
            printf("\r  Time  %02ld : %02ld", count - 1, seconds - 1);
            
            for(mseconds = 1000; mseconds > 0; mseconds--)
            {
                udelay(1000);
            }
        }
    }
    return 0;
}

int updateDramTestParam(void)
{
	#define CHECK_NULL_RET(a)\
	do\
	{\
		if(NULL == (a))\
		{\
			return -1;\
		}\
	}while(0)
	
	char *retChar = NULL;
		
	CHECK_NULL_RET(retChar = getenv(DRAM_TEST_DRAM_SIZE));
	dramTestDramSize = simple_strtoul(retChar, NULL, 10);

	CHECK_NULL_RET(retChar = getenv(DRAM_TEST_DATABUS_WIDTH));
	dramTestDataBusWidth = simple_strtoul(retChar, NULL, 10);

	CHECK_NULL_RET(retChar = getenv(DRAM_TEST_LOWER_RAM_BASE));
	memoryMap[0].base = simple_strtoul(retChar, NULL, 10);
	
	CHECK_NULL_RET(retChar = getenv(DRAM_TEST_LOWER_RAM_SIZE));
	memoryMap[0].extent = simple_strtoul(retChar, NULL, 10);

	CHECK_NULL_RET(retChar = getenv(DRAM_TEST_UPER_RAM_BASE));
	memoryMap[1].base = simple_strtoul(retChar, NULL, 10);
	
	CHECK_NULL_RET(retChar = getenv(DRAM_TEST_UPER_RAM_SIZE));
	memoryMap[1].extent = simple_strtoul(retChar, NULL, 10);

	return 0;
}

