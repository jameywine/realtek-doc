/*
 * Copyright (C) 2009-2016 Realtek Semiconductor Corp.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * $Revision: 71708 $
 * $Date: 2016-09-19 11:31:17 +0800 (Mon, 19 Sep 2016) $
 *
 * Purpose : Realtek Switch SDK Rtusr API Module
 *
 * Feature : The file have include the following module and sub-modules
 *           1) utility
 *
 */

#ifndef __RTUSR_UTIL_H__
#define __RTUSR_UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>


#define SETSOCKOPT(optid, varptr, vartype, qty) \
    do { \
        unsigned int ret; \
        if ((ret = rtusr_msg_setsockopt(optid, varptr, (sizeof(vartype) * qty))) != RT_ERR_OK) {\
            return ret; \
        }\
    } while (0)

#define GETSOCKOPT(optid, varptr, vartype, qty) \
    do { \
        unsigned int ret; \
        if ((ret = rtusr_msg_getsockopt(optid, varptr, (sizeof(vartype) * qty))) != RT_ERR_OK) {\
            return ret; \
        }\
    } while (0)




extern int
rtusr_msg_setsockopt(unsigned int optid, void *varptr, unsigned int size);
extern int
rtusr_msg_getsockopt(unsigned int optid, void *varptr, unsigned int size);

#endif /* __RTUSR_UTIL_H__ */
