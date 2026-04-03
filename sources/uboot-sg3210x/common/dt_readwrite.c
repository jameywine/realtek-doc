/*
 *  Copyright (c) 2014  TP-LINK Co., Ltd
 *
 *  TP-LINK Co., Ltd R&D SMB Product Line
 *
 */

/* readtest
 *
 * dramtest tester which reads a continuous stream of random data
 * from the memory. The test is intended to support measuring various DRAM
 * clocks and signals for tuning the DRAM write performance.
 */

#include <common.h>
#include <command.h>
#include "dt_common.h"

testPatten tPat_read = {1, 0x5A5A5A5A};

/*-----------------------*/
/*  test_mem_fill_rand  */
/*-----------------------*/
/* Write a random data pattern to each location in the memory block. This
 * only happens once for each memory area.
 */

void
test_mem_fill_rand_data (DT_UINT area, DT_UINT max_address)
{
    uint32_t    j;
    DT_UINT    *p1, count, extent, pattern;
	
    pattern = 0;
    extent = max_address - area;
    count  = (extent / sizeof (DT_UINT));

    p1 = (DT_UINT *) area;

    /* Get a random pattern for this burst. Avoid a pattern of zero which
     * would fill memory with a zero pattern.
     */
    do 
    {
        pattern = dt_random_generate(0);
    } while (pattern == 0);
	
    /* Multiply the memory index (j) times the pattern to form a random
     * value and write that pattern to the memory location. Thus each
     * write should have a differing bit pattern.
     */
    
    for (j = 0; j < count; j++) 
    {
        //*p1 = pattern * j;
        *p1 = pattern;
        p1++;	
    }
}

/*-----------------------*/
/*  test_mem_fill_pattern  */
/*-----------------------*/
/* Write a  data pattern to each location in the memory block. This
 * only happens once for each memory area.
 */
void
test_mem_fill_pattern_data (DT_UINT area, DT_UINT max_address, DT_UINT pattern)
{
    uint32_t    j;
    DT_UINT    *p1, count, extent;

    extent = max_address - area;
    count  = (extent / sizeof (DT_UINT));

    p1 = (DT_UINT *) area;

    for (j = 0; j < count; j++) 
    {
        *p1 = pattern;
        p1++;
    }
}

/*-----------------------*/
/*  test_mem_read_rand  */
/*-----------------------*/
/* Read the previously written random data pattern from each location
 * in the memory block. This routine only performs a read; it never writes.
 */
int32_t
test_mem_read_data_line (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t    i, j;
    DT_UINT     count, extent, data;

    volatile DT_UINT   *p1;

    extent = max_address - area;
    count  = (extent / sizeof (DT_UINT));

    for (i = 0; i < bursts; i = i + 1) 
    {
        
        /* check for Interrupted.*/
        CHECK_CTRL_C(); 
        printf("    Bursts: %4d, Reading...   ", i + 1);
        
        p1 = (DT_UINT *) area;
        for (j = 0; j < count; j++) 
        {
            
            data = *p1++;
            debug("Read data = %ld", (ulong)data);
        }
        
        printf(" OK\n");
    }

    return (0);
}

/*----------------------------*/
/*  test_mem_read_addr_line   */
/*----------------------------*/
int32_t
test_mem_read_addr_line (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t    i, j;
    DT_UINT     count, extent, data, test_pat, mask_pat, anti_pat;

    volatile DT_UINT   *p1;

    extent = max_address - area;
    count  = (extent / sizeof (DT_UINT));
    
    if(tPat_read.auto_en == 1)
    {
        do 
        {
            test_pat = dt_random_generate(0);
        } while (test_pat == 0);
    }
    else
    {
        test_pat = tPat_read.type;
    }
    
    mask_pat = test_pat & 0xFFFFFFFC;
    mask_pat = mask_pat & (max_address - area - 1);
    anti_pat = (~test_pat) & 0xFFFFFFFC;
    anti_pat = anti_pat & (max_address - area - 1);

    for (i = 0; i < bursts; i = i + 1) 
    {
        
        /* check for Interrupted.*/
        CHECK_CTRL_C(); 
        printf("    Bursts: %4d, addr_pattern = 0x%08lX, anti_pattern = 0x%08lX, Reading...   ", 
               i + 1, (ulong)(mask_pat + area), (ulong)(anti_pat + area));
        
        for (j = 0; j < count; j++) 
        {
            if(j&1)
                p1 = (DT_UINT *)(mask_pat + area);
            else
                p1 = (DT_UINT *)(anti_pat + area);
            
            data = *p1;
            debug("Read data = %ld", (ulong)data);
        }
        
        printf(" OK\n");
    }

    return (0);
}

