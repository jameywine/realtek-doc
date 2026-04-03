/*
 *  Copyright (c) 2014  TP-LINK Co., Ltd
 *
 *  TP-LINK Co., Ltd R&D SMB Product Line
 *
 */

#include <common.h>
#include <command.h>
#include "dt_common.h"

#define BIT_32_C
#ifdef BIT_32_C
typedef uint32_t uintxx_t;
#else
typedef uint64_t uintxx_t;
#endif

int32_t
test_mem_xtalk_pattern (DT_UINT area, DT_UINT max_address, uintxx_t pattern, uint32_t passes)
{
    uint32_t i, failures;
    uintxx_t data;
    volatile uintxx_t *p1;

    failures = 0;
    
    for (i = 0; i < passes; i = i + 1) 
    {
        CHECK_CTRL_C();
        
        /* write pattern*/
        for (p1 = (uintxx_t *) area; p1 < (uintxx_t *) max_address; p1++) 
        {
            *p1 = pattern;
        }
        
        /* Read the written memory and confirm that it has the expected
         * data pattern.
         */
        for (p1 = (uintxx_t *) area; p1 < (uintxx_t *) max_address; p1++) 
        {
            data = *p1; 
            
            if (data != pattern) 
            {
                failures++;
                printf ("\n[Failure]: 0x%08lX: want 0x%016LX  got 0x%016LX xor 0x%016LX",
                        (ulong)p1, (unsigned long long)pattern, (unsigned long long)data,
                        (unsigned long long)(pattern ^ data));
            }
        }
    }
    
    return (failures);
}

/*--------------------------------------*/
/*  dram_xtalk_pattern_data_line_tests  */
/*--------------------------------------*/
int32_t
dram_xtalk_pattern_data_line_tests(DT_UINT area, DT_UINT max_address, uint32_t bursts, 
                         uint32_t DQ_bit, uint32_t levelType)
{
    uint32_t j, databus_width, failures, ret_err = 0;
    uint64_t main_pat, help_pat;
    uint64_t pattern;
    
    databus_width = dt_dramDataBusWidth();
    
    switch (databus_width) 
    {  
        case  8:
            if(levelType == 0)
            {
                main_pat = ~(1 << DQ_bit) & 0xFF;
                help_pat = 0x00;
            }
            else
            {
                main_pat = (1 << DQ_bit) & 0xFF;
                help_pat = 0xFF;
            }
            pattern = (main_pat << 24) | (help_pat << 16) | (main_pat << 8) | (help_pat);
            pattern = (pattern << 32) | (pattern);
            break;
        case 16:
            if(levelType == 0)
            {
                main_pat = ~(1 << DQ_bit) & 0xFFFF;
                help_pat = 0x0000;
            }
            else
            {
                main_pat = (1 << DQ_bit) & 0xFFFF;
                help_pat = 0xFFFF;
            }
            pattern = (main_pat << 16) | (help_pat) ;
            pattern = (pattern << 32)  | (pattern);
            break;
        case 32:
        default:
            if(levelType == 0)
            {
                main_pat = ~(1 << DQ_bit) & 0xFFFFFFFF;
                help_pat = 0x00000000;
            }
            else
            {
                main_pat = (1 << DQ_bit) & 0xFFFFFFFF;
                help_pat = 0xFFFFFFFF;
            }
            pattern = (main_pat << 32) | (help_pat);
            break;
    }
    
    failures = 0;
    for (j = 0; j < bursts; j = j + 1) 
    {   
        printf("  Burst %4d, bit = %2d, level = %d, pattern=0x%016LX ...", 
               (j + 1), DQ_bit, levelType, (uint64_t)pattern);
               
        ret_err = test_mem_xtalk_pattern (area, max_address, pattern, 1);
        if(ret_err == -1)
        {
            return -1;
        }
        else
        {
            if(ret_err == 0)
                printf("  [PASS]");

			printf("\n");

            failures += ret_err;
        }
    }
    return (failures);
}

