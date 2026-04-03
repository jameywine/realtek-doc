/*
 *  Copyright (c) 2014  TP-LINK Co., Ltd
 *
 *  TP-LINK Co., Ltd R&D SMB Product Line
 *
 */
 
/*
 * Memory Functions
 *
 */

#include <common.h>
#include <command.h>
// #include <mapmem.h>
#include "dt_common.h"

//#define map_sysmem(a,b)    (a)
//#define unmap_sysmem(a)    do{}while(0)

const uint32_t dram_mtest_patterns[] =   {
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

/*
vu_long *map_sysmem(vu_long dummy_addr, int size)
{
	size = size;
	return (vu_long *)dummy_addr;
}
void unmap_sysmem(void *a)
{
	a = a;
}
*/
static inline void *map_sysmem(phys_addr_t paddr, unsigned long len)
{
	return (void *)(uintptr_t)paddr;
}

static inline void unmap_sysmem(const void *vaddr)
{
}

long dram_mtest_alt(vu_long *buf, ulong start_addr, ulong end_addr,
              vu_long *dummy)
{
    vu_long *addr;
    ulong errs = 0;
    ulong val, readback;
    int j;
    vu_long offset;
    vu_long test_offset;
    vu_long pattern;
    vu_long temp;
    vu_long anti_pattern;
    vu_long num_words;
    static const ulong bitpattern[] = {
        0x00000001, /* single bit */
        0x00000003, /* two adjacent bits */
        0x00000007, /* three adjacent bits */
        0x0000000F, /* four adjacent bits */
        0x00000005, /* two non-adjacent bits */
        0x00000015, /* three non-adjacent bits */
        0x00000055, /* four non-adjacent bits */
        0xaaaaaaaa, /* alternating 1/0 */
    };

    num_words = (end_addr - start_addr) / sizeof(vu_long) - 1;
    
    /*
     * Data line test: write a pattern to the first
     * location, write the 1's complement to a 'parking'
     * address (changes the state of the data bus so a
     * floating bus doesn't give a false OK), and then
     * read the value back. Note that we read it back
     * into a variable because the next time we read it,
     * it might be right (been there, tough to explain to
     * the quality guys why it prints a failure when the
     * "is" and "should be" are obviously the same in the
     * error message).
     *
     * Rather than exhaustively testing, we test some
     * patterns by shifting '1' bits through a field of
     * '0's and '0' bits through a field of '1's (i.e.
     * pattern and ~pattern).
     */
    printf("  Data line ... "); 
    addr = buf;
    for (j = 0; j < sizeof(bitpattern) / sizeof(bitpattern[0]); j++) 
	{
        val = bitpattern[j];
        for (; val != 0; val <<= 1) 
		{
            *addr = val;
            *dummy  = ~val; /* clear the test data off the bus */
            readback = *addr;
            if (readback != val) 
			{
                printf("\n[Failure]: (data line): "
                       "expected 0x%08lX, actual 0x%08lX, xor 0x%08lX",
                       val, readback, val ^ readback);
                errs++;
                CHECK_CTRL_C();
            }
			
            *addr  = ~val;
            *dummy  = val;
            readback = *addr;
            if (readback != ~val) 
			{
                printf("\n[Failure]: (data line): "
                       "Is 0x%08lX, should be 0x%08lX, xor 0x%08lX",
                       readback, ~val, readback ^ (~val));
                errs++;
                CHECK_CTRL_C();
            }
        }
    }

    /*
     * Based on code whose Original Author and Copyright
     * information follows: Copyright (c) 1998 by Michael
     * Barr. This software is placed into the public
     * domain and may be used for any purpose. However,
     * this notice must not be changed or removed and no
     * warranty is either expressed or implied by its
     * publication or distribution.
     */

    /*
    * Address line test

     * Description: Test the address bus wiring in a
     *              memory region by performing a walking
     *              1's test on the relevant bits of the
     *              address and checking for aliasing.
     *              This test will find single-bit
     *              address failures such as stuck-high,
     *              stuck-low, and shorted pins. The base
     *              address and size of the region are
     *              selected by the caller.

     * Notes:   For best results, the selected base
     *              address should have enough LSB 0's to
     *              guarantee single address bit changes.
     *              For example, to test a 64-Kbyte
     *              region, select a base address on a
     *              64-Kbyte boundary. Also, select the
     *              region size as a power-of-two if at
     *              all possible.
     *
     * Returns:     0 if the test succeeds, 1 if the test fails.
     */
    printf("Address line ... "); 
    pattern = (vu_long) 0xaaaaaaaa;
    anti_pattern = (vu_long) 0x55555555;

    debug("%s:%d: length = 0x%.8lX\n", __func__, __LINE__, num_words);
    /*
     * Write the default pattern at each of the
     * power-of-two offsets.
     */
    for (offset = 1; offset < num_words; offset <<= 1)
        addr[offset] = pattern;

    /*
     * Check for address bits stuck high.
     */
    test_offset = 0;
    addr[test_offset] = anti_pattern;

    for (offset = 1; offset < num_words; offset <<= 1) 
	{
        temp = addr[offset];
        if (temp != pattern) 
		{
            printf("\n[Failure]: Address bit stuck high @ 0x%.8lX: "
				   "expected 0x%.8lX, actual 0x%.8lX, xor 0x%.8lX",
                   start_addr + offset, pattern, temp, pattern ^ temp);
            errs++;
            CHECK_CTRL_C();
        }
    }
    addr[test_offset] = pattern;

    /*
     * Check for addr bits stuck low or shorted.
     */
    for (test_offset = 1; test_offset < num_words; test_offset <<= 1) 
	{
        addr[test_offset] = anti_pattern;

        for (offset = 1; offset < num_words; offset <<= 1) 
		{
            temp = addr[offset];
            if ((temp != pattern) && (offset != test_offset)) 
			{
                printf("\n[Failure]: Address bit stuck low or shorted @ 0x%.8lX: "
					   "expected 0x%.8lX, actual 0x%.8lX, xor 0x%.8lX",
                       start_addr + offset, pattern, temp, pattern ^ temp);
                errs++;
                CHECK_CTRL_C();
            }
        }
        addr[test_offset] = pattern;
    }

    /*
     * Description: Test the integrity of a physical
     *      memory device by performing an
     *      increment/decrement test over the
     *      entire region. In the process every
     *      storage bit in the device is tested
     *      as a zero and a one. The base address
     *      and the size of the region are
     *      selected by the caller.
     *
     * Returns:     0 if the test succeeds, 1 if the test fails.
     */
    printf("Integrity ... ");
    num_words++;

    /*
     * Fill memory with a known pattern.
     */
    for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++) 
	{
        addr[offset] = pattern;
    }

    /*
     * Check each location and invert it for the second pass.
     */
    for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++) 
	{
        temp = addr[offset];
        if (temp != pattern) 
		{
            printf("\n[Failure]: (read/write): @ 0x%.8lX:"
                   " expected 0x%.8lX, actual 0x%.8lX, xor 0x%.8lX",
                   start_addr + offset, pattern, temp, pattern ^ temp);
            errs++;
            CHECK_CTRL_C();
        }

        anti_pattern = ~pattern;
        addr[offset] = anti_pattern;
    }

    /*
     * Check each location for the inverted pattern and zero it.
     */
    for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++) 
	{
        anti_pattern = ~pattern;
        temp = addr[offset];
        if (temp != anti_pattern)
		{
            printf("\n[Failure]: (read/write): @ 0x%.8lX:"
                   " expected 0x%.8lX, actual 0x%.8lX, xor 0x%.8lX",
                   start_addr + offset, anti_pattern, temp, anti_pattern ^ temp);
            errs++;
            CHECK_CTRL_C();
        }
        addr[offset] = 0;
    }
    
    if(errs == 0)
        printf(" [PASS]");

	printf("\n");

    return errs;
}