/*--------------------*/
/* do_dram_read_test  */
/*--------------------*/
/* dram read count_limit type base_addr end_addr pattern */
int do_dram_read_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint32_t i, j, map_start, map_end, count_limit, test_type;
    int      run_position = dt_probe_position();
    
    if (argc > 1)
        count_limit = (ulong)simple_strtoul(argv[1], NULL, 10);
    else
        count_limit = 10; 
        
    if (argc > 2)
        test_type = (ulong)simple_strtoul(argv[2], NULL, 10); /* 地址线上的读取测试 */
    else
        test_type = 0;  /* 数据线上的读取测试 */
    
    if(argc == 4)
        return CMD_RET_USAGE;
    
    if(argc > 4) 
    {
        map_start = MapUser;
        map_end   = MapUser + 1;
        memoryMap[MapUser].name   = "User Tester";
        memoryMap[MapUser].base   = simple_strtoul(argv[3], NULL, 16);
        memoryMap[MapUser].extent = simple_strtoul(argv[4], NULL, 16) - memoryMap[MapUser].base;
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

    if (argc > 5)
    {
        tPat_read.auto_en = 0;
        tPat_read.type = (ulong)simple_strtoul(argv[5], NULL, 16);
    }
    else
    {
        tPat_read.auto_en = 1;
        tPat_read.type = 0x55555555;
    }

    printf ("\nDram Memory Read Test\n\n");
    
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

    /* Fill all memory regions with a random data pattern. 
     * These should be the only memory writes in the test.
     */
    printf("Fill all memory regions with a random data pattern.\n");
    for (i = map_start; i < map_end; i = i + 1) 
    {
        if(memoryMap[i].extent > 0)
        {
            if((test_type == 0) && (tPat_read.auto_en == 0))
            {
                test_mem_fill_pattern_data(memoryMap[i].base,
                            memoryMap[i].base + memoryMap[i].extent, tPat_read.type);
            }
            else
            {
                test_mem_fill_rand_data(memoryMap[i].base,
                                memoryMap[i].base + memoryMap[i].extent);
            }
        }
    }
    /* Run the memory read tests over the three regions. */
    /* read_tests (memoryMap, map_start, map_end, count_limit); */
    uint32_t  failures, pass, ret_err;

    failures = 0;

    for (pass = 1; pass <= count_limit; pass++) 
    {
        printf ("\nCount:%4d\n", pass);

        for (j = map_start; j < map_end; j++) 
        {
            if (memoryMap[j].extent > 0) 
            {
                if(test_type == 1)
                {
                    ret_err = test_mem_read_addr_line (memoryMap[j].base,
                                             memoryMap[j].base + memoryMap[j].extent, 1);
                }
                else
                {
                    ret_err = test_mem_read_data_line (memoryMap[j].base,
                                             memoryMap[j].base + memoryMap[j].extent, 1);
                }
                                          
                if (ret_err == -1) 
                {
                    return CMD_RET_FAILURE;/* Interrupted by user. */
                }
                else if(ret_err > 0)
                {
                    failures = failures + ret_err;
                    printf ("Test failed: %d\n", failures);
                }
            }
        }

        if(failures == 0)
            printf("%30s\n", "PASS");
    }
    
    return (0);
}

/*----------------*/
/*  Write Tester  */
/*----------------*/
testPatten tPat_write = {1, 0x55555555};

/*-----------------------*/
/*  test_mem_write_rand  */
/*-----------------------*/
/* Write a random data pattern to each location in the memory block. This
 * routine only performs a write; it never reads.
 */
int32_t
test_mem_write_data_line (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t    i, j = 0; 
    DT_UINT    *p1, count, extent, pattern;

    pattern = 0;
    extent = max_address - area;
    count  = (extent / sizeof (DT_UINT));

    for (i = 0; i < bursts; i = i + 1) 
    {
        /* check for Interrupted.*/
        CHECK_CTRL_C();
        printf("    Bursts:%4d, Writing...   ", i + 1);
        
        p1 = (DT_UINT *) area;
        
        if(tPat_write.auto_en == 1)
        {
            /* Get a new random pattern for this burst. Avoid a pattern of zero
             * which would cause the test's memory write to only write zeros to
             * all of memory.
             */
            do {
                pattern = dt_random_generate(0);
            } while (pattern == 0);
        }
        else
        {
            pattern = tPat_write.type;
        }
        
        for (j = 0; j < count; j++) 
        {    
            *p1 = pattern;
            p1++;
        }
        printf(" OK\n");
    }

    return (0);
}

