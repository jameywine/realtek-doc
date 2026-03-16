
#include "spi_nand_struct.h"


/* Following APIs are listed in mtdcore.h */
extern int add_mtd_device(struct mtd_info *mtd);
extern int add_mtd_partitions(struct mtd_info *, const struct mtd_partition *, int);
extern int del_mtd_partitions(struct mtd_info *);
extern int parse_mtd_partitions(struct mtd_info *master, const char * const *types,struct mtd_partition **pparts,struct mtd_part_parser_data *data);
/* Above APIs are listed in mtdcore.h */

#define PAD_PSR0                    0xb8000100
#define PAD_PSR0_CS1_MUX_SEL_OFF    13
#define PAD_PSR0_CS1_MUX_SEL_MASK   ((0x3)<<PAD_PSR0_CS1_MUX_SEL_OFF)

#ifndef REGISTER_REG32
#define REGISTER_REG32(reg)		(*(volatile unsigned int   *)(reg))
#endif

extern spi_nand_flash_info_t * probe_gd_spi_nand_chip(void);
extern spi_nand_flash_info_t * probe_mxic_spi_nand_chip(void);
extern spi_nand_flash_info_t * probe_micron_spi_nand_chip(void);
extern spi_nand_flash_info_t * probe_toshiba_spi_nand_chip(void);
extern spi_nand_flash_info_t * probe_winbond_spi_nand_chip(void);

extern int gd_init_rest(spi_nand_flash_info_t * _spi_nand_info);
extern int mxic_init_rest(spi_nand_flash_info_t * _spi_nand_info);
extern int toshiba_init_rest(spi_nand_flash_info_t * _spi_nand_info);
extern int winbond_init_rest(spi_nand_flash_info_t * _spi_nand_info);


void rtk_luna_cs_pin_set(void);
spi_nand_flash_info_t * probe_spi_nand_chip_func(void);
int probe_spi_nand_chip_init_rest(spi_nand_flash_info_t * _spi_nand_info);