/*--------------------------------------*/
/*  dram_xtalk_pattern_addr_line_tests  */
/*--------------------------------------*/
int32_t
dram_xtalk_pattern_addr_line_tests (DT_UINT area, DT_UINT max_address, uint32_t bursts,
                                    uint32_t addr_bit, uint32_t levelType)
{
    volatile uchar  *p1, *p2;
    uint32_t i, failures;
    DT_UINT  main_pat, help_pat;
    uchar    mask_pattern, anti_pattern, temp_read;
    
    mask_pattern = 0x55;
    anti_pattern = ~mask_pattern;
    
    failures = 0;
    
    if(levelType == 0)
    {
        main_pat = ~(1 << addr_bit) & 0xFFFFFFFF;
        main_pat = main_pat & (max_address - area - 1);
        help_pat = 0x00000000;
    }
    else
    {
        main_pat = (1 << addr_bit) & 0xFFFFFFFF;
        main_pat = main_pat & (max_address - area - 1);
        help_pat = 0xFFFFFFFF & (max_address - area - 1);
    }
    
    p1 = (uchar *)(main_pat + area);
    p2 = (uchar *)(help_pat + area);
    
    for (i = 0; i < bursts; i = i + 1) 
    {
        printf("  Burst %4d, bit = %2d, level = %d, p1 = 0x%08lX , p2 = 0x%08lX ...", 
               i+1, addr_bit, levelType, (ulong)p1, (ulong)p2);
        CHECK_CTRL_C();
        
        /* Fill the memory at selected locations with pattern.
         */
        *p1 = mask_pattern;
        *p2 = anti_pattern;
        
        temp_read = ~(*p1);
        if(temp_read != *p2)
        {
            printf("\n[Failure]: p1 = 0x%.8lX(0x%x), p2 = 0x%.8lX(0x%x)\n",
                    (ulong)p1, (uchar)(p1[0]),(ulong)p2, (uchar)(p2[0]));
            failures++;
        }
		else
		{
			printf("  [PASS]\n");
		}
        
    }
    
    return (failures);
}

/*-----------------------*/
/*  run_dram_xtalk_tests  */
/*-----------------------*/
int32_t
run_dram_xtalk_tests(DT_UINT area, DT_UINT max_address, uint32_t bursts)
{
    uint32_t j, ret_err = 0, failures = 0;
    
    int databus_width = dt_dramDataBusWidth();
    int addrbus_width = dt_dramAddrBusWidth();
    
    printf("\n  Data Line Crosstalk Test...\n");
    for(j = 0; j < databus_width; j++)
    {
        ret_err = dram_xtalk_pattern_data_line_tests ((uint32_t)area, 
                  (uint32_t)(max_address), bursts, j, 0);
                    
        if(ret_err == -1)
            return CMD_RET_FAILURE;
        else
            failures += ret_err;
    }
    
    for(j = 0; j < databus_width; j++)
    {
        ret_err = dram_xtalk_pattern_data_line_tests ((uint32_t)area, 
                  (uint32_t)(max_address), bursts, j, 1);
                    
        if(ret_err == -1)
            return CMD_RET_FAILURE;
        else
            failures += ret_err;
    }
    
    printf("\n  Address Line Crosstalk Test...\n");
    for(j = 0; j < addrbus_width; j++)
    {
        ret_err = dram_xtalk_pattern_addr_line_tests ((uint32_t)area, 
                  (uint32_t)(max_address), bursts, j, 0);
                        
        if(ret_err == -1)
            return CMD_RET_FAILURE;
        else
            failures += ret_err;
    }
    
    for(j = 0; j < addrbus_width; j++)
    {
        ret_err = dram_xtalk_pattern_addr_line_tests ((uint32_t)area, 
                  (uint32_t)(max_address), bursts, j, 1);
                        
        if(ret_err == -1)
            return CMD_RET_FAILURE;
        else
            failures += ret_err;
    }
    
    return failures;
}

