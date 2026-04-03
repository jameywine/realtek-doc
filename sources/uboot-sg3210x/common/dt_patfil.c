/*
 *  Copyright (c) 2014  TP-LINK Co., Ltd
 *
 *  TP-LINK Co., Ltd R&D SMB Product Line
 *
 */

#include <common.h>
#include <command.h>
#include "dt_common.h"

const uint32_t dram_test_patterns[] =   {
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
//#undef debug
//#define debug printf

/*-----------------*/
/*  retry_failure  */
/*-----------------*/
/* Report the circumstances of a failure and try re-reading the memory
 * location to see if the error is transient or permanent.
 */
uint32_t
retry_failure (DT_UINT burst,
               volatile DT_UINT *p1, DT_UINT data, DT_UINT expected,
               uint32_t failures)
{
    uint32_t   i, refail, repass;

    failures++;

    if (failures < ERROR_LIMIT) 
    {
        printf ("\n");
        printf ("[Failure][%u]: 0x%08lX: want 0x%08lX  got 0x%08lX  xor 0x%08lX\n",
                (uint32_t)burst+1, (ulong)p1, (ulong)expected, (ulong)data, (ulong)(expected ^ data));

        repass = 0;
        refail = 0;

        /* Try re-reading the memory location. A transient error may fail on
         * on one read and work on another. Keep on retrying even when a 
         * read succeeds.
         */
        for (i = 0; i < RETRY_LIMIT; i = i + 1) 
        {
            data = *p1;
            if ((data != expected)) 
            {
                printf ("  ReRead[%3d] Fail: 0x%08lX: want 0x%08lX  got 0x%08lX  xor 0x%08lX\n",
                        i+1, (ulong)p1, (ulong)expected, (ulong)data, (ulong)(expected ^ data));
                refail = refail + 1;
            } 
            else 
            {
                repass = repass + 1;
            }
        }
        
        printf ("  Reread %d times: Passed: %d, Failed %d\n",
                i, repass, refail);
    } 
    else if (failures == ERROR_LIMIT) 
    {
        printf ("Too many errors, stopping printing\n");
    }
    
    return (failures);
}

/*----------------------*/
/*  test_mem_self_addr  */
/*----------------------*/
/* Fill an memory area.
 * Reread to confirm the pattern.
 */
int32_t
test_mem_self_addr (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t i, failures, probes, total_fail;
	uint32_t area_size, dram_count, dram_size = 0x10000;
    DT_UINT  data, end, mask;
    volatile DT_UINT *p1;
	
    failures   = 0;
	total_fail = 0;

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

	probes = area_size / (sizeof(DT_UINT)) / (dram_size / 2048);

    for (i = 0; i < bursts; i = i + 1) 
    {
        CHECK_CTRL_C();
		failures   = 0;
        
        printf("  Bursts: %4d\n", i + 1);
        
        /* Write the pattern to memory. Each location recieves the address
         * of the location. 
         */
        printf("  Write self_address pattern... \n");
        for (p1 = (DT_UINT *) area; p1 < (DT_UINT *) max_address; p1++) 
        {
            *p1 = (DT_UINT) p1;
        }
        
        /* Read by ascending address the written memory and confirm that it
         * has the expected data pattern.
         */
        printf("  Ascending Confirming... \n");
        for (p1 = (DT_UINT *) area; p1 < (DT_UINT *) max_address; p1++) 
        {
            data = *p1;
            if (data != (DT_UINT) p1) 
            {
                failures = retry_failure (i, p1, data, (DT_UINT) p1,
                                          failures);
            }
        }
        
        /* Read by descending address the written memory and confirm that it
         * has the expected data pattern.
         */
        printf("  Descending Confirming... \n");
        end = max_address - sizeof (DT_UINT);
        end = end & 0xFFFFFFFC;
        
        for (p1 = (DT_UINT *) end; p1 > (DT_UINT *) area; p1--) 
        {
            data = *p1;
            if (data != (DT_UINT) p1) 
            {
                failures = retry_failure (i, p1, data, (DT_UINT) p1, failures);
            }
        }
        
        /* Read from random addresses within the memory area.
         */
        printf("  Random Confirming... \n");
        for (i = 0; i < probes; i = i + 1) 
        {
			p1 = (volatile DT_UINT *)(area + \
				 (dt_random_generate(mask) * dram_size % area_size) + \
				 (dt_random_generate(dram_size - 1) & 0xFFFFFFF0));
			#ifdef  BIT_32_C
            debug("  Read Random Address: 0x%08X. Counts: %d.\n", (DT_UINT)p1, probes - i);
			#else
			debug("  Read Random Address: 0x%08llX. Counts: %d.\n", (DT_UINT)p1, probes - i);
			#endif
			data = *p1;
            if (data != (DT_UINT) p1) {
                failures = retry_failure (i, p1, data, (DT_UINT) p1, failures);
            }
        }
        
        if(failures == 0)
            printf("  [PASS]\n");
		
		total_fail += failures;
        
    }
	
    return (total_fail);
}

/*--------------------*/
/*  test_mem_pattern  */
/*--------------------*/
int32_t
test_mem_pattern (DT_UINT area, DT_UINT max_address, DT_UINT pattern, uint32_t passes)
{
    uint32_t i, failures;
    DT_UINT  data;
    volatile DT_UINT *p1;

    failures = 0;

    for (i = 0; i < passes; i = i + 1) 
    {
        CHECK_CTRL_C();
        
        /* write pattern*/
        for (p1 = (DT_UINT *) area; p1 < (DT_UINT *) max_address; p1++) 
        {
            *p1 = pattern;
        }
        
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
    }
    
    return (failures);
}



/*------------------------*/
/*  test_mem_alt_pattern  */
/*------------------------*/
int32_t
test_mem_alt_pattern (DT_UINT area, DT_UINT max_address, DT_UINT pattern, uint32_t passes)
{
    uint32_t i, failures;
    DT_UINT  data;
    volatile DT_UINT *p1;

    failures = 0;

    for (i = 0; i < passes; i = i + 1) 
    {
        CHECK_CTRL_C();
        
        if (i & 0x1)
            pattern = ~pattern;

        for (p1 = (DT_UINT *) area; p1 < (DT_UINT *) max_address; p1++) 
        {
            *p1 = pattern;
        }
        
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
    }
    
    return (failures);
}

/*----------------------*/
/*  test_mem_leftwalk0  */
/*----------------------*/
int32_t
test_mem_leftwalk0 (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t i, failures, ret_err;
    DT_UINT  pattern;

    failures = 0;
    for (i = 0; i < bursts; i = i + 1) 
    {
        for (pattern = 1; pattern != 0; pattern = pattern << 1) 
        {
            printf("  Bursts %4d, pattern = 0x%08lX    ", i + 1, (ulong)~pattern);
            ret_err = test_mem_alt_pattern (area, max_address, ~pattern, 1);
            
            if(ret_err == -1)
                return -1;
                
            if(ret_err == 0)
                printf("  [PASS]\n");
            
            failures += ret_err;
        }
    }
    return (failures);
}

/*----------------------*/
/*  test_mem_leftwalk1  */
/*----------------------*/
int32_t
test_mem_leftwalk1 (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t i, failures, ret_err;
    DT_UINT  pattern;

    failures = 0;
    for (i = 0; i < bursts; i = i + 1) 
    {
        for (pattern = 1; pattern != 0; pattern = pattern << 1) 
        {
            printf("  Bursts %4d, pattern = 0x%08lX    ", i + 1, (ulong)pattern);
            ret_err = test_mem_alt_pattern (area, max_address, pattern, 1);
            
            if(ret_err == -1)
                return -1;
            
            if(ret_err == 0)
                printf("  [PASS]\n");
            
            failures += ret_err;
        }
    }
    return (failures);
}

/*-----------------------*/
/*  test_mem_rightwalk0  */
/*-----------------------*/
int32_t
test_mem_rightwalk0 (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t i, failures, ret_err;
    DT_UINT  pattern;

    failures = 0;
    for (i = 0; i < bursts; i = i + 1) 
    {
        for (pattern = 0x80000000; pattern != 0; pattern = pattern >> 1) 
        {
            printf("  Bursts %4d, pattern = 0x%08lX    ", i + 1, (ulong)~pattern);
            ret_err = test_mem_alt_pattern (area, max_address, ~pattern, 1);
            
            if(ret_err == -1)
                return -1;
            
            if(ret_err == 0)
                printf("  [PASS]\n");
            
            failures += ret_err;
        }
    }
    return (failures);
}

/*-----------------------*/
/*  test_mem_rightwalk1  */
/*-----------------------*/
int32_t
test_mem_rightwalk1 (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t i, failures, ret_err;
    DT_UINT  pattern;

    failures = 0;
    for (i = 0; i < bursts; i = i + 1) 
    {
        for (pattern = 0x80000000; pattern != 0; pattern = pattern >> 1) 
        {
            printf("  Bursts %4d, pattern = 0x%08lX    ", i + 1, (ulong)pattern);
            ret_err = test_mem_alt_pattern (area, max_address, pattern, 1);
            
            if(ret_err == -1)
                return -1;
            
            if(ret_err == 0)
                printf("  [PASS]\n");
            
            failures += ret_err;
        }
    }
    return (failures);
}

/*--------------------*/
/*  test_mem_march_c  */
/*--------------------*/
int32_t
test_mem_march_c (DT_UINT area, DT_UINT max_address, DT_UINT pattern)
{
    uint32_t failures;
    DT_UINT  end;
    volatile DT_UINT *p1;

    failures = 0;

    end = max_address  - sizeof (DT_UINT);
    end = end & 0xFFFFFFFC;
    
    CHECK_CTRL_C();
    /* Pass 1 ascending addresses, fill memory with pattern. */
    printf ("  Phase1: ascending  addresses, fill  memory with pattern(0x%08lX).\n", 
                                                 (ulong)pattern);
    for (p1 = (DT_UINT *) area; p1 < (DT_UINT *) max_address; p1++) 
    {
        *p1 = pattern;
    }
    
    CHECK_CTRL_C();
    /* Pass 2: ascending addresses, read pattern and write ~pattern */
    printf ("  Phase2: ascending  addresses, read  pattern(0x%08lX) and write ~pattern(0x%08lX)\n", 
                                                 (ulong)pattern, (ulong)(~pattern));
    for (p1 = (DT_UINT *) area; p1 < (DT_UINT *) max_address; p1++) 
    {    
        if (*p1 != pattern) 
        {
            printf ("[Failure]: 0x%08lX: want 0x%08lX  got 0x%08lX  xor 0x%08lX\n",
                   (ulong) p1, (ulong)pattern, (ulong)*p1, (ulong)(pattern ^ (*p1)));
            failures++;
        }
        *p1 = ~pattern;
    }
    
    CHECK_CTRL_C();
    /* Pass 3: ascending addresses, read ~pattern and write pattern. */
    printf ("  Phase3: ascending  addresses, read ~pattern(0x%08lX) and write  pattern(0x%08lX).\n",
                                                  (ulong)(~pattern), (ulong)pattern);
    for (p1 = (DT_UINT *) area; p1 < (DT_UINT *) max_address; p1++) 
    {    
        if (*p1 != ~pattern) 
        {
            printf ("[Failure]: 0x%08lX: want 0x%08lX  got 0x%08lX  xor 0x%08lX\n",
                   (ulong) p1, (ulong)~pattern, (ulong)*p1, (ulong)((~pattern) ^ (*p1)));
            failures++;
        }
        *p1 = pattern;
    }
    
    CHECK_CTRL_C();
    /* Pass 4: descending addresses, read pattern and write ~pattern. */
    printf ("  Phase4: descending addresses, read  pattern(0x%08lX) and write ~pattern(0x%08lX).\n",
                                                    (ulong)pattern, (ulong)(~pattern));
    for (p1 = (DT_UINT *) end; p1 > (DT_UINT *) area; p1--) 
    {    
        if (*p1 != pattern) 
        {
            printf ("[Failure]: 0x%08lX: want 0x%08lX  got 0x%08lX  xor 0x%08lX\n",
                   (ulong) p1, (ulong)pattern, (ulong)*p1, (ulong)(pattern ^ (*p1)));
            failures++;
        }
        
        *p1 = ~pattern;
    }
    
    CHECK_CTRL_C();
    /* Pass 5: descending addresses, read ~pattern and write pattern. */
    printf ("  Phase5: descending addresses, read ~pattern(0x%08lX) and write  pattern(0x%08lX).\n",
                                                    (ulong)(~pattern), (ulong)pattern);
    for (p1 = (DT_UINT *) end; p1 > (DT_UINT *) area; p1--) 
    {
        if (*p1 != ~pattern) 
        {
            printf ("[Failure]: 0x%08lX: want 0x%08lX  got 0x%08lX  xor 0x%08lX\n",
                   (ulong) p1, (ulong)~pattern, (ulong)*p1, (ulong)((~pattern) ^ (*p1)));
            failures++;
        }
        *p1 = pattern;
    }
    
    CHECK_CTRL_C();
    /* Pass 6: ascending addresses, read pattern. */
    printf ("  Phase6: ascending  addresses, read  pattern(0x%08lX).\n", (ulong)pattern);
    for (p1 = (DT_UINT *) area; p1 < (DT_UINT *) max_address; p1++) 
    {    
        if (*p1 != pattern) 
        {
            printf ("[Failure]: 0x%08lX: want 0x%08lX  got 0x%08lX  xor 0x%08lX\n",
                   (ulong) p1, (ulong)pattern, (ulong)*p1, (ulong)(pattern ^ (*p1)));
            failures++;
        }
    }

    return (failures);
}

/*------------------*/
/*  test_mem_solid  */
/*------------------*/
int32_t
test_mem_solid (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t    i, failures, ret_err;

    failures = 0;

    for (i = 0; i < bursts; i = i + 1) 
    {
        CHECK_CTRL_C();
        printf("  Bursts %4d, pattern = 0x%08lX\n", i + 1, (ulong)0xFFFFFFFF);
        ret_err = test_mem_march_c(area, max_address, 0xFFFFFFFF);
        
        if(ret_err == -1)
            return -1;
        
        if(ret_err == 0)
            printf("  [PASS]\n");
            
        failures += ret_err;
    }
    return (failures);
}

/*-------------------------*/
/*  test_mem_alternating   */
/*-------------------------*/
int32_t
test_mem_alternating (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t    i, failures, ret_err;

    failures = 0;
    for (i = 0; i < bursts; i = i + 1) 
    {
        CHECK_CTRL_C();
        printf("  Bursts %4d, pattern = 0x%08lX\n", i + 1, (ulong)0x55555555);
        ret_err = test_mem_march_c(area, max_address, 0x55555555);
        
        if(ret_err == -1)
            return -1;
        
        if(ret_err == 0)
            printf("  [PASS]\n");
            
        failures += ret_err;
    }
    return (failures);
}

/*-------------------*/
/*  test_mem_random  */
/*-------------------*/
int32_t
test_mem_random (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t          i, failures, ret_err;
    DT_UINT          data, init_pattern, pattern;
    const DT_UINT    INC = 0x10101010;

    volatile DT_UINT *p1;

    failures = 0;
    for (i = 0; i < bursts; i = i + 1) 
    {
        CHECK_CTRL_C();
        ret_err = 0;
        
        init_pattern = (DT_UINT)dt_random_generate(0);
        printf("  Bursts %4d, random_pattern = 0x%08lX    ", i + 1, (ulong)init_pattern);
        
        pattern = init_pattern;

        /* Write the pattern to memory. Each location recieves the address
         * of the location. 
         */
        for (p1 = (DT_UINT *) area; p1 < (DT_UINT *) max_address; p1++) 
        {
            *p1 = pattern;
            pattern = pattern + INC;
        }

        /* Read the written memory and confirm that it has the expected
         * data pattern.
         */
        pattern = init_pattern;
        for (p1 = (DT_UINT *) area; p1 < (DT_UINT *) max_address; p1++) 
        {
            data = *p1;
            if (data != pattern) 
            {
                ret_err = retry_failure (i, p1, data, pattern, ret_err);
            }
            pattern = pattern + INC;
        }
        
        if(ret_err == 0)
            printf(" [PASS]");
            
        failures += ret_err;
        printf("\n");
    }
    return (failures);
}

/*---------------------*/
/*  retry_xor_failure  */
/*---------------------*/
static void
retry_xor_failure (DT_UINT burst, volatile DT_UINT *p1, volatile DT_UINT *p2)
{
    uint32_t i, repass, refail;
    DT_UINT  d1, d2;

    repass = 0;
    refail = 0;

    for (i = 0; i < RETRY_LIMIT; i = i + 1) 
    {
        d1 = *p1;
        d2 = *p2;

        if (d1 != d2) 
        {
            printf ("  ReRead[%3d] Fail: [0x%08lX] want 0x%08lX  "
                    "[0x%08lX] got 0x%08lX xor 0x%08lX\n",
                    i, (ulong) p2, (ulong)d2, 
                    (ulong)p1, (ulong)d1, (ulong)(d1 ^ d2));
            refail = refail + 1;
        } 
        else 
        {
            repass = repass + 1;
        }
    }
    
    printf ("  Reread %d times: Passed: %d, Failed %d\n",
            i, repass, refail);
}

/*----------------*/
/*  test_mem_xor  */
/*----------------*/
int32_t
test_mem_xor (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t i, j, failures, ret_err;
    DT_UINT  *p1, *p2, p1addrfix, p2addrfix, count, extent, pattern;
    register DT_UINT d1, d2;

    failures = 0;
    
    extent = max_address - area;
    count  = (extent / sizeof (DT_UINT)) / 2;

    p1addrfix = area & 0xFFFFFFFC;
    p2addrfix = (area + (extent / 2)) & 0xFFFFFFFC;

    /* Fill both halves of the memory area with identical randomized data.
     */
    p1 = (DT_UINT *) p1addrfix;
    p2 = (DT_UINT *) p2addrfix;
    
    pattern = (DT_UINT)dt_random_generate(0);

    for (j = 0; j < count; j++) 
    {
        *p1 = pattern * ((DT_UINT) p1);
        *p2 = *p1;
        p1++;
        p2++;
    }

      /* Make a series of passes over the memory areas. */
    for (i = 0; i < bursts; i = i + 1) 
    {
        CHECK_CTRL_C();
        ret_err = 0;
        
        /* XOR the data with a random value, applying the change to both
         * memory areas.
         */
        pattern = (DT_UINT)dt_random_generate(0);
        
        printf("  Bursts %4d, random_pattern = 0x%08lX, ", i + 1, (ulong)pattern);
        
        p1 = (DT_UINT *) p1addrfix;
        p2 = (DT_UINT *) p2addrfix;
        
        for (j = 0; j < count; j++) 
        {
            *p1++ ^= pattern;
            *p2++ ^= pattern;
        }

        /* Look for differences in the areas. If there is a mismatch, reset
         * both memory locations with the same pattern. Failing to do so
         * means that on all subsequent passes the pair of locations remain
         * out of sync giving spurious errors.
         */
        p1 = (DT_UINT *) p1addrfix;
        p2 = (DT_UINT *) p2addrfix;
        for (j = 0; j < count; j++) 
        {
            d1 = *p1;
            d2 = *p2;
            if (d1 != d2) 
            {
                if(ret_err == 0) printf("\n");
                
                printf ("[Failure][%2d]: "
                        "[0x%08lX] want 0x%08lX  "
                        "[0x%08lX] got 0x%08lX xor 0x%08lX\n",
                        i+1, (ulong) p2, (ulong)d2, 
                        (ulong) p1, (ulong)d1, (ulong)(d1 ^ d2));

                retry_xor_failure (i, p1, p2);

                *p1 = d2; // Synchronize the two areas, adjusting for the error.
                *p2 = d2;
                ret_err++;
            }
            p1++;
            p2++;
        }
        
        if(ret_err == 0)
        {
            printf("  [PASS]");
        }
            
        failures += ret_err;
        
        printf("\n");
    }
    return (failures);
}

int32_t dram_addr_rot(DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t i, j, k, failures, total_fail;
    DT_UINT start_value;
    DT_UINT read_start_addr;
    DT_UINT dram_start, area_size;
    DT_UINT dram_count, dram_size = 0x10000;
    
    dram_start = (DT_UINT)area;
    area_size  = (DT_UINT)(max_address - area);
    dram_count = area_size / dram_size * DT_UNIT_BYTE_SIZE;

    if(area_size < dram_size)
    {
        printf("end_addr - start_addr should large than 0x10000.\n");
        return 0;
    }
    
    volatile DT_UINT *start;
    volatile DT_UINT *read_start;
    
    total_fail = 0;
    
    for (k = 0; k < bursts; k = k + 1) 
    {
        CHECK_CTRL_C();
        
        failures = 0;
        printf("  Bursts %4d, address rotate ... ", k+1);
        
        for (i = 0; i < dram_count; i = i + DT_UNIT_BYTE_SIZE)
        {
            CHECK_CTRL_C();
            
            /* write pattern*/
            start       = (DT_UINT *)(dram_start + ((i/DT_UNIT_BYTE_SIZE)*dram_size)%(area_size));
            read_start  = (DT_UINT *)((DT_UINT)start + dram_size - DT_UNIT_BYTE_SIZE);
            read_start_addr = ((DT_UINT)start);
			#ifdef  BIT_32_C
			debug("i = %u/%u, start = 0x%08X, read_start = 0x%08X\n", 
                   i, dram_count, (DT_UINT)start, (DT_UINT)read_start);
			#else
			debug("i = %u/%llu, start = 0x%08llX, read_start = 0x%08llX\n", 
                   i, dram_count, (DT_UINT)start, (DT_UINT)read_start);
			#endif
            

            for(j=0; j < dram_size; j=j+DT_UNIT_BYTE_SIZE)
            {
                *start = (read_start_addr << (i % DT_UNIT_BIT_SIZE));
                start++;
                read_start_addr = read_start_addr + DT_UNIT_BYTE_SIZE ;
            }

            read_start_addr = ((DT_UINT)read_start);
            /* check data reversing order */  
            for(j=0; j < dram_size; j=j+DT_UNIT_BYTE_SIZE)
            {
                start_value = (*read_start);
                if(start_value != ((read_start_addr) << (i % DT_UNIT_BIT_SIZE)))
                {
                	#ifdef  BIT_32_C
					printf("\n[Failure][%2u]: "
					 	    "Decr addr[0x%08X] want 0x%08X got 0x%08X xor 0x%08X",
                            k+1, (DT_UINT)read_start, 
                            (read_start_addr << (i % DT_UNIT_BIT_SIZE)), start_value,
                            (read_start_addr << (i % DT_UNIT_BIT_SIZE)) ^ start_value);
					#else
					printf("\n[Failure][%2u]: "
					 	    "Decr addr[0x%08llX] want 0x%08llX got 0x%08llX xor 0x%08llX",
                            k+1, (DT_UINT)read_start, 
                            (read_start_addr << (i % DT_UNIT_BIT_SIZE)), start_value,
                            (read_start_addr << (i % DT_UNIT_BIT_SIZE)) ^ start_value);
					#endif
           
                    failures++;
                }
                read_start_addr = read_start_addr - DT_UNIT_BYTE_SIZE;
                read_start--;
            }

            read_start_addr += DT_UNIT_BYTE_SIZE;
            read_start++;

            /* check data sequential order */  
            for(j=0; j < dram_size; j=j+DT_UNIT_BYTE_SIZE)
            {
                start_value = (*read_start);
                if(start_value != ((read_start_addr) << (i % DT_UNIT_BIT_SIZE)))
                {
                	#ifdef  BIT_32_C
					printf("\n[Failure][%2u]: "
					 	    "Seq  addr[0x%08X] want 0x%08X got 0x%08X xor 0x%08X",
                            k+1, (DT_UINT)read_start, 
                            (read_start_addr << (i % DT_UNIT_BIT_SIZE)), start_value,
                            (read_start_addr << (i % DT_UNIT_BIT_SIZE)) ^ start_value);
					#else
					printf("\n[Failure][%2u]: "
					 	    "Seq  addr[0x%08llX] want 0x%08llX got 0x%08llX xor 0x%08llX",
                            k+1, (DT_UINT)read_start, 
                            (read_start_addr << (i % DT_UNIT_BIT_SIZE)), start_value,
                            (read_start_addr << (i % DT_UNIT_BIT_SIZE)) ^ start_value);
					#endif
                    failures++;
                }
                read_start_addr = read_start_addr + DT_UNIT_BYTE_SIZE;
                read_start++;
            }
        }
        if(failures == 0)
            printf("  [PASS]");
		
		printf("\n");
        
        total_fail += failures;
    }
    
    return (total_fail);
}

int32_t dram_com_addr_rot(DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t i, j, k, failures, total_fail;
    DT_UINT start_value;
    DT_UINT read_start_addr;
    DT_UINT dram_start, area_size;
    DT_UINT dram_count, dram_size = 0x10000;
    
    dram_start = (DT_UINT)area;
    area_size  = (DT_UINT)(max_address - area);
    dram_count = area_size / dram_size * DT_UNIT_BYTE_SIZE;

    if(area_size < dram_size)
    {
        printf("end_addr - start_addr should large than 0x10000.\n");
        return 0;
    }
    
    volatile DT_UINT *start;
    volatile DT_UINT *read_start;
    
    total_fail = 0;
    
    for (k = 0; k < bursts; k = k + 1) 
    {
        CHECK_CTRL_C();
        
        failures = 0;
        printf("  Bursts %4d, address ~rotate ...", k+1);
        
        for (i = 0; i < dram_count; i = i + DT_UNIT_BYTE_SIZE)
        {
            CHECK_CTRL_C();
            
            /* write pattern*/
            start       = (DT_UINT *)(dram_start + ((i/DT_UNIT_BYTE_SIZE)*dram_size)%(area_size));
            read_start  = (DT_UINT *)((DT_UINT)start + dram_size-DT_UNIT_BYTE_SIZE);
            read_start_addr = ((DT_UINT)start);
			#ifdef  BIT_32_C
            debug("i = %u/%u, start = 0x%08X, read_start = 0x%08X\n", 
                   i, dram_count, (DT_UINT)start, (DT_UINT)read_start);
			#else 
			debug("i = %u/%llu, start = 0x%08llX, read_start = 0x%08llX\n", 
                   i, dram_count, (DT_UINT)start, (DT_UINT)read_start);
			#endif
            
            for(j=0; j < dram_size; j=j+DT_UNIT_BYTE_SIZE)
            {
                *start = ~(read_start_addr << (i % DT_UNIT_BIT_SIZE));
                start++;
                read_start_addr = read_start_addr + DT_UNIT_BYTE_SIZE ;
            }

            read_start_addr = ((DT_UINT)read_start);
            /* check data reversing order */  
            for(j=0; j < dram_size; j=j+DT_UNIT_BYTE_SIZE)
            {
                start_value = (*read_start);
                if(start_value != (~(read_start_addr << (i % DT_UNIT_BIT_SIZE))))
                {
                	#ifdef  BIT_32_C
                    printf("\n[Failure][%2u]: "
                           "Decr addr[0x%08X] want 0x%08X got 0x%08X xor 0x%08X",
                           k+1, (DT_UINT)read_start, 
                           ~((read_start_addr) << (i % DT_UNIT_BIT_SIZE)), start_value,
                           (~((read_start_addr) << (i % DT_UNIT_BIT_SIZE))) ^ start_value);
					#else
					printf("\n[Failure][%2u]: "
                           "Decr addr[0x%08llX] want 0x%08llX got 0x%08llX xor 0x%08llX",
                           k+1, (DT_UINT)read_start, 
                           ~((read_start_addr) << (i % DT_UNIT_BIT_SIZE)), start_value,
                           (~((read_start_addr) << (i % DT_UNIT_BIT_SIZE))) ^ start_value);
					#endif
					failures++;
                }
                read_start_addr = read_start_addr - DT_UNIT_BYTE_SIZE;
                read_start--;
            }

            read_start_addr += DT_UNIT_BYTE_SIZE;
            read_start++;

            /* check data sequential order */  
            for(j=0; j < dram_size; j=j+DT_UNIT_BYTE_SIZE)
            {
                start_value = (*read_start);
                if(start_value != (~(read_start_addr << (i % DT_UNIT_BIT_SIZE))))
                {
                	#ifdef  BIT_32_C
                    printf("\n[Failure][%2u]: "
                           "Seq  addr[0x%08X] want 0x%08X got 0x%08X xor 0x%08X",
                           k+1, (DT_UINT)read_start, 
                           ~((read_start_addr) << (i % DT_UNIT_BIT_SIZE)), start_value,
                           (~((read_start_addr) << (i % DT_UNIT_BIT_SIZE))) ^ start_value);
                    #else
					printf("\n[Failure][%2u]: "
                           "Seq  addr[0x%08llX] want 0x%08llX got 0x%08llX xor 0x%08llX",
                           k+1, (DT_UINT)read_start, 
                           ~((read_start_addr) << (i % DT_UNIT_BIT_SIZE)), start_value,
                           (~((read_start_addr) << (i % DT_UNIT_BIT_SIZE))) ^ start_value);
					#endif
                    failures++;
                }
                read_start_addr = read_start_addr + DT_UNIT_BYTE_SIZE;
                read_start++;
            }
        }
        if(failures == 0)
            printf("  [PASS]");

		printf("\n");
            
        total_fail += failures;
    }
    
    return (total_fail);
}

/* The union data is stored starting from a low address */
static union{char c[4];unsigned long l;}endian_test={{'l','?','?','b'}};
#define ENDIANNESS ((char)endian_test.l)  /* ENDIANNESS='l': little endian. 'b': big endian. */

/*
 * write two half-words and read word.
 */
int32_t dram_half_word_access(DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t i, j, k, m, failures, total_fail;
    uint16_t h_word;
    uint32_t start_value;
    
    uint32_t dram_start, area_size;
    uint32_t dram_count, dram_size = 0x10000;
    
    dram_start = (uint32_t)area;
    area_size  = (uint32_t)(max_address - area);
    dram_count = area_size / dram_size;

    if(area_size < dram_size)
    {
        printf("end_addr - start_addr should large than 0x10000.\n");
        return 0;
    }
    
    volatile uint16_t *start_h;
    volatile uint32_t *start_w;
    
    total_fail = 0;
    
    for (k = 0; k < bursts; k = k + 1) 
    {
        CHECK_CTRL_C();
        
        failures = 0;
        printf("  Bursts %4d, half_word_access \n", k+1);
        
        for (m=0; m < (sizeof(dram_test_patterns)/sizeof(uint32_t)); m++)
        {
            CHECK_CTRL_C();
            printf("    pattern = 0x%08X ...", dram_test_patterns[m]);

            for (i = 0; i < dram_count; i = i + 1)
            {
                CHECK_CTRL_C();
                
                /* write half-word pattern*/
                start_h = (uint16_t *)((DT_UINT)(dram_start+ (i*dram_size)%(area_size)));
                start_w = (uint32_t *)((DT_UINT)(dram_start+ (i*dram_size)%(area_size) + dram_size-4));
				#ifdef  BIT_32_C
				debug("i = %d/%d, start_h = 0x%08X, start_w = 0x%08X\n", 
                       i, dram_count, (DT_UINT)start_h, (DT_UINT)start_w);
				#else 
				debug("i = %d/%d, start_h = 0x%08llX, start_w = 0x%08llX\n", 
                       i, dram_count, (DT_UINT)start_h, (DT_UINT)start_w);
				#endif

                for(j=0; j < dram_size; j=j+4)
                {
                    if(ENDIANNESS == 'b') /* big endian */
                    {
                        h_word = (uint16_t)(dram_test_patterns[m]);
                        *(start_h+1) = h_word;
                        h_word = (uint16_t)(dram_test_patterns[m] >> 16);
                        *(start_h) = h_word;
                    }
                    else
                    {
                        h_word = (uint16_t)(dram_test_patterns[m]);
                        *(start_h) = h_word;
                        h_word = (uint16_t)(dram_test_patterns[m] >> 16);
                        *(start_h+1) = h_word;
                    }
                    start_h+=2;
                }

                /* read word and check data */  
                for(j=0; j < dram_size; j=j+4)
                {
                    start_value = (*start_w);
                    if(start_value != dram_test_patterns[m])
                    {
                        printf("\n[Failure][%2d]: "
                               "addr[0x%08X] want 0x%08X got 0x%08X xor 0x%08X",
                               k+1, (uint32_t)((DT_UINT)start_w), 
                               dram_test_patterns[m], start_value,
                               dram_test_patterns[m] ^ start_value);
                        failures++;
                    }
                    start_w--;
                }
            }

            if(failures == 0)
                printf("  [PASS]");

			printf("\n");
            
            total_fail += failures;
        }
    }
    
    return (total_fail);
}

int32_t dram_byte_access(DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t i, j, k, m, failures, total_fail;
    uint8_t  byte;
    uint32_t start_value;
    
    uint32_t dram_start, area_size;
    uint32_t dram_count, dram_size = 0x10000;
    
    dram_start = (uint32_t)area;
    area_size  = (uint32_t)(max_address - area);
    dram_count = area_size / dram_size;

    if(area_size < dram_size)
    {
        printf("end_addr - start_addr should large than 0x10000.\n");
        return 0;
    }
    
    volatile uint8_t *start_b;
    volatile uint32_t *start_w;
    
    total_fail = 0;
    
    for (k = 0; k < bursts; k = k + 1) 
    {
        CHECK_CTRL_C();
        
        failures = 0;
        printf("  Bursts %4d, byte_access \n", k+1);
        
        for (m=0; m < (sizeof(dram_test_patterns)/sizeof(uint32_t)); m++)
        {
            CHECK_CTRL_C();
            printf("    pattern = 0x%08X ...", dram_test_patterns[m]);
            
            for (i = 0; i < dram_count; i = i + 1)
            {
                CHECK_CTRL_C();
                
                /* write byte pattern*/
                start_w = (uint32_t *)((DT_UINT)(dram_start+(i*dram_size)%(area_size)+dram_size-4));
                start_b =  (uint8_t *)((DT_UINT)(dram_start+(i*dram_size)%(area_size)));
                debug("i = %d/%d, start_b = 0x%08X, start_w = 0x%08X\n", 
                       i, dram_count, (uint32_t)((DT_UINT)start_b), (uint32_t)((DT_UINT)start_w));
                
                for(j=0; j < dram_size; j=j+4)
                {
                    if(ENDIANNESS == 'b') /* big endian */
                    {
                        byte = (uint8_t)(dram_test_patterns[m]);
                        *(start_b+3) = byte;
                        byte = (uint8_t)(dram_test_patterns[m] >> 8);
                        *(start_b+2) = byte;
                        byte = (uint8_t)(dram_test_patterns[m] >> 16);
                        *(start_b+1) = byte;
                        byte = (uint8_t)(dram_test_patterns[m] >> 24);
                        *(start_b) = byte;
                    }
                    else
                    {
                        byte = (uint8_t)(dram_test_patterns[m]);
                        *(start_b) = byte;
                        byte = (uint8_t)(dram_test_patterns[m] >> 8);
                        *(start_b+1) = byte;
                        byte = (uint8_t)(dram_test_patterns[m] >> 16);
                        *(start_b+2) = byte;
                        byte = (uint8_t)(dram_test_patterns[m] >> 24);
                        *(start_b+3) = byte;
                    }
                    start_b+=4;
                }

                /* read word and check data */  
                for (j=0; j < dram_size; j=j+4)
                {
                    start_value = *start_w;
                    if (start_value != dram_test_patterns[m])
                    {
                        printf("\n[Failure][%2d]: "
                               "addr[0x%08X] want 0x%08X got 0x%08X xor 0x%08X",
                               k + 1, (uint32_t)((DT_UINT)start_w), 
                               dram_test_patterns[m], start_value,
                               dram_test_patterns[m] ^ start_value);
                        failures++;
                    }
                    start_w--;
                }
            }
            
            if(failures == 0)
                printf("  [PASS]");

			printf("\n");
            
            total_fail += failures;
        }
    }
    return (total_fail);
}

int32_t dram_memcpy_test(DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t i, j, k, failures, total_fail;
    uint32_t start_value;
    uint32_t read_start_addr;
    
    uint32_t dram_start, area_size;
    uint32_t dram_count, dram_size = 0x10000;
    
    dram_start = (uint32_t)area;
    area_size  = (uint32_t)(max_address - area);
    dram_count = area_size / dram_size;

    if(area_size < dram_size)
    {
        printf("end_addr - start_addr should large than 0x10000.\n");
        return 0;
    }
    
    volatile uint32_t *start;
    volatile uint32_t *read_start;
    
    total_fail = 0;
    
    for (k = 0; k < bursts; k = k + 1) 
    {
        CHECK_CTRL_C();
        
        failures = 0;
        printf("  Bursts %4d, memory copy test ...", k+1);
        
        for (i=0; i < dram_count; i=i+1)
        {
            CHECK_CTRL_C();

            /* write pattern*/
            start = (uint32_t *)((DT_UINT)(dram_start + (i*dram_size)%(area_size)));
            read_start_addr = ((uint32_t)((DT_UINT)start) + (dram_size-4));
            read_start = (uint32_t *)(DT_UINT)(((uint32_t)((DT_UINT)start))+dram_size-4);
            debug("i = %d/%d, start = 0x%08X, read_start = 0x%08X\n", 
                   i, dram_count, (uint32_t)((DT_UINT)start), (uint32_t)((DT_UINT)start));
            
            for(j=0; j < dram_size; j=j+4)
            {
                *start = ((uint32_t)(DT_UINT)start << (i % 32));
                start++;
            }
            
            memcpy((char *)(DT_UINT)(dram_start+dram_size), 
                   (char *)(DT_UINT)(dram_start + (i*dram_size)%(area_size)), 
                   dram_size);

            /* check data */  
            read_start = (uint32_t *)(DT_UINT)(dram_start+dram_size+dram_size-4);
            for(j=0; j < dram_size; j=j+4)
            {
                start_value = (*read_start);
                if(start_value != ((read_start_addr) << (i % 32)))
                {
                     printf("\n[Failure][%2d]: "
                            "addr[0x%08X] want 0x%08X got 0x%08X xor 0x%08X",
                            k+1, (uint32_t)(DT_UINT)read_start , 
                            ((read_start_addr) << (i % 32)), start_value,
                            ((read_start_addr) << (i % 32)) ^ start_value);
                     failures++;
                }
                read_start = read_start - 1;
                read_start_addr = read_start_addr - 4;
            }
        }
        
        if(failures == 0)
            printf("  [PASS]");

		printf("\n");
        
        total_fail += failures;
    }

    return (total_fail);
}

