/*
 *  Copyright (c) 2014  TP-LINK Co., Ltd
 *
 *  TP-LINK Co., Ltd R&D SMB Product Line
 *
 */

#include <common.h>
#include <command.h>
#include "dt_common.h"

const DT_UINT dram_08bit_databus_patterns[] = {
        0x00FF00FF,
        0x55AA55AA,
        0xCC33CC33,
        0xDD22DD22,
        0xFF00FF00,
        0xAA55AA55,
        0x33CC33CC,
        0x22DD22DD,
};

const DT_UINT dram_16bit_databus_patterns[] = {
        0x0000FFFF,
        0x5555AAAA,
        0xCCCC3333,
        0xDDDD2222,
        0xFFFF0000,
        0xAAAA5555,
        0x3333CCCC,
        0x2222DDDD,
};

const DT_UINT dram_32bit_databus_patterns[] = {
        0xFFFFFFFF,
        0x00000000,
        0xAAAAAAAA,
        0x55555555,
        0xCCCCCCCC,
        0x33333333,
        0xDDDDDDDD,
        0x22222222,
};

const DT_UINT dram_databus_walks_pattern[] = {
        0x00000001,
        0x00000003,
        0x00000007,
};


testPatten tPat_databus = {1, 0x5A5A5A5A};

/* databus
 *
 * The tests contained herein are designed to exercise the data bus (DQ pins)
 * of the Dram memory interface. 
 */

/*-----------------------*/
/*  test_data_bus_burst  */
/*-----------------------*/
/* Writes a fixed pattern and its compliment to the dram data bus, and trie
 * to read back the same pattern from the bus. This pattern appears 'bursts' 
 * times on the data bus.
 */
int32_t
test_data_bus_burst (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t    i, j, bus_width, failures, ret_err;
    
    ret_err = 0;
    failures = 0;

    bus_width = dt_dramDataBusWidth();

    for (i = 0; i < bursts; i = i + 1) 
    {
        CHECK_CTRL_C();

        printf("\n"
               "[data bus burst]\n"
               "  Bursts: %3d\n", (i + 1));
        
        if(tPat_databus.auto_en == 0)
        {
            printf("  pattern = 0x%08lX ... ", (ulong)tPat_databus.type);
            ret_err = test_mem_pattern(area, max_address, tPat_databus.type, 1);
            if(ret_err == -1)
            {
                return -1;
            }
            
            if (ret_err == 0) 
            {
                printf(" [PASS]\n");
            }
            
            failures += ret_err;
        }
        else  /* use default pattern */
        {
            switch (bus_width) {  
                case  8:
                    for (j=0; j < (sizeof(dram_08bit_databus_patterns)/sizeof(DT_UINT)); j++) 
                    {
                        printf("  pattern = 0x%08lX ... ", (ulong)dram_08bit_databus_patterns[j]);
                        ret_err = test_mem_pattern(area, max_address,
                                         dram_08bit_databus_patterns[j], 1);
                        if(ret_err == -1)
                        {
                            return -1;
                        }
                        
                        if (ret_err == 0) 
                        {
                            printf(" [PASS]\n");
                        }
                        
                        failures += ret_err;
                    }
                    break;
                case 16:
                    for (j=0; j < (sizeof(dram_16bit_databus_patterns)/sizeof(DT_UINT)); j++) 
                    {
                        printf("  pattern = 0x%08lX ...", (ulong)dram_16bit_databus_patterns[j]);
                        ret_err = test_mem_pattern(area, max_address, 
                                         dram_16bit_databus_patterns[j], 1);
                        
                        if(ret_err == -1)
                        {
                            return -1;
                        }
                        
                        if (ret_err == 0) 
                        {
                            printf(" [PASS]\n");
                        }
                        
                        failures += ret_err;
                    }
                    break;
                case 32:
                    for (j=0; j < (sizeof(dram_32bit_databus_patterns)/sizeof(DT_UINT)); j++)
                    {
                        printf("  pattern = 0x%08lX ...", (ulong)dram_32bit_databus_patterns[j]);
                        ret_err = test_mem_pattern(area, max_address, 
                                         dram_32bit_databus_patterns[j], 1);
                        
                        if(ret_err == -1)
                        {
                            return -1;
                        }
                        
                        if (ret_err == 0) 
                        {
                            printf(" [PASS]\n");
                        }
                        
                        failures += ret_err;
                    }
                    break;
            }
        }
    }

    return (failures);
}