/*----------------------*/
/*  do_dram_xtalk_test  */
/*----------------------*/
/* dram xtalk count_limit test_type line_bit level_type */
int do_dram_xtalk_test (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint32_t failures = 0, ret_err = 0;
    uint32_t i, j, map_start, map_end, test_type, line_bit, level_type;
    uint32_t count_limit;
    int databus_width = dt_dramDataBusWidth();
    int addrbus_width = dt_dramAddrBusWidth();
    int run_position = dt_probe_position();
    
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
    
    if (argc > 1)
        count_limit = (ulong)simple_strtoul(argv[1], NULL, 10);
    else
        count_limit = 10;
    
    if (argc > 2)
    {
        test_type = (ulong)simple_strtoul(argv[2], NULL, 10);
    }
    else
    {
        test_type = 0;
    }
    
    if((test_type != 0) && (test_type != 1))
    {   
        printf("Error: test_type = %d\n", test_type);
        return CMD_RET_USAGE;
    }
    
    if (argc > 3)
    {
        level_type = (ulong)simple_strtoul(argv[3], NULL, 10);
        
        if((level_type != 0) && (level_type != 1))
        {
            printf("Error: level_type = %d\n", level_type);
            return CMD_RET_USAGE;
        }
    }
    else
    {
        level_type = 2;  /* undef */
    }
    
    
    
    if (argc > 4)
    {
        line_bit = (ulong)simple_strtoul(argv[4], NULL, 10);
    }
    else
    {
        if(test_type == 0)
            line_bit = databus_width;
        else
            line_bit = addrbus_width;
    }
    
    /* 地址线测试必须在高地址空间 */
    if(test_type == 1)
    {
        if(map_start != MapUpper)
        {
            printf("Address Line Crosstalk Test should RUN in MapUpper!\n");
            return 0;
        }
    }

    printf ("\nDram Memory Crosstalk Test\n\n");

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
            if(test_type == 0)
            {   
                /* data line crosstalk */
                puts("Data Line Crosstalk Test...\n\n");
                if(line_bit >= databus_width)
                {
                    if((level_type == 0) ||(level_type == 2))
                    {
                        for(j = 0; j < databus_width; j++)
                        {
                            ret_err = dram_xtalk_pattern_data_line_tests ((uint32_t)memoryMap[i].base, 
                                        (uint32_t)(memoryMap[i].base + memoryMap[i].extent), 
                                        count_limit, j, 0);
                            if(ret_err == -1)
                                return CMD_RET_FAILURE;
                            else
                                failures += ret_err;
                        }
                    }
                    
                    if((level_type == 1) ||(level_type == 2))
                    {
                        for(j = 0; j < databus_width; j++)
                        {
                            ret_err = dram_xtalk_pattern_data_line_tests ((uint32_t)memoryMap[i].base, 
                                        (uint32_t)(memoryMap[i].base + memoryMap[i].extent), 
                                        count_limit, j, 1);
                            if(ret_err == -1)
                                return CMD_RET_FAILURE;
                            else
                                failures += ret_err;
                        }
                    }
                }
                else
                {
                    ret_err = dram_xtalk_pattern_data_line_tests ((uint32_t)memoryMap[i].base, 
                                (uint32_t)(memoryMap[i].base + memoryMap[i].extent), 
                                count_limit, line_bit, level_type);
                    if(ret_err == -1)
                        return CMD_RET_FAILURE;
                    else
                        failures += ret_err;
                }
            }
            else
            {
                /* address line crosstalk */
                puts("Address Line Crosstalk Test...\n\n");
                if(line_bit >= addrbus_width)
                {
                    if((level_type == 0) ||(level_type == 2))
                    {
                        for(j = 0; j < addrbus_width; j++)
                        {
                            ret_err = dram_xtalk_pattern_addr_line_tests ((uint32_t)memoryMap[i].base, 
                                            (uint32_t)(memoryMap[i].base + memoryMap[i].extent), 
                                            count_limit, j, 0);
                            if(ret_err == -1)
                                return CMD_RET_FAILURE;
                            else
                                failures += ret_err;
                        }
                    }
                    
                    if((level_type == 1) ||(level_type == 2))
                    {
                        for(j = 0; j < addrbus_width; j++)
                        {
                            ret_err = dram_xtalk_pattern_addr_line_tests ((uint32_t)memoryMap[i].base, 
                                            (uint32_t)(memoryMap[i].base + memoryMap[i].extent), 
                                            count_limit, j, 1);
                            if(ret_err == -1)
                                return CMD_RET_FAILURE;
                            else
                                failures += ret_err;
                        }
                    }
                }
                else
                {
                    ret_err = dram_xtalk_pattern_addr_line_tests ((uint32_t)memoryMap[i].base, 
                                        (uint32_t)(memoryMap[i].base + memoryMap[i].extent), 
                                        count_limit, line_bit, level_type);
                    if(ret_err == -1)
                        return CMD_RET_FAILURE;
                    else
                        failures += ret_err;
                }
            }
            
            
            if(failures == 0)
                printf("%30s","[PASS]\n"); 
        }
    }
    
    return (0);
}
