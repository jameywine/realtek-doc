/*
 *  Copyright (c) 2014  TP-LINK Co., Ltd
 *
 *  TP-LINK Co., Ltd R&D SMB Product Line
 *
 */

/* rmwtest
 *
 * Dram memory tester which generates a continuous stream of read-
 * modify-write actions against the Dram memory. The test is intended to
 * support measuring various DDR clocks and signals for tuning the DDR write
 * performance.
 */

#include <common.h>
#include <command.h>
#include "dt_common.h"

/*-------------------*/
/*  RMW MEMORY TEST  */
/*-------------------*/
testPatten tPat_rmw = {1, 0xDEADC0DE};

testDesc tests_rmw[] = {
    { "Read-Modify-Write", run_mem_rmw_tests, 0x0001,  512/*bursts*/, 0, 0},
    { NULL, NULL, 0, 0, 0, 0}
};

/*-----------------------------*/
/*  test_mem_fill_pattern      */
/*-----------------------------*/
/* Write a random data pattern to the memory. 
 */
void
test_mem_fill_pattern (DT_UINT area, DT_UINT max_address)
{
    uint32_t    j;
    DT_UINT    *p1, count, extent, pattern;

    pattern = 0;
    extent = max_address - area;
    count  = (extent / sizeof (DT_UINT));

    p1 = (DT_UINT *) area;

	puts("Filling memory ...");
    for (j = 0; j < count; j++) 
    {
        if(tPat_rmw.auto_en == 1)
        {
            /* Get a random pattern for this memory location. Avoid a pattern of
             * zero which would fill memory with a zero pattern.
             */
            do {
               pattern = (DT_UINT )dt_random_generate (0);
            } while (pattern == 0);
        }
        else
        {
            pattern = tPat_rmw.type;
        }
		if(((DT_UINT)p1) % 0x100000 == 0)
			puts(".");
		
        *p1 = pattern;
        p1++;
    }

	puts("\n");
}

/*---------------------*/
/*  run_mem_rmw_tests  */
/*---------------------*/
int32_t
run_mem_rmw_tests (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t    i, j, failures = 0, total_fail = 0;
	uint32_t    area_size, dram_count, dram_size = 0x10000;
    DT_UINT     pattern, mask, new_p1, old_p1;

    volatile DT_UINT *p1;

    pattern    = 0;
    mask       = 0;

	area_size  = (uint32_t)(max_address - area);
	dram_count = area_size / dram_size;

	if(area_size < dram_size)
    {
        printf("end_addr - start_addr should large than 0x10000.\n");
        return 0;
    }

	mask = 0xFFFFFFFF;
	while(mask > dram_count)
	{
		mask = (mask >> 1);
	}

	printf("  Address Mask = 0x%08lX\n", (ulong)mask);

    for (i = 0; i < bursts; i = i + 1) 
    {
        CHECK_CTRL_C();

		failures = 0;
        
        if(tPat_rmw.auto_en == 1)
        {
            pattern = (DT_UINT)dt_random_generate(0);
        }
        else
        {
            pattern = tPat_rmw.type;
        }

        old_p1 = pattern;

        /*
         * pick random addresses.     
         */
		p1 = (DT_UINT *)((DT_UINT)area + \
			             (dt_random_generate(mask)) * dram_size % area_size );

        printf ("  Burst %4d, address = 0x%08lX, init_pattern = 0x%08lX ...", 
                i + 1, (ulong)p1, (ulong)pattern);
        
        /* Write a memory block containing random data. Remember the value 
         * and check to see if memory has that value on a subsequent iteration.
         */
        for (j = 0; j < dram_size / 4; j = j + 1) 
        {
            p1[j] = pattern;
            pattern = pattern ^ (pattern >> 1); // Apply Gray code
        }

        /* Read the first word of the p1 memory block . 
         * If it does not match the expected value, report an error.
         */
        pattern = old_p1;

        for (j = 0; j < dram_size / 4; j = j + 1) 
        {
            new_p1 = p1[j];
            if (new_p1 != pattern) 
			{
                printf ("\n[Failure]: "
					    "0x%08lX: want 0x%08lX  got 0x%08lX  xor 0x%08lX",
                        (ulong)&p1[j], 
                        (ulong)pattern, (ulong)new_p1, 
                        (ulong)(pattern ^ new_p1));

				failures++;
            }
            pattern = pattern ^ (pattern >> 1); // Apply Gray code
        }

		if(failures == 0)
			printf("  [PASS]");

		printf("\n");

		total_fail += failures;
    }
	
    return total_fail;
}

  /*--------------*/
  /*  run_tests   */
  /*--------------*/