/*-----------------------------*/
/*  write_08bit_data_bus_walk  */
/*-----------------------------*/
/* Create a Walking Ones and Walking Zeros across dram memory bus.
 */
static int32_t
write_08bit_data_bus_walk (DT_UINT pattern, DT_UINT area, DT_UINT max_address)
{
    DT_UINT  mask;
    uint32_t j, k, failures;
    DT_UINT  *p1, *p2;
    DT_UINT  p1addrfix, p2addrfix, count, extent;

    extent = max_address - area;
    count  = (extent / sizeof (DT_UINT)) / 2;
    count  = count - (count % 4);

    if(count <= 16)
    {
        printf("Error: the Tests Requires at least 32 * %zu bytes of space!\n", sizeof(DT_UINT));
        return -1;
    }

    p1addrfix = area & 0xFFFFFFFC;
    p2addrfix = (area + (extent / 2)) & 0xFFFFFFFC;

    /* Fill both halves of the memory area.
     */
    p1 = (DT_UINT *) p1addrfix;
    p2 = (DT_UINT *) p2addrfix;

    failures = 0;

    mask = pattern;
    
    for(j = 0; j < count; j = j + 4)
    {
        /* Form Entry -- walk one's */
        p1[j]   =  mask;
        p1[j+1] = ~p1[j];
        
        p1[j+2] =  p1[j] << 8;
        p1[j+3] = ~p1[j+2];

        /* Form entry -- walk 0's */
        p2[j]   = ~mask;
        p2[j+1] = ~p2[j];
        
        p2[j+2] = ~(p2[j+1] << 8);
        p2[j+3] = ~p2[j+2];
        
    }

    /* P2 should be the complement of P1 */
    for (k = 0; k < count / 2 ; k = k + 2) 
    {
        CHECK_CTRL_C();
        if ((p1[k] != ~p2[k]) | (p1[k + 1] != ~p2[k + 1]))
        {
            failures = failures + 1;
            printf ("\n[Failure]: \n"
                    "    p1[%2d] 0x%08lX(@0x%08lX)_0x%08lX(@0x%08lX)\n"
                    "    p2[%2d] 0x%08lX(@0x%08lX)_0x%08lX(@0x%08lX)\n"
                    "    xor(0x%08lX_0x%08lX)\n",
                    k, (ulong)p1[k], (ulong)(&p1[k]), (ulong)p1[k+1], (ulong)(&p1[k+1]),
                    k, (ulong)p2[k], (ulong)(&p2[k]), (ulong)p2[k+1], (ulong)(&p2[k+1]),
                    (ulong)(p1[k] ^ p2[k]), (ulong)(p1[k+1] ^ p2[k+1]));
        }
    }

    return (failures);
}


/*-----------------------------*/
/*  write_16bit_data_bus_walk  */
/*-----------------------------*/
/* Create a Walking Ones and Walking Zeros across dram memory bus.
 */
