/*
 * Interface to SPI flash
 *
 * Copyright (C) 2008 Atmel Corporation
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_

#include <spi.h>
#include <linux/types.h>

struct spi_flash {
	struct spi_slave *spi;

	const char	*name;

	/* Total flash size */
	u32		size;
	/* Write (page) size */
	u32		page_size;
	/* Erase (sector) size */
	u32		sector_size;

	int		(*read)(struct spi_flash *flash, u32 offset,
				size_t len, void *buf);
	int		(*write)(struct spi_flash *flash, u32 offset,
				size_t len, const void *buf);
	int		(*erase)(struct spi_flash *flash, u32 offset,
				size_t len);
};

struct spi_flash *spi_flash_probe(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int spi_mode);
void spi_flash_free(struct spi_flash *flash);

static inline int spi_flash_read(struct spi_flash *flash, u32 offset,
		size_t len, void *buf)
{
	return flash->read(flash, offset, len, buf);
}

static inline int spi_flash_write(struct spi_flash *flash, u32 offset,
		size_t len, const void *buf)
{
	return flash->write(flash, offset, len, buf);
}

static inline int spi_flash_erase(struct spi_flash *flash, u32 offset,
		size_t len)
{
	return flash->erase(flash, offset, len);
}


#define UNIT_SIZE 65536 // 64Kb
#define LOADER_START (0x00000000)
#define LOADER_SIZE (UNIT_SIZE*14)	//896K
//#define LOADER_BDINFO_SIZE (UNIT_SIZE*2) //128k
#define RTK_KERNEL_START (UNIT_SIZE*16) //1M
#define RTK_KERNEL_SIZE (UNIT_SIZE*16*6) //6M
#define UIMAGE1_START (UNIT_SIZE*16*7) //7M
#define UIMAGE1_SIZE (UNIT_SIZE*16*10) //10M
#define UIMAGE2_START (UNIT_SIZE*16*17) //7M
#define UIMAGE2_SIZE (UNIT_SIZE*16*10) //10M	
#define USR_APP_START (UNIT_SIZE*16*27) //27M
#define USR_APP_SIZE (UNIT_SIZE*16*4) //4M

#ifdef LOADER_BDINFO_SIZE
#undef LOADER_BDINFO_SIZE
#define LOADER_BDINFO_SIZE (UNIT_SIZE*2) //128k
#endif

#ifdef LOADER_BDINFO_START
#undef LOADER_BDINFO_START
#define LOADER_BDINFO_START (UNIT_SIZE*14) //896k
#endif	
#define KERNEL2_START (UNIT_SIZE*16) //1M





#endif /* _SPI_FLASH_H_ */