int32_t
rmw_tests (dramMemRange *map, uint32_t map_start, uint32_t map_end, uint32_t passes)
{
    int       i, j;
    uint32_t  failures, pass, ret_err = 0;

    failures = 0;

    for (pass = 1; pass <= passes; pass++) 
    {
        printf ("\nPass %d\n", pass);
        for (i = 0; ; i++) 
        {
            if (!tests_rmw[i].name)
                break;

            for (j = map_start; j < map_end; j++) 
            {
                if (!map[j].name)
                    break;
                if (map[j].extent > 0) 
                {
                    printf ("%s over (0x%08lX..%08lX)\n",
                            tests_rmw[i].name, 
                            (ulong)map[j].base,
                            (ulong)map[j].base + (ulong)map[j].extent);
                    
                    ret_err = tests_rmw[i].fp(map[j].base,
                                              map[j].base + map[j].extent,
                                              tests_rmw[i].bursts);
                    if(ret_err == -1)
                    {
						return -1;
                    }
                    else
                    {
                        tests_rmw[i].failures += ret_err;
                    }

                    if (tests_rmw[i].failures)
                    {
                        printf ("Test failed: %s\n", tests_rmw[i].name);
                        tests_rmw[i].total_failures =
                            tests_rmw[i].total_failures + tests_rmw[i].failures;
                    }
                }
            }
        }

        /* Display the test summary information. 
         */
        printf ("Pass %d Summary\n", pass);
        for (i = 0; ; i = i + 1) 
        {
            if (!tests_rmw[i].name)
                break;
            printf ("%30s: fail(%12u)  total_fail(%12u)\n",
                             tests_rmw[i].name,
                             tests_rmw[i].failures,
                             tests_rmw[i].total_failures);
            failures = failures + tests_rmw[i].failures;
        }
    }

    return (failures);
}

/*-------------------*/
/*  do_dram_rwm_test  */
/*-------------------*/
int do_dram_rwm_test (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint32_t   i, map_start, map_end, count_limit, ret_err = 0;
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
        tPat_rmw.auto_en = 0;
        tPat_rmw.type = (ulong)simple_strtoul(argv[4], NULL, 16);
    }
    else
    {
        tPat_rmw.auto_en = 1;
        tPat_rmw.type = 0xDEADC0DE;
    }

    printf ("\nDram RMW Test\n\n");
        
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

    /* Fill all three memory regions with a random data pattern. These should
     * be the only memory writes in the test.
     */
    for (i = map_start; i < map_end; i = i + 1) 
    {
        if (memoryMap[i].extent == 0)
            continue;
    
        printf ("Filling memory(0x%08lX..%08lX) with initial random pattern.\n",
                (ulong)memoryMap[i].base,
                (ulong)memoryMap[i].base + (ulong)memoryMap[i].extent);
        test_mem_fill_pattern (memoryMap[i].base,
                            memoryMap[i].base + memoryMap[i].extent);
    }

    /* Run the memory tests over the three regions. */
    ret_err = rmw_tests (memoryMap, map_start, map_end, count_limit);
    if(ret_err == -1)
        return CMD_RET_FAILURE;

    return (0);
}

/*--------------------*/
/*  FADE MEMORY TEST  */
/*--------------------*/

#define oneMinute  1

testPatten tPat_fade = {1, 0xFFFFFFFF};
/*------------------------*/
/*  test_mem_alt_pattern  */
/*------------------------*/
static uint32_t
test_mem_fade_pattern (DT_UINT area, DT_UINT max_address, uint32_t passes)
{
    uint32_t i, failures;
    DT_UINT  test_pattern;
    volatile DT_UINT *p1;
    
    test_pattern = tPat_fade.type;
    failures = 0;

    for (i = 0; i < passes; i = i + 1) 
    {
        if (i & 0x1)
            test_pattern = ~test_pattern;

        /* Filling the memory */
        for (p1 = (DT_UINT *) area; p1 < (DT_UINT *) max_address; p1++) 
        {
            *p1 = test_pattern;
        }

        /* Read the written memory and confirm that it has the expected
         * data pattern.
         */
        for (p1 = (DT_UINT *) area; p1 < (DT_UINT *) max_address; p1++) 
        {
            if (*p1 != test_pattern) 
            {
                printf ("[Failure][%d]: 0x%08lX: want 0x%08lX  got 0x%08lX xor 0x%08lX\n",
                        i, (ulong) p1, (ulong)test_pattern, (ulong)*p1, (ulong)(test_pattern ^ (*p1)));
                failures++;
            }
        }
    }
    return (failures);
}