static int32_t
write_16bit_data_bus_walk (DT_UINT pattern, DT_UINT area, DT_UINT max_address)
{
    DT_UINT  mask;
    uint32_t j, k, failures;
    DT_UINT  *p1, *p2;
    DT_UINT  p1addrfix, p2addrfix, count, extent;

    extent = max_address - area;
    count  = (extent / sizeof (DT_UINT)) / 2;
    count  = count - (count % 8);

    if(count <= 16)
    {
        printf("Error: the Tests Requires at least 32 * %zu bytes of space!\n", sizeof(DT_UINT));
        return -1;
    }

    p1addrfix = area & 0xFFFFFFFC;
    p2addrfix = (area + (extent / 2)) & 0xFFFFFFFC;

    /* Fill both halves of the memory area.
     */
    p1 = (DT_UINT *) p1addrfix;
    p2 = (DT_UINT *) p2addrfix;

    failures      = 0;

    mask = pattern;
    
    for(j = 0; j < count; j = j + 8)
    {
        /* Form Entry -- walk one's */
        p1[j  ] =  mask;
        p1[j+1] = ~p1[j];
        
        p1[j+2] =  p1[j] << 4;
        p1[j+3] = ~p1[j+2];

        p1[j+4] =  p1[j+2] << 4;
        p1[j+5] = ~p1[j+4];

        p1[j+6] =  p1[j+4] << 4;
        p1[j+7] = ~p1[j+6];

        /* Form entry -- walk 0's */
        p2[j  ] = ~mask;
        p2[j+1] = ~p2[j];
        
        p2[j+2] = ~(p2[j+1] << 4);
        p2[j+3] = ~p2[j+2];

        p2[j+4] = ~(p2[j+3] << 4);
        p2[j+5] = ~p2[j+4];

        p2[j+6] = ~(p2[j+5] << 4);
        p2[j+7] = ~p2[j+6];
    }

    /* P2 should be the complement of P1 */
    for (k = 0; k < count / 2 ; k = k + 2) 
    {
        CHECK_CTRL_C();
        if ((p1[k] != ~p2[k]) | (p1[k + 1] != ~p2[k + 1]))
        {
            failures = failures + 1;
            printf ("\n[Failure]: \n"
                    "    p1[%2d] 0x%08lX(@0x%08lX)_0x%08lX(@0x%08lX)\n"
                    "    p2[%2d] 0x%08lX(@0x%08lX)_0x%08lX(@0x%08lX)\n"
                    "    xor(0x%08lX_0x%08lX)\n",
                    k, (ulong)p1[k], (ulong)(&p1[k]), (ulong)p1[k+1], (ulong)(&p1[k+1]),
                    k, (ulong)p2[k], (ulong)(&p2[k]), (ulong)p2[k+1], (ulong)(&p2[k+1]),
                    (ulong)(p1[k] ^ p2[k]), (ulong)(p1[k+1] ^ p2[k+1]));
        }
    }

    return (failures);
}

/*-----------------------------*/
/*  write_32bit_data_bus_walk  */
/*-----------------------------*/
/* Create a Walking Ones and Walking Zeros across a 32bit wide DDR memory bus.
 * P1 and P2 must be evicting L2Cache addresses. It requires two cache blocks
 * to write a full iteration of the pattern.
 */