int32_t
test_mem_write_addr_line (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t    i, j;
    DT_UINT     count, extent, test_pat, mask_pat, anti_pat;

    volatile DT_UINT   *p1;

    extent = max_address - area;
    count  = (extent / sizeof (DT_UINT));
    
    if(tPat_write.auto_en == 1)
    {
        do 
        {
            test_pat = dt_random_generate(0);
        } while (test_pat == 0);
    }
    else
    {
        test_pat = tPat_write.type;
    }
    
    mask_pat = test_pat & 0xFFFFFFFC;
    mask_pat = mask_pat & (max_address - area - 1);
    anti_pat = (~test_pat) & 0xFFFFFFFC;
    anti_pat = anti_pat & (max_address - area - 1);

    for (i = 0; i < bursts; i = i + 1) 
    {
        /* check for Interrupted.*/
        CHECK_CTRL_C(); 
        printf("    Bursts:%4d,  addr_pattern = 0x%08lX, anti_pattern = 0x%08lX, Writing...   ", 
               i + 1, (ulong)(mask_pat + area), (ulong)(anti_pat + area));
        
        for (j = 0; j < count; j++) 
        {
            if(j&1)
                p1 = (DT_UINT *)(mask_pat + area);
            else
                p1 = (DT_UINT *)(anti_pat + area);
            
            *p1 = (DT_UINT)p1;  /* write address data to address */
        }
        
        printf(" OK\n");
    }

    return (0);
}