long dram_mtest_quick(vu_long *buf, ulong start_addr, ulong end_addr,
                vu_long pattern, int iteration)
{
    vu_long *end;
    vu_long *addr;
    ulong errs = 0;
    ulong incr, length;
    ulong val, readback;

    /* Alternate the pattern */
    incr = 1;
    if (iteration & 1) 
	{
        incr = -incr;
        /*
         * Flip the pattern each time to make lots of zeros and
         * then, the next time, lots of ones.  We decrement
         * the "negative" patterns and increment the "positive"
         * patterns to preserve this feature.
         */
        if (pattern & 0x80000000)
            pattern = -pattern; /* complement & increment */
        else
            pattern = ~pattern;
    }
	
    length = (end_addr - start_addr) / sizeof(ulong);
    end = buf + length;
    printf("  Pattern 0x%08lX  Writing... 0x%08lX  0x%08lX", pattern, (vu_long)buf, (vu_long)end);

    for (addr = buf, val = pattern; addr < end; addr++) 
	{
        *addr = val;
        val += incr;
		CHECK_CTRL_C();
    }

    puts("Reading...  ");
	CHECK_CTRL_C();

    for (addr = buf, val = pattern; addr < end; addr++) 
	{
        readback = *addr;
        if (readback != val) 
		{
            ulong offset = addr - buf;

            printf("\n[Failure]: Mem error @ 0x%08X: "
                   "found 0x%08lX, expected 0x%08lX, xor 0x%08lx",
                   (uint)(uintptr_t)(start_addr + offset),
                   readback, val, readback ^ val);
            errs++;

			CHECK_CTRL_C();
        }
        
        val += incr;
    }
    
    if(errs == 0)
        printf(" [PASS]");

	printf("\n");
    
    return errs;
}