static int32_t
write_32bit_data_bus_walk (DT_UINT pattern, DT_UINT area, DT_UINT max_address)
{
    DT_UINT  mask;
    uint32_t j, k, failures;
    DT_UINT  *p1, *p2;
    DT_UINT  p1addrfix, p2addrfix, count, extent;

    extent = max_address - area;
    count  = (extent / sizeof (DT_UINT)) / 2;
    count  = count - (count % 16);

    if(count <= 16)
    {
        printf("Error: the Tests Requires at least 32 * %zu bytes of space!\n", sizeof(DT_UINT));
        return -1;
    }

    p1addrfix = area & 0xFFFFFFFC;
    p2addrfix = (area + (extent / 2)) & 0xFFFFFFFC;

    /* Fill both halves of the memory area.
     */
    p1 = (DT_UINT *) p1addrfix;
    p2 = (DT_UINT *) p2addrfix;

    failures      = 0;

    /* Duplicate the pattern to put two copies of the 32-bit pattern into a
     * single 64-bit word. The upper copy is shifted by one bit to form the
     * successor of the lower copy.
     */
    mask = pattern;

    for (j = 0; j < count; j = j + 16) 
    {
        /* Form Entry -- walk one's */
        p1[j   ] =  mask;
        p1[j+ 1] = ~p1[j];

        p1[j+ 2] =  p1[j] << 2;
        p1[j+ 3] = ~p1[j+2];

        p1[j+ 4] =  p1[j+2] << 2;
        p1[j+ 5] = ~p1[j+4];

        p1[j+ 6] =  p1[j+4] << 2;
        p1[j+ 7] = ~p1[j+6];

        p1[j+ 8] =  p1[j+6] << 2;
        p1[j+ 9] = ~p1[j+8];

        p1[j+10] =  p1[j+8] << 2;
        p1[j+11] = ~p1[j+10];

        p1[j+12] =  p1[j+10] << 2;
        p1[j+13] = ~p1[j+12];

        p1[j+14] =  p1[j+12] << 2;
        p1[j+15] = ~p1[j+14];

        /* Form entry -- walk 0's */
        p2[j   ] = ~mask;
        p2[j+ 1] = ~p2[j];

        p2[j+ 2] = ~(p2[j+1] << 2);
        p2[j+ 3] = ~p2[j+2];

        p2[j+ 4] = ~(p2[j+3] << 2);
        p2[j+ 5] = ~p2[j+4];

        p2[j+ 6] = ~(p2[j+5] << 2);
        p2[j+ 7] = ~p2[j+6];

        p2[j+ 8] = ~(p2[j+7] << 2);
        p2[j+ 9] = ~p2[j+8];

        p2[j+10] = ~(p2[j+9] << 2);
        p2[j+11] = ~p2[j+10];

        p2[j+12] = ~(p2[j+11] << 2);
        p2[j+13] = ~p2[j+12];

        p2[j+14] = ~(p2[j+13] << 2);
        p2[j+15] = ~p2[j+14];
    }

    /* P2 should be the complement of P1 */
    for (k = 0; k < count / 2; k = k + 2) 
    {
        CHECK_CTRL_C();
        if ((p1[k] != ~p2[k]) | (p1[k + 1] != ~p2[k + 1]))
        {
            failures = failures + 1;
            printf ("\n[Failure]: \n"
                    "    p1[%2d] 0x%08lX(@0x%08lX)_0x%08lX(@0x%08lX)\n"
                    "    p2[%2d] 0x%08lX(@0x%08lX)_0x%08lX(@0x%08lX)\n"
                    "    xor(0x%08lX_0x%08lX)\n",
                    k, (ulong)p1[k], (ulong)(&p1[k]), (ulong)p1[k+1], (ulong)(&p1[k+1]),
                    k, (ulong)p2[k], (ulong)(&p2[k]), (ulong)p2[k+1], (ulong)(&p2[k+1]),
                    (ulong)(p1[k] ^ p2[k]), (ulong)(p1[k+1] ^ p2[k+1]));
        }
    }
    

    return (failures);
}

/*-----------------------*/
/*  test_data_bus_walks  */
/*-----------------------*/
/* Apply a walking One's and Zero's pattern to the DDR data bus. A walking
 * one pattern sets just one bit in a word to one, starting with the least
 * significant bit (LSB). On each iteration the one bit appears in the
 * next highest bit position. When the bit appears in the most significant
 * bit (MSB) position, the pattern resets and starts again at the LSB. The
 * complement of a walking one is a walking zero.
 */
int32_t
test_data_bus_walks (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    DT_UINT  mask = 0;
    
    uint32_t i, j, failures, ret_err;
    int bus_width = dt_dramDataBusWidth();
    
    ret_err = 0; 
    failures = 0;
    
    for (j=0; j < (sizeof(dram_databus_walks_pattern)/sizeof(DT_UINT)); j++) 
    {
        CHECK_CTRL_C();
        if(tPat_databus.auto_en == 0)
            mask = tPat_databus.type;
        else
            mask = dram_databus_walks_pattern[j];
            
        printf("\n[data bus walks]\n"
               "  pattern = 0x%08lX\n", (ulong)mask);
        
        for (i = 0; i < bursts; i = i + 1) 
        {
            printf("  Bursts: %3d,  mask_pattern = 0x%08lX ... ", (i + 1), (ulong)mask);
            switch (bus_width) 
            {  
                case  8 :
                        ret_err = write_08bit_data_bus_walk(mask, area, max_address);
                        if(ret_err == -1)
                        {
                            return -1;
                        }
                        
                        if (ret_err == 0) 
                        {
                            printf(" [PASS]\n");
                        }
                        
                        failures += ret_err;
                        break;
                case 16 :
                        ret_err = write_16bit_data_bus_walk(mask, area, max_address);
                        if(ret_err == -1)
                        {
                            return -1;
                        }
                        
                        if (ret_err == 0) 
                        {
                            printf(" [PASS]\n");
                        }
                        
                        failures += ret_err;
                        break;
                case 32 : 
                default :
                        ret_err = write_32bit_data_bus_walk(mask, area, max_address);
                        if(ret_err == -1)
                        {
                            return -1;
                        }
                        
                        if (ret_err == 0) 
                        {
                            printf(" [PASS]\n");
                        }
                        
                        failures += ret_err;
                        break;
            }

            /* Select the next starting value */
            if (mask & 0x00010000L) 
            {
                if(tPat_databus.auto_en == 0)
                    mask = tPat_databus.type;
                else
                    mask = dram_databus_walks_pattern[j];
            } 
            else 
            {
                mask = mask << 16;
            }
        }
    }
    
    return (failures);
}