/*---------------------*/
/*  do_dram_write_test  */
/*---------------------*/
int do_dram_write_test (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint32_t i, j, map_start, map_end, count_limit, test_type;
    int      run_position = dt_probe_position();
    
    if (argc > 1)
        count_limit = (ulong)simple_strtoul(argv[1], NULL, 10);
    else
        count_limit = 10; 
        
    if (argc > 2)
        test_type = (ulong)simple_strtoul(argv[2], NULL, 10); /* 地址线上的读取测试 */
    else
        test_type = 0;  /* 数据线上的读取测试 */
    
    if(argc == 4)
        return CMD_RET_USAGE;
    
    if(argc > 4) 
    {
        map_start = MapUser;
        map_end   = MapUser + 1;
        memoryMap[MapUser].name   = "User Tester";
        memoryMap[MapUser].base   = simple_strtoul(argv[3], NULL, 16);
        memoryMap[MapUser].extent = simple_strtoul(argv[4], NULL, 16) - memoryMap[MapUser].base;
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

    if (argc > 5)
    {
        tPat_write.auto_en = 0;
        tPat_write.type = (ulong)simple_strtoul(argv[5], NULL, 16);
    }
    else
    {
        tPat_write.auto_en = 1;
        tPat_write.type = 0x55555555;
    }

    printf ("\nDram Memory Write Test\n\n");

    puts ("Test On Memory Map: \n");
    for(i = map_start; i < map_end; i++)
    {
        if(memoryMap[i].extent > 0){
            printf("[%02d]%12s : 0x%08x ~ 0x%08x(", i, memoryMap[i].name,
                   (uint32_t)memoryMap[i].base,
                   (uint32_t)memoryMap[i].base + (uint32_t)memoryMap[i].extent);
            print_size(memoryMap[i].extent, ")\n");
        }
    }
    puts("\n");

    /* write_tests (memoryMap, map_start, map_end, count_limit); */
    uint32_t  failures, pass, ret_err;

    failures = 0;

    for (pass = 1; pass <= count_limit; pass++) 
    {
        printf ("\nCount:%4d\n", pass);

        for (j = map_start; j < map_end; j++) 
        {
            if (memoryMap[j].extent > 0) 
            {
                if(test_type == 1)
                {
                    ret_err = test_mem_write_addr_line (memoryMap[j].base,
                                             memoryMap[j].base + memoryMap[j].extent, 1);
                }
                else
                {
                    ret_err = test_mem_write_data_line (memoryMap[j].base,
                                             memoryMap[j].base + memoryMap[j].extent, 1);
                }
                                          
                if (ret_err == -1) 
                {
                    return CMD_RET_FAILURE;/* Interrupted by user. */
                }
                else if(ret_err > 0)
                {
                    failures = failures + ret_err;
                    printf ("Test failed: %d\n", failures);
                }
            }
        }

        if(failures == 0)
            printf("%30s\n", "PASS");
    }
    
    return (0);
}


testPatten tPat_readwrite = {1, 0x5A5A5A5A};

const DT_UINT dram_readwrite_patterns[] =    {
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

extern uint32_t retry_failure (DT_UINT burst, volatile DT_UINT *p1, 
                               DT_UINT data, DT_UINT expected, uint32_t failures);

/*------------------------------*/
/*  test_dram_readwrite_pattern */
/*------------------------------*/
int32_t
test_dram_readwrite_data_line_pattern (DT_UINT area, DT_UINT max_address, DT_UINT pattern,
                   uint32_t passes)
{
    uint32_t i, failures;
    DT_UINT  data;
    volatile DT_UINT *p1;

    failures = 0;

    for (i = 0; i < passes; i = i + 1) 
    {
        /* check for Interrupted.*/
        CHECK_CTRL_C();
        printf("    Bursts: %4d  Writing... ", i + 1);
        
        /* write pattern*/
        for (p1 = (DT_UINT *) area; p1 < (DT_UINT *) max_address; p1++) 
        {
                
            *p1 = pattern;
        }
        
        printf("  Reading...  ");
        /* Read the written memory and confirm that it has the expected
         * data pattern.
         */
        for (p1 = (DT_UINT *) area; p1 < (DT_UINT *) max_address; p1++) 
        {
            
            data = *p1;
            if (data != pattern) 
            {
                failures = retry_failure (i, p1, data, pattern, failures);
            }
        }
        
        if(failures == 0)
            printf(" [PASS]\n");
    }
    
    return (failures);
}

/*------------------------*/
/*  dram_readwrite_tests  */
/*------------------------*/
int32_t
dram_readwrite_data_line_tests(DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t i, j, failures, ret_err = 0;

    failures = 0;
    for (j = 0; j < bursts; j = j + 1) 
    {
        if(tPat_readwrite.auto_en == 1)
        {
            for (i=0; i < (sizeof(dram_readwrite_patterns)/sizeof(DT_UINT)); i++) 
            {   
                ret_err = test_dram_readwrite_data_line_pattern (area, max_address, 
                                                     dram_readwrite_patterns[i], 1);
                if(ret_err == -1)
                    return -1;
                else
                    failures += ret_err;
            }
        }
        else
        {
            ret_err = test_dram_readwrite_data_line_pattern (area, max_address, 
                                                    tPat_readwrite.type, 1);
            if(ret_err == -1)
                return -1;
            else
                failures += ret_err;
        }
    }
    return (failures);
}

/*------------------------------*/
/*  test_dram_readwrite_pattern */
/*------------------------------*/
int32_t
test_dram_readwrite_addr_line_pattern (DT_UINT area, DT_UINT max_address, DT_UINT pattern,
                   uint32_t passes)
{
    uint32_t    i, failures;
    DT_UINT     test_pat, mask_pat, anti_pat;
    volatile DT_UINT *p1, *p2;

    failures = 0;
    
    if(tPat_readwrite.auto_en == 1)
    {
        do 
        {
            test_pat = dt_random_generate(0);
        } while (test_pat == 0);
    }
    else
    {
        test_pat = tPat_readwrite.type;
    }
    
    mask_pat = test_pat & 0xFFFFFFFC;
    mask_pat = mask_pat & (max_address - area - 1);
    anti_pat = (~test_pat) & 0xFFFFFFFC;
    anti_pat = anti_pat & (max_address - area - 1);
    
    p1 = (DT_UINT *)(mask_pat + area);
    p2 = (DT_UINT *)(anti_pat + area);

    for (i = 0; i < passes; i = i + 1) 
    {
        /* check for Interrupted.*/
        CHECK_CTRL_C();
        printf("    Bursts: %4d, addr_pattern = 0x%08lX(0x%08lX), Writing(0x%08lX)...   ", 
               i + 1, (ulong)(mask_pat + area), (ulong)(anti_pat + area), (ulong)pattern);
        
        /* write pattern*/
        *p1 = pattern;
        *p2 = ~pattern;
        
        /* read pattern*/
        printf("Reading...  ");
        if(*p2 != ~(*p1))
        {
            failures++;
            printf(" [Failure] test_pattern = 0x%08lX\n", (ulong)pattern);
        }
        else
        {
            printf(" [PASS]\n");
        }
    }
    
    return (failures);
}

/*------------------------*/
/*  dram_readwrite_tests  */
/*------------------------*/
int32_t
dram_readwrite_addr_line_tests(DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t i, j, failures, ret_err = 0;

    failures = 0;
    for (j = 0; j < bursts; j = j + 1) 
    {
        if(tPat_readwrite.auto_en == 1)
        {
            for (i=0; i < (sizeof(dram_readwrite_patterns)/sizeof(DT_UINT)); i++) 
            {   
                ret_err = test_dram_readwrite_addr_line_pattern (area, max_address, 
                                                     dram_readwrite_patterns[i], 1);
                if(ret_err == -1)
                    return -1;
                else
                    failures += ret_err;
            }
        }
        else
        {
            ret_err = test_dram_readwrite_addr_line_pattern (area, max_address, 
                                                    tPat_readwrite.type, 1);
            if(ret_err == -1)
                return -1;
            else
                failures += ret_err;
        }
    }
    return (failures);
}

/*---------------------*/
/*  do_dram_rw_test    */
/*---------------------*/
int do_dram_rw_test (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint32_t i, j, map_start, map_end, count_limit, test_type;
    int      run_position = dt_probe_position();
    
    if (argc > 1)
        count_limit = (ulong)simple_strtoul(argv[1], NULL, 10);
    else
        count_limit = 10; 
        
    if (argc > 2)
        test_type = (ulong)simple_strtoul(argv[2], NULL, 10); /* 地址线上的读取测试 */
    else
        test_type = 0;  /* 数据线上的读取测试 */
    
    if(argc == 4)
        return CMD_RET_USAGE;
    
    if(argc > 4) 
    {
        map_start = MapUser;
        map_end   = MapUser + 1;
        memoryMap[MapUser].name   = "User Tester";
        memoryMap[MapUser].base   = simple_strtoul(argv[3], NULL, 16);
        memoryMap[MapUser].extent = simple_strtoul(argv[4], NULL, 16) - memoryMap[MapUser].base;
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

    if (argc > 5)
    {
        tPat_readwrite.auto_en = 0;
        tPat_readwrite.type = (ulong)simple_strtoul(argv[5], NULL, 16);
    }
    else
    {
        tPat_readwrite.auto_en = 1;
        tPat_readwrite.type = 0x55555555;
    } 

    printf ("\nDram Memory ReadWrite Test\n\n");

    puts ("Test On Memory Map: \n");
    for(i = map_start; i < map_end; i++)
    {
        if(memoryMap[i].extent > 0){
            printf("[%02d]%12s : 0x%08x ~ 0x%08x(", i, memoryMap[i].name,
                   (uint32_t)memoryMap[i].base,
                   (uint32_t)memoryMap[i].base + (uint32_t)memoryMap[i].extent);
            print_size(memoryMap[i].extent, ")\n");
        }
    }
    puts("\n");
    
    uint32_t  failures, pass, ret_err;

    failures = 0;

    for (pass = 1; pass <= count_limit; pass++) 
    {
        printf ("\nCount:%4d\n", pass);

        for (j = map_start; j < map_end; j++) 
        {
            if (memoryMap[j].extent > 0) 
            {
                if(test_type == 1)
                {
                    ret_err = dram_readwrite_addr_line_tests (memoryMap[j].base,
                                             memoryMap[j].base + memoryMap[j].extent, 1);
                }
                else
                {
                    ret_err = dram_readwrite_data_line_tests (memoryMap[j].base,
                                             memoryMap[j].base + memoryMap[j].extent, 1);
                }
                                          
                if (ret_err == -1) 
                {
                    return CMD_RET_FAILURE;/* Interrupted by user. */
                }
                else if(ret_err > 0)
                {
                    failures = failures + ret_err;
                    printf ("Test failed: %d\n", failures);
                }
            }
        }

        if(failures == 0)
            printf("%30s\n", "PASS");
    }
    return (0);
}