/*---------------------*/
/*  check_mem_pattern  */
/*---------------------*/
static uint32_t
check_mem_pattern (DT_UINT area, DT_UINT max_address, DT_UINT pattern)
{
    uint32_t    failures;
    DT_UINT    data, *p1;

    failures = 0;
    for (p1 = (DT_UINT *) area; p1 < (DT_UINT *) max_address; p1++) 
    {
        data = *p1;
        if (data != pattern) 
        {
            printf ("[Failure]: 0x%08lX: want 0x%08lX  got 0x%08lX xor 0x%08lX\n",
                   (ulong) p1, (ulong)pattern, (ulong)data, (ulong)(pattern ^ data));
            
            failures++;
        }
    }
    return (failures);
}

/*-----------------*/
/*  test_mem_fade  */
/*-----------------*/
static int
test_mem_fade (dramMemRange *map, uint32_t map_start, uint32_t map_end, uint32_t delay_minutes)
{
    uint32_t    i, failures;

    failures = 0;

      /* Fill all of memory with solid ones. */
    for (i = map_start; i < map_end; i = i + 1) 
    {
        if(map[i].extent == 0)
            continue;
        
        printf ("  Filling memory: 0x%08lX..0x%08lX\n",
                        (ulong)map[i].base, 
                        (ulong)(map[i].base + map[i].extent));
                        
        failures = test_mem_fade_pattern (map[i].base, map[i].base + map[i].extent, 1);
        
        if (failures)
            return (failures);
    }

    /* Wait for 'delay' cycles. */
    printf ("  Waiting ");
    if( dt_minutes_delay (delay_minutes) == -1/*Interrupted by user*/)
        return -1;

    /* Read memory and see if any locations changed. */
    printf ("\n  Checking memory\n");
    for (i = map_start; i < map_end; i = i + 1) 
    {
        failures = check_mem_pattern (map[i].base, map[i].base + map[i].extent, tPat_fade.type);
        
        if (failures)
            return (failures);
    }
            
    return (failures);
}



/*----------------------*/
/*  run_mem_fade_tests  */
/*----------------------*/
int32_t
run_mem_fade_tests (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{

    uint32_t i, failures, ret_err;
    uint32_t delay_minutes = 90;
    
    /* pattern init */
    tPat_fade.auto_en = 1;
    tPat_fade.type = 0xFFFFFFFF;
    
    failures = 0;
    
    for(i = 0; i < bursts; i = i + 1)
    {
        /* Fill all of memory with solid ones. */
        printf ("  Filling memory: 0x%08lX..0x%08lX\n", (ulong)area, (ulong)(max_address));
        ret_err = test_mem_fade_pattern (area, max_address, 1);
            
        if (ret_err != 0)
            return (ret_err);

        /* Wait for 'delay' cycles. */
        printf ("  Waiting ");
        if( dt_minutes_delay (delay_minutes) == -1/*Interrupted by user*/)
            return -1;

        /* Read memory and see if any locations changed. */
        printf ("\n  Checking memory\n");

        failures += check_mem_pattern (area, max_address, tPat_fade.type);
    }
            
    return (failures);
}

/*--------*/
/*  main  */
/*--------*/
int do_dram_fade_test (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint32_t   failures, pass, total_failures;
    uint32_t   times, count_limit;
    uint32_t   i, map_start, map_end;
    
    int      run_position = dt_probe_position();
    
    if (argc > 1) {
        times   = simple_strtoul(argv[1], NULL, 10);
    } else {
        times   = 90;
    }
    
    if (argc > 2) {
        count_limit   = simple_strtoul(argv[2], NULL, 10);
    } else {
        count_limit   = 1;
    }
    
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
        tPat_fade.auto_en = 0;
        tPat_fade.type = (ulong)simple_strtoul(argv[5], NULL, 16);
    }
    else
    {
        tPat_fade.auto_en = 1;
        tPat_fade.type = 0xFFFFFFFF;
    }

    printf ("\nDram Memory Fade Test\n\n");

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
    
    failures = 0;
    total_failures = 0;
    
    printf ("Testing %02d minutes\n\n", times);
    
    /* Run the memory fade test, which takes a very long time to
     * execute.
     */
    for (pass = 0; pass < count_limit; pass = pass + 1)
    {
        printf ("Pass%d\nMemory Fade\n", pass + 1);
        
        failures = test_mem_fade (memoryMap, map_start, map_end, times * oneMinute);
        if(failures == -1) /* Interrupted by user */
            return CMD_RET_FAILURE;

        printf ("Pass %d Summary\n", pass + 1);
        printf ("%30s: fail(%12u)  total_fail(%12u)\n",
                "Memory Fade",
                failures,
                total_failures);

        total_failures = total_failures + failures;
    }

    return (failures);
}