/*----------------------*/
/*  run_data_bus_tests  */
/*----------------------*/
/* The goal of these tests are to toggle every DDR data pin, one at a time or in related groups,
 * to isolate any short circuits between the data pins or open circuits where the pin is not
 * connected to the DDR memory. A board which fails one of these tests has severe problems and
 * will not be able to run any of the later test patterns.
 */
int32_t
run_data_bus_tests (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t    failures, ret_err;
    
    ret_err = 0; 
    failures = 0;

    /* Conduct a simple test to see if basic write read works. */
    ret_err = test_data_bus_burst (area, max_address, bursts);
    if(ret_err == -1)
    {
        return -1;
    }
    else
    {
        if(ret_err > 0)
            printf ("Test failed: Stuck Data line\n");
        failures += ret_err;
    }

    /* Look for data bus failures with 1, 2 or 3 line drivers enabled at
     * one time (walking ones/zeros test).
     */
    ret_err = test_data_bus_walks (area, max_address, bursts);
    if(ret_err == -1)
    {
        return -1;
    }
    else
    {
        if(ret_err > 0)
            printf ("Test failed: Stuck Data line\n");
        failures += ret_err;
    }
    
    return (failures);
}


/*----------------------*/
/* do_dram_databus_test */
/*----------------------*/
/* dram databus [count [[start_addr end_addr] [pattern]]] */
int do_dram_databus_test (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    
    uint32_t failures = 0, ret_err = 0;
    uint32_t i, map_start, map_end;
    int count_limit;
    
    int run_position = dt_probe_position();
    
    if (argc > 1)
        count_limit = (ulong)simple_strtoul(argv[1], NULL, 10);
    else
        count_limit = 10;
    
    if(argc == 3)
        return CMD_RET_USAGE;
    
    if(argc > 3) 
    {
        map_start = MapUser;
        map_end   = MapUser + 1;
        memoryMap[MapUser].name   = "User Tester";
        memoryMap[MapUser].base   = simple_strtoul(argv[2], NULL, 16);
        memoryMap[MapUser].extent = simple_strtoul(argv[3], NULL, 16) - memoryMap[MapUser].base;
    }
    else
    {
        if((run_position == POS_IN_LOWER_BASE) || 
           (run_position == POS_IN_LOWER_RANGE))
        {
            map_start = MapUpper;
            map_end   = MapUpper + 1;
        }
        else if((run_position == POS_IN_UPPER_BASE) || 
                (run_position == POS_IN_UPPER_RANGE))
        {
            map_start = MapLower;
            map_end   = MapLower + 1;
        }
        else /* POS_IN_NULL_RANGE || POS_IN_BOTH_RANGE */
        {
            map_start = MapUser;
            map_end   = MapUser + 1;
            printf("dramtest should run in Appropriate address.\n"
                   " try to run command: reloc auto.\n\n");
            return 0;
        }
    }

    if (argc > 4)
    {
        tPat_databus.auto_en = 0;
        tPat_databus.type = (ulong)simple_strtoul(argv[4], NULL, 16);
    }
    else
    {
        tPat_databus.auto_en = 1;
        tPat_databus.type = 0x5A5A5A5A;
    }


    printf ("\nDram Databus Test\n\n");

    puts ("Test On Memory Map: \n");
    for(i = map_start; i < map_end; i++)
	{
	    if(memoryMap[i].extent > 0)
	    {
	        printf("%16s : 0x%08lX ~ 0x%08lX(", memoryMap[i].name,
	                   (ulong)memoryMap[i].base,
	                   (ulong)memoryMap[i].base + (ulong)memoryMap[i].extent);
	        print_size((uint32_t)memoryMap[i].extent, ")\n");
	    }
	}
    puts("\n");


    for(i = map_start; i < map_end; i++)
    {
        if(memoryMap[i].extent > 0)
        {   
            ret_err = run_data_bus_tests (memoryMap[i].base, 
                                          memoryMap[i].base + memoryMap[i].extent, count_limit);
            
            if(ret_err == -1)
                return CMD_RET_FAILURE;
            else
                failures += ret_err;
                        
            if(failures == 0) 
                printf("%30s","[PASS]\n");
        }
    }

    return (0);
}