int32_t
run_mtest_quick_tests (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    ulong    start, end;
    vu_long  *buf;
    uint32_t i, j, failures, ret_err;

    failures = 0;
    
    start = (ulong)area;
    end = (ulong)max_address;
    buf = map_sysmem(start, end - start);
    
    /* mtest_quick */
    for (i = 0; i < bursts; i = i + 1) 
    {
        printf("  Burst: %3d\n", i + 1);
        for(j = 0; j < ARRAY_SIZE(dram_mtest_patterns); j++)
        {
            ret_err = dram_mtest_quick(buf, start, end, dram_mtest_patterns[j], i);
            if (ret_err == -1)
                return -1;
            
            if (ret_err >= 0)
            {
                failures += ret_err;
            }
        }
    }
    
    return failures;
}

int32_t
run_mtest_alt_tests (DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    static   vu_long dummy_addr = 0; /* dummy addr for alt mem test */
    ulong    start, end;
    vu_long  *buf, *dummy;
    uint32_t i, failures, ret_err;

    failures = 0;
    
    start = (ulong)area;
    end = (ulong)max_address;
    buf = map_sysmem(start, end - start);
    dummy = map_sysmem((vu_long)(&dummy_addr), sizeof(vu_long));
    debug("dummy_addr = 0x%#08lx\n", (vu_long)&dummy_addr);
    
    for (i = 0; i < bursts; i = i + 1) 
    {
        printf("  Burst: %3d\n", i + 1);
        ret_err = dram_mtest_alt(buf, start, end, dummy);
        if (ret_err == -1)
            return -1;
        
        if (ret_err >= 0)
        {
            failures += ret_err;
        }
    }
    
    return failures;
}

/*
 * Perform a memory test. The complete test loops until
 * interrupted by ctrl-c or by a failure of one of the sub-tests.
 * dram mtest [test_type [[count [start_addr end_addr [pattern]]]]
 */
int do_dram_mtest_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    static vu_long dummy_addr = 0; /* dummy addr for alt mem test */
    ulong start, end;
    vu_long *buf, *dummy;
    int count_limit;
    int ret = 0;
    ulong errs = 0; /* number of errors, or -1 if interrupted */
    ulong pattern;
    int bursts;
    int alt_test = 0;  /* default: dram_mtest_quick*/

    uint32_t i, map_start, map_end;
    int      run_position = dt_probe_position();

    if(argc > 1)
        alt_test = simple_strtoul(argv[1], NULL, 10);
    else
        alt_test = 0;

    if (argc > 2)
        count_limit = (ulong)simple_strtoul(argv[2], NULL, 10);
    else
        count_limit = 0;

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
        pattern = (ulong)simple_strtoul(argv[5], NULL, 16);
    else
        pattern = 0x55555555;

    printf ("\nDram mtest Test\n\n");

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
      
            start = (ulong)memoryMap[i].base;
            end = (ulong)(memoryMap[i].base + memoryMap[i].extent);
            
            printf("Testing 0x%08x ... 0x%08x:\n", (uint)start, (uint)end);
            debug("%s:%d: start 0x%#08lx end 0x%#08lx\n", __func__, __LINE__, start, end);

            buf = map_sysmem(start, end - start);
            dummy = map_sysmem((vu_long)(&dummy_addr), sizeof(vu_long));
            debug("dummy_addr = 0x%#08lx\n", (vu_long)&dummy_addr);
            
            for (bursts = 0; !count_limit || bursts < count_limit; bursts++) 
            {
                if (ctrlc()) 
				{
                    errs = -1;
                    break;
                }

                printf("  Bursts: %3d\n", bursts + 1);

                if (alt_test) 
				{
                    errs = dram_mtest_alt(buf, start, end, dummy);
                } 
				else 
                {
                    errs = dram_mtest_quick(buf, start, end, pattern, bursts);
                }
				
                if (errs == -1)
                {
                    putc('\n');
                    return CMD_RET_FAILURE;
                }
                
                if (errs >= 0)
                {
                    ret += errs;
                }
            }

            void *vbuf = (void *)buf;
            void *vdummy = (void *)dummy;
            unmap_sysmem(vbuf);
            unmap_sysmem(vdummy);
        }
    }
    return ret; 
}