const uint32_t dram_addrbus_patterns[] =   {
        0x00000000,
        0xffffffff,
        0x55555555,
        0xaaaaaaaa,
        0x01234567,
        0x76543210,
        0x89abcdef,
        0xfedcba98,
        0xA5A5A5A5,
        0x5A5A5A5A,
        0xF0F0F0F0,
        0x0F0F0F0F,
        0xFF00FF00,
        0x00FF00FF,
        0x0000FFFF,
        0xFFFF0000,
        0x00FFFF00,
        0xFF0000FF,
        0x5A5AA5A5,
        0xA5A55A5A,
};

/* address bus */
testPatten tPat_addrbus = {1, 0xAAAAAAAA};

/*-----------------------*/
/*  test_addr_bus_walks  */
/*-----------------------*/
int32_t
test_addr_bus_walks (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    volatile DT_UINT  *p1;
    DT_UINT  offset, test_offset, num_words, temp;
    uint32_t i, failures = 0, total_failures = 0;
    DT_UINT  mask_pattern;
    DT_UINT  anti_pattern;
    
    mask_pattern = tPat_addrbus.type;
    anti_pattern = ~mask_pattern;
    
    num_words = (max_address - area) / sizeof(DT_UINT);
    
    p1 =  (DT_UINT *)(area & 0xFFFFFFFC);
    
    for (i = 0; i < bursts; i = i + 1) 
    {
        CHECK_CTRL_C();
        
        failures = 0;
        printf("  Bursts: %3d, pattern = 0x%08lX ... ", i+1, (ulong)mask_pattern);
        
        /*
         * Write the default pattern at each of the
         * power-of-two offsets.
         */
        /* Fill the memory at selected locations with pattern.
         */
        for (offset = 1; offset < num_words; offset <<= 1)
        {
            p1[offset] = mask_pattern;
        }

        /*
         * Check for address bits stuck high.
         */
        test_offset = 0;
        p1[test_offset] = anti_pattern;

        for (offset = 1; offset < num_words; offset <<= 1) 
        {
            CHECK_CTRL_C();
            
            temp = p1[offset];
            if (temp != mask_pattern) {
                printf("\n[Failure]: Address bit stuck high @ 0x%08lX:"
                       " expected 0x%08lX, actual 0x%08lX",
                    (ulong)(area + offset), (ulong)mask_pattern, (ulong)temp);
                failures++;
            }
        }
        
        p1[test_offset] = mask_pattern;

        /*
         * Check for addr bits stuck low or shorted.
         */
        for (test_offset = 1; test_offset < num_words; test_offset <<= 1) 
        {
            p1[test_offset] = anti_pattern;

            for (offset = 1; offset < num_words; offset <<= 1) 
            {
                CHECK_CTRL_C();
                temp = p1[offset];
                if ((temp != mask_pattern) && (offset != test_offset)) {
                    printf("\n[Failure]: Address bit stuck low or"
                           " shorted @ 0x%08lX: expected 0x%08lX, actual 0x%08lX",
                           (ulong)(area + offset), (ulong)mask_pattern, (ulong)temp);
                    failures++;
                }
            }
            p1[test_offset] = mask_pattern;
        }
        
        if(failures == 0)
            printf(" [PASS]");

		printf("\n");
        total_failures += failures;
    }
    
    return (total_failures);
}

/*----------------------*/
/*  run_addr_bus_tests  */
/*----------------------*/
int32_t
run_addr_bus_tests (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{

    uint32_t i, j, failures, ret_err, max_pattern_count;
    
    failures = 0;
    if(tPat_addrbus.auto_en == 1)
        max_pattern_count = ARRAY_SIZE(dram_addrbus_patterns);
    else
        max_pattern_count = 1;
    
    for(j = 0; j < max_pattern_count; j++)
    {
        if(tPat_addrbus.auto_en == 1)
        {
            tPat_addrbus.type = dram_addrbus_patterns[j];
        }
        
        for(i = 0; i < bursts; i = i + 1)
        {
            ret_err = test_addr_bus_walks (area, max_address, bursts); 
            
            if (ret_err == -1)
            {
                return -1;
            }
            else
            {
                if(ret_err > 0)
                    printf ("Test failed: Stuck Address line\n");
                failures += ret_err;
            }
        }
    }

    return (failures);
}

/*----------------------*/
/* do_dram_addrbus_test */
/*----------------------*/
int do_dram_addrbus_test (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    
    uint32_t failures = 0, ret_err = 0;
    uint32_t i, map_start, map_end;
    int count_limit;
    int run_position = dt_probe_position();
    
    if (argc > 1)
        count_limit = (ulong)simple_strtoul(argv[1], NULL, 10);
    else
        count_limit = 10;

    if(argc == 3)
        return CMD_RET_USAGE;
    
    if(argc > 3) 
    {
        map_start = MapUser;
        map_end   = MapUser + 1;
        memoryMap[MapUser].name   = "User Tester";
        memoryMap[MapUser].base   = simple_strtoul(argv[2], NULL, 16);
        memoryMap[MapUser].extent = simple_strtoul(argv[3], NULL, 16) - memoryMap[MapUser].base;
    }
    else
    {
        if((run_position == POS_IN_LOWER_BASE) || 
           (run_position == POS_IN_LOWER_RANGE))
        {
            map_start = MapUpper;
            map_end   = MapUpper + 1;
        }
        else if((run_position == POS_IN_UPPER_BASE) || 
                (run_position == POS_IN_UPPER_RANGE))
        {
            map_start = MapLower;
            map_end   = MapLower + 1;
        }
        else /* POS_IN_NULL_RANGE || POS_IN_BOTH_RANGE */
        {
            map_start = MapUser;
            map_end   = MapUser + 1;
            printf("dramtest should run in Appropriate address.\n"
                   " try to run command: reloc auto.\n\n");
            return 0;
        }
    }

    if (argc > 4)
    {
        tPat_addrbus.auto_en = 0;
        tPat_addrbus.type = (ulong)simple_strtoul(argv[4], NULL, 16);
    }
    else
    {
        tPat_addrbus.auto_en = 1;
        tPat_addrbus.type = 0xAAAAAAAA;
    }
    

    printf ("\nDram Address Bus Test\n\n");
    
    puts ("Test On Memory Map: \n");
    for(i = map_start; i < map_end; i++)
	{
	    if(memoryMap[i].extent > 0)
	    {
	        printf("%16s : 0x%08lX ~ 0x%08lX(", memoryMap[i].name,
	                   (ulong)memoryMap[i].base,
	                   (ulong)memoryMap[i].base + (ulong)memoryMap[i].extent);
	        print_size((uint32_t)memoryMap[i].extent, ")\n");
	    }
	}
    puts("\n");
    
    for(i = map_start; i < map_end; i++)
    {
        if(memoryMap[i].extent > 0)
        {
            ret_err = run_addr_bus_tests (memoryMap[i].base, 
                        memoryMap[i].base + memoryMap[i].extent, count_limit);
                        
            if(ret_err == -1)
                return 1;
            else
                failures += ret_err;
                
            if(failures == 0)
                printf("%30s","[PASS]\n"); 
        }
    }

    return (failures);
}


