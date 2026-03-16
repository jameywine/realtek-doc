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
 * $Revision: 80995 $
 * $Date: 2017-08-03 17:06:02 +0800 (Thu, 03 Aug 2017) $
 *
 * Purpose : Realtek Switch SDK Rtdrv Netfilter Module.
 *
 * Feature : Realtek Switch SDK Rtdrv Netfilter Module
 *
 */

/*
 * Include Files
 */
#include <linux/version.h>

#include <asm/uaccess.h>
#include <linux/netfilter.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
#include <linux/module.h>
#endif
#include <common/rt_autoconf.h>
#include <common/rt_error.h>
#include <common/debug/mem.h>
#include <osal/print.h>
#include <hal/mac/mem.h>
#include <ioal/mem32.h>
#include <drv/nic/nic.h>
#include <private/drv/nic/nic_diag.h>
#include <drv/watchdog/watchdog.h>
#include <rtdrv/ext/rtdrv_netfilter_ext_9300.h>
#if (defined(CONFIG_SDK_DRIVER_TEST) || defined(CONFIG_SDK_DRIVER_TEST_MODULE))
#include <sdk/sdk_test.h>
#endif
#include <hal/mac/reg.h>
#include <hal/common/halctrl.h>
#include <hal/chipdef/longan/rtk_longan_reg_struct.h>
#include <hal/chipdef/longan/rtk_longan_table_struct.h>
#include <rtk/eee.h>
#ifdef CONFIG_SDK_MODEL_MODE
#include <model_comm.h>
#include <tc.h>
#include <virtualmac/vmac_target.h>
#include <osal/time.h>
#endif

#include <osal/memory.h>
#include <osal/time.h>


/*
 * Symbol Definition
 */

#define EXT_PKTGEN_EN               1


#define EXT_PACKET_HDR_LEN          14    //dmac+smac+ethtype
#define PKTGEN_PORT_STREAM_NUM      2
#define PKTGEN_PORT_NUM             28

#define PKT_STREAM_IDX(_idx)      (_idx%PKTGEN_PORT_STREAM_NUM)

#define PKT_PAGE_CELL_NUM         4
#define PKT_CELL_BYTE             90
#define PKT_STREAM_IDX_MAX        55
#define PKT_STREAM_PAGE_BASE      0x800


#define REG_SPG_GLOBAL_INDEX_CTRL0          0xdcfc
#define REG_SPG_PB_ACCESS_CTRL2                 0xdcf8
#define REG_SPG_PB_ACCESS_CTRL1                 0xdcf4

uint32  page_base = PKT_STREAM_PAGE_BASE;

/*
 * Data Declaration
 */
rtdrv_ext_pkt_t *pDiagExtPacket;

uint8 pDiagExtPacketHdr[PKTGEN_PORT_NUM][PKTGEN_PORT_STREAM_NUM][EXT_PACKET_HDR_LEN];	/* enough to comprise ethernet, IP and TCP header */
uint8 include_vlan_header[PKTGEN_PORT_NUM][PKTGEN_PORT_STREAM_NUM] = {{0, 0}};
uint8 include_svlan_header[PKTGEN_PORT_NUM][PKTGEN_PORT_STREAM_NUM] = {{0, 0}};
uint8 include_ethtype_header[PKTGEN_PORT_NUM][PKTGEN_PORT_STREAM_NUM] = {{0, 0}};
uint8 spg_payload_type[PKTGEN_PORT_NUM][PKTGEN_PORT_STREAM_NUM] = {{0, 0}};
uint32 spg_payload_pattern[PKTGEN_PORT_NUM][PKTGEN_PORT_STREAM_NUM] = {{0, 0}};
uint32 oTag[PKTGEN_PORT_NUM][PKTGEN_PORT_STREAM_NUM] = {{0, 0}};
uint32 iTag[PKTGEN_PORT_NUM][PKTGEN_PORT_STREAM_NUM] = {{0, 0}};
uint32 pktLen[PKTGEN_PORT_NUM][PKTGEN_PORT_STREAM_NUM] = {{0, 0}};


/*
 * Macro Declaration
 */

/*
 * Function Declaration
 */

int32 _pkt_buf_alloc(rtdrv_ext_pkt_t **pPkt, uint32 len)
{
    *pPkt = osal_alloc(sizeof(rtdrv_ext_pkt_t));
    if (NULL == *pPkt)
    {
        osal_printf("Alloc memery fail !\n");
        return RT_ERR_FAILED;
    }

    (*pPkt)->data = osal_alloc(len + 4);

    if (NULL == (*pPkt)->data)
    {
        osal_printf("Alloc packet buffer fail !\n");
        return RT_ERR_FAILED;
    }

    (*pPkt)->length = len;

    return RT_ERR_OK;
}

int32 _pkt_buf_free(rtdrv_ext_pkt_t *pPkt)
{
    osal_free(pPkt->data);
    osal_free(pPkt);

    return RT_ERR_OK;
}

int32 _uint2buf(uint32 value, uint8 *pBuf)
{
    if (NULL == pBuf)
    {
        return RT_ERR_FAILED;
    }

    pBuf[0] = (value & 0xff000000) >> 24;
    pBuf[1] = (value & 0x00ff0000) >> 16;
    pBuf[2] = (value & 0x0000ff00) >> 8;
    pBuf[3] = (value & 0x000000ff) >> 0;

    return RT_ERR_OK;
}

int32 _dal_longan_packetGen_buf_oper(uint32 unit, uint32 cell, uint32 byte, uint32 isWr, uint32 *pVal)
{
    int32   ret = RT_ERR_OK;
    uint32 value =0;
    const char *pRWstr[] = {
        "Read",
        "Write",
        };

    #if EXT_SRAM_PKT_TEST
    if(cell >= PKT_STREAM_LAST_PORT_PAGE_NUM * PKT_PAGE_CELL_NUM)
    #else
    if(cell >= PKT_STREAM_PAGE_NUM * PKT_PAGE_CELL_NUM)
    #endif
    {
            osal_printf("%s packet cell is out of range: ERROR!  \n", pRWstr[isWr]);
            return RT_ERR_OUT_OF_RANGE;
    }

    if(byte >= PKT_CELL_BYTE)
    {
            osal_printf("%s packet cell byte is out of range: ERROR!  \n", pRWstr[isWr]);
            return RT_ERR_OUT_OF_RANGE;
    }

    //byte==88, only set 16bit value
    if (0 != byte%4)
    {
        osal_printf("%s packet cell byte position: ERROR!  \n", pRWstr[isWr]);
        return RT_ERR_INPUT;
    }

    #if EXT_SRAM_PKT_TEST
    value = (byte & 0xff) | ((cell & 0xff) << 8) | ((isWr&0x1)<<30);
    RT_ERR_CHK(ioal_mem32_write(unit, REG_SPG_PB_ACCESS_CTRL2, value), ret);
    #else
    RT_ERR_CHK(reg_field_write(unit, LONGAN_SPG_PB_ACCESS_CTRL2r,LONGAN_PB_TYPEf, &isWr), ret);
    RT_ERR_CHK(reg_field_write(unit, LONGAN_SPG_PB_ACCESS_CTRL2r,LONGAN_PB_CELL_INDEXf, &cell), ret);
    RT_ERR_CHK(reg_field_write(unit, LONGAN_SPG_PB_ACCESS_CTRL2r,LONGAN_PB_BYTE_INDEXf, &byte), ret);
    #endif

    if (isWr)
    {
        #if EXT_SRAM_PKT_TEST
        RT_ERR_CHK(ioal_mem32_write(unit, REG_SPG_PB_ACCESS_CTRL1, *pVal), ret);
        value = value | (1 << 31);
        RT_ERR_CHK(ioal_mem32_write(unit, REG_SPG_PB_ACCESS_CTRL2, value), ret);
        #else
        value = *pVal;
        RT_ERR_CHK(reg_field_write(unit, LONGAN_SPG_PB_ACCESS_CTRL1r,LONGAN_ACCESS_DATAf, &value), ret);
        value = 1;
        RT_ERR_CHK(reg_field_write(unit, LONGAN_SPG_PB_ACCESS_CTRL2r,LONGAN_PB_TRIGf, &value), ret);
        #endif
    }
    else
    {
        #if EXT_SRAM_PKT_TEST
        value = value | (1 << 31);
        RT_ERR_CHK(ioal_mem32_write(unit, REG_SPG_PB_ACCESS_CTRL2, value), ret);
        RT_ERR_CHK(ioal_mem32_read(unit, REG_SPG_PB_ACCESS_CTRL1, &value), ret);
        #else
        value = 1;
        RT_ERR_CHK(reg_field_write(unit, LONGAN_SPG_PB_ACCESS_CTRL2r,LONGAN_PB_TRIGf, &value), ret);
        RT_ERR_CHK(reg_field_read(unit, LONGAN_SPG_PB_ACCESS_CTRL1r,LONGAN_ACCESS_DATAf, &value), ret);
        #endif
        *pVal = value;
    }

    return ret;
}

//streamIdx = 0~55
// user pkt offset : 0,  2,  6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58, 62, 66, 70, 74, 78, 82,86     4n-2(n>=1)
// asic  byte        : 88, 84 80, 76, 72, 68, 64,60, 56, 52, 48,44, 40, 36, 32, 28, 24, 20, 16, 12, 8,  4, 0
// note : if offset != 0, the len should less than 90
int32 _pkt_stream_offset_oper(uint32 unit, uint32 streamIdx, uint32 offset, uint8 *pktBuf, uint32 len, uint32 isWrite)
{
    int32   ret = RT_ERR_OK;
    uint32  oper = 0;
    uint32  value;
    uint32 reg, regVal;
    uint32  pbCellIdx, pbByteIndex = 0;
    int32 i, pktIdx=0;
    uint32  cellCnt = 0, startCell = 0, startIdx = 0;
    int32   rwByte = 0, byteIdx, baseShift = 0, lastCellByte = 0, cellByteOffset = 0;
    int32   begin2Byte, OffsetByteIdx=0, OffsetRdByte=0;
    uint32  aligByte = 0;

    typedef struct pgn_byte_idx_s{
        uint8 usrIdx;
        uint8 asicIdx;
    }pgn_byte_idx_t;

    pgn_byte_idx_t idxArry[23];
    uint32 rdValMask[] = {0xff000000,0xffff0000,0xffffff00};
    uint32 rmHValMask[] = {0x00ffffff,0x0000ffff,0x000000ff};

    if (len <= 0)
        return RT_ERR_OK;

    #if EXT_SRAM_PKT_TEST
    if ((offset+len) > PKTGEN_LAST_PORT_MAX_LEN)
    #else
    if ((offset+len) > PKTGEN_MAX_LEN)
    #endif
    {
        osal_printf("Packet offset is out of range max :%d  \n", PKTGEN_MAX_LEN);
        return RT_ERR_OUT_OF_RANGE;
    }

   if (len < 4)
   {
        osal_printf("Packet wite length less than 4 \n");
        return RT_ERR_INPUT;
    }

    if (streamIdx > PKT_STREAM_IDX_MAX)
        return RT_ERR_OUT_OF_RANGE;

    if (isWrite)
        oper = 1;

    value = streamIdx;
    RT_ERR_CHK(reg_field_write(unit, LONGAN_SPG_PB_ACCESS_CTRL0r,LONGAN_PB_INDEXf, &value), ret);

    reg = REG_SPG_GLOBAL_INDEX_CTRL0 + (streamIdx/2) * 4;
    value = page_base + streamIdx * PKT_STREAM_PAGE_NUM;

    #if EXT_SRAM_PKT_TEST
    if (PKT_STREAM_IDX_MAX == streamIdx)
        value = page_base + streamIdx * PKT_STREAM_PAGE_NUM + (PKT_STREAM_LAST_PORT_PAGE_NUM - PKT_STREAM_PAGE_NUM);
    #endif

    if (0 == streamIdx%2)
    {
        RT_ERR_CHK(ioal_mem32_read(unit, reg, &regVal), ret);
        regVal = (regVal & 0xffff0000) | value;
        RT_ERR_CHK(ioal_mem32_write(unit, reg, regVal), ret);
    }
    else
    {
        RT_ERR_CHK(ioal_mem32_read(unit, reg, &regVal), ret);
        regVal = (regVal & 0x0000ffff) | (value << 16);
        RT_ERR_CHK(ioal_mem32_write(unit, reg, regVal), ret);
    }

    idxArry[0].usrIdx = 0;
    idxArry[0].asicIdx = 88;

    for(i = 1; i < sizeof(idxArry)/sizeof(pgn_byte_idx_t);i++)
    {
        idxArry[i].usrIdx = 4 * i -2;
        idxArry[i].asicIdx = 86 - idxArry[i].usrIdx;
    }

    cellCnt = ((offset + len) + 89)/PKT_CELL_BYTE;
    startCell = offset/PKT_CELL_BYTE;
    startIdx = offset%PKT_CELL_BYTE;

    for (i = 0; i < sizeof(idxArry)/sizeof(pgn_byte_idx_t);i++)
    {
       if (startIdx <=  idxArry[i].usrIdx)
       {
            if (startIdx ==  idxArry[i].usrIdx)
            {
                OffsetByteIdx = i;
                startIdx = idxArry[OffsetByteIdx].usrIdx;
            }
            else
            {
                OffsetByteIdx = i-1;
                OffsetRdByte = idxArry[i].usrIdx - startIdx;
            }

            pbByteIndex = idxArry[OffsetByteIdx].asicIdx;

            cellByteOffset = startIdx;
            break;
        }
    }

    if (i == sizeof(idxArry)/sizeof(pgn_byte_idx_t))
    {
        if (0 != startIdx)
        {
            lastCellByte = 1;
            cellByteOffset = 0;
            OffsetRdByte = 90 - startIdx;
            pbByteIndex = 0; //last byte of cell packet bufer
        }
    }

   if (0 != OffsetRdByte)
   {
        RT_ERR_CHK(_dal_longan_packetGen_buf_oper(unit,startCell,pbByteIndex,FALSE,&value),ret);
        value = value & rdValMask[4-1-OffsetRdByte];
        for (byteIdx = 0; byteIdx < OffsetRdByte; byteIdx++)
        {
            value |= (pktBuf[pktIdx + byteIdx] & 0xff) << ((OffsetRdByte-1)*8 - byteIdx*8);
        }

        RT_ERR_CHK(_dal_longan_packetGen_buf_oper(unit,startCell,pbByteIndex,TRUE,&value),ret);

        pktIdx+=OffsetRdByte;
        if (lastCellByte)
        {
            startIdx = 0;
            startCell += 1;
        }
        else
        {
            startIdx = idxArry[OffsetByteIdx+1].usrIdx;  //The algin positation
        }
   }

    for (pbCellIdx = startCell; pbCellIdx < cellCnt; pbCellIdx++)
    {
        for (i = 0; i < PKT_CELL_BYTE; )
        {
            if (pktIdx >= len)
                goto exit;

            if (0 != cellByteOffset)
            {
                if ((i < startIdx) && (pbCellIdx == startCell))
                {
                    i+=1;
                    continue;
                }
                else
                {
                    if ((i != 0) && (!aligByte))
                    {
                        i+=2;
                        aligByte = 1;
                    }
                }
            }

            if (0 == i)
                begin2Byte = TRUE;
            else
                begin2Byte = FALSE;

            OffsetRdByte = 0;
            if (TRUE == begin2Byte)
            {
                pbByteIndex = 90-2;
                rwByte = (len - pktIdx) >= 2 ? 2 : (len - pktIdx);
                baseShift = 8;
                if (rwByte < 2)
                    OffsetRdByte = (len - pktIdx);
            }
            else
            {
                pbByteIndex = 88-i%PKT_CELL_BYTE;
                rwByte = (len - pktIdx) >= 4 ? 4 : (len - pktIdx);
                baseShift = 24;
                if (rwByte < 4)
                {
                    OffsetRdByte = (len - pktIdx);
                }
            }

            //osal_printf("pktIdx = %d, len = %d, begin2Byte = %d, rwByte = %d, i = %d,  offset = %d ,  cell = %d, pbByteIndex = %d \n",
             //                  pktIdx, len ,begin2Byte, rwByte, i, offset, pbCellIdx, pbByteIndex);

            if (isWrite)
            {
                value = 0;
                if (0 != OffsetRdByte)
                {
                    RT_ERR_CHK(_dal_longan_packetGen_buf_oper(unit,pbCellIdx,pbByteIndex,FALSE,&value),ret);

                    if (begin2Byte)
                       value = value & 0x00ff;
                    else
                        value = value & rmHValMask[rwByte-1];
                }

                for (byteIdx = 0; byteIdx < rwByte; byteIdx++)
                {
                    //mask the value;
                    value |= (pktBuf[pktIdx + byteIdx] & 0xff) << (baseShift - byteIdx*8);
                    //if (0 != OffsetRdByte)
                        //osal_printf("Write value = 0x%8x \n", value);
                }
                RT_ERR_CHK(_dal_longan_packetGen_buf_oper(unit,pbCellIdx,pbByteIndex,oper,&value),ret);
            }
            else
            {
                RT_ERR_CHK(_dal_longan_packetGen_buf_oper(unit,pbCellIdx,pbByteIndex,oper,&value),ret);
                for (byteIdx = 0; byteIdx < rwByte; byteIdx++)
                {
                    pktBuf[pktIdx+byteIdx] = (value & (0xff << (baseShift -byteIdx*8))) >> (baseShift -byteIdx*8) ;
                }
            }

            if ((TRUE == begin2Byte))
                pktIdx+=2;
            else
                pktIdx+=4;

            i+=4;
        }
    }


exit:
    return ret;
}

//for test pkt buf
int32 _pkt_pattern_generate(uint32 port, uint32 streamIdx, uint8 *pktBuf, uint32 offset, uint32 len, rtdrv_ext_spg_payload_type_t type)
{
    int32 i;
    uint32 *ptr;

    RT_PARAM_CHK((NULL == pktBuf), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK(!HWP_PORT_EXIST(0, port), RT_ERR_PORT_ID);
    RT_PARAM_CHK(streamIdx >= PKTGEN_PORT_STREAM_NUM, RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(type > RTDRV_EXT_SPG_PAYLOAD_REPEAT, RT_ERR_INPUT);
    RT_PARAM_CHK((offset+len) >= PKTGEN_MAX_LEN, RT_ERR_INPUT);

    if (len <= 0)
        return RT_ERR_OK;

    switch (type)
        {
            case RTDRV_EXT_SPG_PAYLOAD_ZERO:
                break;

            case RTDRV_EXT_SPG_PAYLOAD_FIX:
                ptr = (uint32*)&pktBuf[offset];
                *ptr = spg_payload_pattern[port][streamIdx];
                break;

            case RTDRV_EXT_SPG_PAYLOAD_REPEAT:
                {
                    ptr = (uint32*)&pktBuf[offset];
                    for (i = 0; i < len/4; i++)
                    {
                        *ptr = spg_payload_pattern[port][streamIdx];
                        ptr++;
                    }

                    if (0 != len%4)
                    {
                        for (i = 0; i < len%4; i++)
                        {
                            pktBuf[offset + (len/4)*4 + i] =  (*ptr >> (24 - 8 * i)) & 0xff;
                        }
                    }
                }
                break;

            case RTDRV_EXT_SPG_PAYLOAD_INCR:
                {
                    for(i = 0; i < len; i++)
                    {
                        pktBuf[offset+i] = (i & 0xff);
                    }
                }
                break;

            case RTDRV_EXT_SPG_PAYLOAD_DECR:
                {
                   for(i = 0; i < len; i++)
                    {
                        pktBuf[offset+i] = (0xff - (i & 0xff));
                    }
                }
                break;

            default:
                break;
        }

    return RT_ERR_OK;
}

int32 pkt_stream_read(uint32 unit, uint32 streamIdx, uint32 offset, uint32 len, uint8 *pktBuf)
{
    return _pkt_stream_offset_oper(unit, streamIdx, offset, pktBuf, len, FALSE);
}

int32 pkt_stream_write(uint32 unit, uint32 streamIdx, uint32 offset, uint32 len, uint8 *pktBuf)
{
    return _pkt_stream_offset_oper(unit, streamIdx, offset, pktBuf, len, TRUE);
}


#if EXT_SRAM_PKT_TEST
int32 pkt_strem_buffer_section_set(uint32 unit, uint32 isFirst2k)
{
    if (isFirst2k)
        page_base = 0x0;
    else
        page_base = 0x0800;

    return RT_ERR_OK;
}

int32 pkt_stream_rw_time_test(uint32 unit)
{
    int32 ret = RT_ERR_OK;
    uint32 testCnt =  10;
    uint32 startTime =0, endTime = 0;
    uint32 i, j, strIdx;
    uint32 sectBuf[] = {FALSE, TRUE};
    uint32 pktLen = 12288;
    uint32 value;

    _pkt_buf_alloc(&pDiagExtPacket, pktLen);
    osal_memset(pDiagExtPacket->data, 0xff, pktLen);

    value = ENABLED;
    RT_ERR_CHK(reg_field_write(unit, LONGAN_SPG_GLB_CTRLr,LONGAN_SPG_MODEf, &value), ret);
    RT_ERR_CHK(reg_field_write(unit, LONGAN_PKB_ACC_DEBUG_CTRLr,LONGAN_DBG_ACC_PKB_ENf, &value), ret);

    RT_ERR_CHK(osal_time_usecs_get(&startTime), ret);
    for (j = 0; j < 2; j++)
    {
        RT_ERR_CHK(pkt_strem_buffer_section_set(unit, sectBuf[j]), ret);
        for (i = 0; i < testCnt; i++)
        {
            for (strIdx = 0; strIdx < 28*2; strIdx++)
            {
                RT_ERR_CHK(pkt_stream_write(unit, strIdx, 0, pktLen, pDiagExtPacket->data), ret);
            }
        }
    }
    RT_ERR_CHK(osal_time_usecs_get(&endTime), ret);
    osal_printf("write test %d times use time %u us, all page write avg time is: %d ms \n", testCnt, (endTime-startTime), (endTime-startTime)/(1000*testCnt));

    RT_ERR_CHK(osal_time_usecs_get(&startTime), ret);

    for (j = 0; j < 2; j++)
    {
        RT_ERR_CHK(pkt_strem_buffer_section_set(unit, sectBuf[j]), ret);
        for (i = 0; i < testCnt; i++)
        {
            for (strIdx = 0; strIdx < 28*2; strIdx++)
            {
                RT_ERR_CHK(pkt_stream_read(unit, strIdx, 0, pktLen, pDiagExtPacket->data), ret);
            }
        }
    }
    RT_ERR_CHK(osal_time_usecs_get(&endTime), ret);
    osal_printf("read test %d times use time %u us, all page write avg time is: %d ms \n", testCnt, (endTime-startTime), (endTime-startTime)/(1000*testCnt));

    _pkt_buf_free(pDiagExtPacket);

    //restore
    RT_ERR_CHK(pkt_strem_buffer_section_set(unit, FALSE), ret);
    value = DISABLED;
    RT_ERR_CHK(reg_field_write(unit, LONGAN_PKB_ACC_DEBUG_CTRLr,LONGAN_DBG_ACC_PKB_ENf, &value), ret);
    page_base = PKT_STREAM_PAGE_BASE;

    return RT_ERR_OK;
}


int32 pktIndex2cellByte(uint32 pktIdx, uint32 *pPageIdx, uint32 *pCell, uint32 *pByte)
{
    uint32 byteOffset = 0;

    RT_PARAM_CHK(pktIdx >= PKTGEN_LAST_PORT_MAX_LEN, RT_ERR_INPUT);
    *pPageIdx = pktIdx/360;
    *pCell = (pktIdx%360)/90;

    byteOffset = pktIdx%90;
    if (byteOffset%2)
        return RT_ERR_INPUT;

    if (0 == byteOffset)
        *pByte = 88;
    else
        *pByte = 86-byteOffset;

    return RT_ERR_OK;
}

int32 pktbuf_rw_test(uint32 unit, rtdrv_ext_bufferTest_t opter)
{
    int32 ret;
    uint32 pktLen = PKTGEN_LAST_PORT_MAX_LEN;
    int32 j, i;
    uint32 sectBuf[] = {TRUE, FALSE};
    uint32 value;
    uint32 strIdx;
    static  int32 wrValue = -1;
    uint32 pageNum =  PKT_STREAM_PAGE_NUM;

    RT_PARAM_CHK(opter >= RTDRV_EXT_BUF_TEST_END, RT_ERR_INPUT);

    _pkt_buf_alloc(&pDiagExtPacket, pktLen);
    osal_memset(pDiagExtPacket->data, 0x0, pktLen);

    value = ENABLED;
    RT_ERR_HDL(reg_field_write(unit, LONGAN_SPG_GLB_CTRLr,LONGAN_SPG_MODEf, &value), exit, ret);
    RT_ERR_HDL(reg_field_write(unit, LONGAN_PKB_ACC_DEBUG_CTRLr,LONGAN_DBG_ACC_PKB_ENf, &value), exit, ret);

    switch (opter)
        {
            case RTDRV_EXT_BUF_TEST_WR0:
            case RTDRV_EXT_BUF_TEST_WR1:
                {
                    if (RTDRV_EXT_BUF_TEST_WR0 == opter)
                    {
                        wrValue = 0;
                        osal_memset(pDiagExtPacket->data, 0x0, pktLen);
                    }
                    else
                    {
                        wrValue = 1;
                        osal_memset(pDiagExtPacket->data, 0xff, pktLen);
                    }

                    for (j = 0; j < 2; j++)
                    {
                        RT_ERR_HDL(pkt_strem_buffer_section_set(unit, sectBuf[j]), exit,  ret);
                        for (strIdx = 0; strIdx < 28*2; strIdx++)
                        {
                            if (strIdx < 27 * 2)
                                pktLen = PKTGEN_MAX_LEN;
                            else
                                pktLen = PKTGEN_LAST_PORT_MAX_LEN;
                            RT_ERR_HDL(pkt_stream_write(unit, strIdx, 0, pktLen, pDiagExtPacket->data), exit, ret);
                        }
                    }
                    osal_printf("Write packet buffer successful !\n\n");
                    break;
                }
            case RTDRV_EXT_BUF_TEST_READONLY:
                {
                    uint32 pageBaseIdx = 0;
                    for (j = 0; j < 2; j++)
                    {
                        RT_ERR_HDL(pkt_strem_buffer_section_set(unit, sectBuf[j]), exit,  ret);
                        for (strIdx = 0; strIdx < 28*2; strIdx++)
                        {
                            if (strIdx < 27 * 2)
                            {
                                pktLen = PKTGEN_MAX_LEN;
                                pageNum = PKT_STREAM_PAGE_NUM;
                                pageBaseIdx = j * 2048 + strIdx*PKT_STREAM_PAGE_NUM;
                                osal_printf("Packet buffer page for %d to %d :\n", pageBaseIdx,  pageBaseIdx + pageNum-1);
                            }
                            else
                            {
                                pktLen = PKTGEN_LAST_PORT_MAX_LEN;
                                pageNum = PKT_STREAM_LAST_PORT_PAGE_NUM;
                                pageBaseIdx = j * 2048 + 27*2*PKT_STREAM_PAGE_NUM;

                                if (PKT_STREAM_IDX_MAX == strIdx)
                                    osal_printf("Packet buffer page for %d to %d :\n",  pageBaseIdx+pageNum,  pageBaseIdx + 2*pageNum-1);
                                else
                                    osal_printf("Packet buffer page for %d to %d :\n",  pageBaseIdx,  pageBaseIdx + pageNum -1);
                            }

                            RT_ERR_HDL(pkt_stream_read(unit, strIdx, 0, pktLen, pDiagExtPacket->data), exit, ret);

                            #if 1
                            pktLen = 200; //read 100Byte
                            for (i = 0; i < pktLen; i++)
                            {
                                if ((0 == i%16) || (0 == i%360))
                                    osal_printf("0x%04x ", i);

                                osal_printf("%02x ", pDiagExtPacket->data[i]);

                                //per cell put \n
                                if (((i+1)%16 == 0) ||((i+1)%360 == 0))
                                    osal_printf("\n");
                            }
                            osal_printf("\n\n");
                            #endif
                        }
                    }
                    osal_printf("Read packet buffer successful !\n\n");
                    break;
                }
            case RTDRV_EXT_BUF_TEST_READCHECK:
                {
                    uint32 pageOffset = 0, cell = 0, byte = 0;
                    uint32 last, pktData;
                    uint32 errFlag = 0;
                    uint32 errCnt = 0;
                    uint32 pageIdx = 0;
                    uint32 pageBaseIdx = 0;
                    //uint32 *ptr;

                    if (-1 == wrValue)
                    {
                        osal_printf("Have Not write packet buffer. \n");
                        goto exit;
                    }

                    for (j = 0; j < 2; j++)
                    {
                        RT_ERR_HDL(pkt_strem_buffer_section_set(unit, sectBuf[j]), exit,  ret);
                        for (strIdx = 0; strIdx < 28*2; strIdx++)
                        {
                            if (strIdx < 27 * 2)
                            {
                                pageBaseIdx = j * 2048 + strIdx*PKT_STREAM_PAGE_NUM;
                                pktLen = PKTGEN_MAX_LEN;
                            }
                            else
                            {
                                pktLen = PKTGEN_LAST_PORT_MAX_LEN;
                                pageBaseIdx = j * 2048 + 27*2*PKT_STREAM_PAGE_NUM;
                            }

                            osal_memset(pDiagExtPacket->data, 0, pktLen);
                            RT_ERR_HDL(pkt_stream_read(unit, strIdx, 0, pktLen, pDiagExtPacket->data), exit, ret);

                            for (i = 0; i < pktLen; )
                            {
                                last = 0;
                                pktData = pDiagExtPacket->data[i] << 24 |   \
                                                pDiagExtPacket->data[i+1] << 16 |   \
                                                pDiagExtPacket->data[i+2] << 8 |    \
                                                pDiagExtPacket->data[i+3] << 0 ;
                                //ptr = (uint32*)&(pDiagExtPacket->data[i]);
                                //pktData = *ptr;

                                if ((i + 3) > pktLen)
                                {
                                    last = 1;
                                }

                                if (0 == wrValue)
                                {
                                    if ( 0x0 != pktData)
                                    {
                                        errFlag = 1;
                                        errCnt++;
                                        if (RT_ERR_OK != pktIndex2cellByte(i, &pageOffset, &cell, &byte))
                                            osal_printf("Error input packet buffer offset!! \n");

                                        if (PKT_STREAM_IDX_MAX == strIdx)
                                            pageIdx = (pageBaseIdx + PKT_STREAM_LAST_PORT_PAGE_NUM + pageOffset);
                                        else
                                            pageIdx = (pageBaseIdx + pageOffset);

                                        osal_printf("Packet buffer check error! 0x0 !=0x%08x, SW packet Idx: %d,  ASIC page: %d, cell:%d, byte:%d \n",
                                                                                            pktData, i, pageIdx, cell, byte);
                                    }
                                }
                                else
                                {
                                    if (((0xffffffff != pktData) && (0 == last)) || ((last) && (0xffff0000 != pktData)) )
                                    {
                                        errFlag = 1;
                                        errCnt++;
                                        if (RT_ERR_OK != pktIndex2cellByte(i, &pageOffset, &cell, &byte))
                                            osal_printf("Error input packet buffer offset!! \n");

                                        if (PKT_STREAM_IDX_MAX == strIdx)
                                            pageIdx = (pageBaseIdx + PKT_STREAM_LAST_PORT_PAGE_NUM + pageOffset);
                                        else
                                            pageIdx = (pageBaseIdx + pageOffset);

                                        osal_printf("Packet buffer check error! 0xffffffff !=0x%08x, SW packet Idx: %d, ASIC page: %u, cell:%u, byte:%u \n",
                                                                                                   pktData, i, pageIdx, cell, byte);
                                    }
                                }

                                if (0 == i%90)
                                    i +=2;
                                else
                                    i +=4;

                                if (i >= pktLen)
                                    break;

                                if (errCnt > 100)
                                {
                                    osal_printf("Check packet buffer failure !\n\n");
                                    goto exit;
                                }
                            }

                        }
                    }

                    if (errFlag)
                        osal_printf("Check packet buffer failure !\n\n");
                    else
                        osal_printf("Check packet buffer successful !\n\n");

                    break;
                }

            default:
                break;
        }


exit:
    _pkt_buf_free(pDiagExtPacket);
    return RT_ERR_OK;
}

#endif




/* Function Name:
 *      do_rtdrv_ext_set_ctl
 * Description:
 *      This function is called whenever a process tries to do setsockopt
 * Input:
 *      *sk   - network layer representation of sockets
 *      cmd   - ioctl commands
 *      *user - data buffer handled between user and kernel space
 *      len   - data length
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32 do_rtdrv_ext_set_ctl(struct sock *sk, int cmd, void *user_in, unsigned int len)
{
    void            *user = (char *)user_in + sizeof(rtdrv_msgHdr_t);
    int32   ret = RT_ERR_FAILED;
    int32   i = 0, payload_offset = 0;

    uint32  reg, field, value;
//    uint32  *ptr;
    rtk_port_linkStatus_t   link_status = PORT_LINKDOWN;
    rtk_port_t basePortId;
    rtdrv_ext_union_t   buf;

    if (user); /* to avoid compile warning */

    switch(cmd)
    {
    /** INIT **/
    /** L2 **/
    /** PORT **/
    /** VLAN **/
    /** STP **/
    /** REG **/
    /** COUNTER **/
    /** TRAP **/
    /** FILTER **/
    /** PIE **/
    /** QOS **/
    /** TRUNK **/
    /** DOT1X **/
    /** FLOWCTRL **/
    /** RATE **/
    /** SVLAN **/
    /** SWITCH **/
    /** NIC **/
    /** MPLS **/
    /** EEE **/

    /** IOL **/
    /** MODEL TEST **/
    /** packet generation */

    /**testing cases**/
#ifdef CONFIG_SDK_MODEL_MODE
        case RTDRV_EXT_MODEL_TEST_SET:
            copy_from_user(&buf.model_cfg, user, sizeof(rtdrv_ext_modelCfg_t));
            vmac_setCaredICType(CARE_TYPE_REAL);
            ret = tc_exec(buf.model_cfg.startID, buf.model_cfg.endID);
            break;

        case RTDRV_EXT_MODEL_TARGET_SET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_ext_unitCfg_t));
            vmac_setCaredICType(CARE_TYPE_REAL);
            ret = vmac_setTarget(buf.unit_cfg.data);
            break;

        case RTDRV_EXT_MODEL_REG_ACCESS_SET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_ext_unitCfg_t));
            vmac_setCaredICType(CARE_TYPE_REAL);
            vmac_setRegAccessType(buf.unit_cfg.data);
            ret = RT_ERR_OK;
            break;
#endif

#if EXT_PKTGEN_EN
     /*** packet generation ***/
        case RTDRV_EXT_PKTGEN_TX_CMD_SET:
            {
                uint32 portsEn;
                copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));

                ret = reg_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_GLB_CTRLr,LONGAN_GRP_TX_CMDf,&value);

                if (RTDRV_EXT_SPG_TXCMD_START == value && RTDRV_EXT_SPG_TXCMD_START == buf.pktgen_cfg.value)
                {
                    /* Only finished, can we go on */
                    ret += reg_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_GLOBAL_STSr,LONGAN_TX_DONE_PORTf,&value);
                    ret += reg_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_TX_GRP_CTRLr,LONGAN_GRP_TX_PORTf,&portsEn);

                    HWP_PORT_TRAVS_EXCEPT_CPU(buf.pktgen_cfg.unit, basePortId)
                    {
                        rtk_port_link_get(buf.pktgen_cfg.unit, basePortId, &link_status);
                        if (PORT_LINKDOWN == link_status)
                            continue;

                        if ((1 == (portsEn & (0x1 << basePortId))) && (0 == (value & (0x1 << basePortId))))
                        {
                            ret = RT_ERR_FAILED;
                            goto FAIL_EXIT;
                        }
                    }

                value = RTDRV_EXT_SPG_TXCMD_NOP;
                ret += reg_field_write(buf.pktgen_cfg.unit, LONGAN_SPG_GLB_CTRLr, LONGAN_GRP_TX_CMDf, &value);
            }
                ret += reg_field_write(buf.pktgen_cfg.unit, LONGAN_SPG_GLB_CTRLr,LONGAN_GRP_TX_CMDf, &buf.pktgen_cfg.value);
            }
            break;
        case RTDRV_EXT_PKTGEN_STATE_SET:
            {
                uint32 index0, index1, enable;
                rtk_port_t port;
                rtk_portmask_t pmsk;

                copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
                ret = reg_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_GLB_CTRLr,
                        LONGAN_SPG_MODEf, &enable);
                ret += reg_field_write(buf.pktgen_cfg.unit, LONGAN_SPG_GLB_CTRLr,
                        LONGAN_SPG_MODEf, &buf.pktgen_cfg.enable);

                //set the port stream index
                if (ENABLED == buf.pktgen_cfg.enable && DISABLED == enable)
                {
                    pmsk.bits[0] = 0x0fffffff;
                    pmsk.bits[1] = 0;
                    RTK_PORTMASK_SCAN(pmsk, port)
                    {
                        index0 = page_base + (port * 2 + 0) * PKT_STREAM_PAGE_NUM;
                        index1 = page_base + (port * 2 + 1) * PKT_STREAM_PAGE_NUM;
                        ret += reg_array_field_write(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_INDEX_CTRL0r,
                                    port, REG_ARRAY_INDEX_NONE, LONGAN_PORT0_S1_INDEXf, &index1);
                        ret += reg_array_field_write(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_INDEX_CTRL0r,
                                    port, REG_ARRAY_INDEX_NONE, LONGAN_PORT0_S0_INDEXf, &index0);
                   }
                }
            }
            break;
        case RTDRV_EXT_PKTGEN_DBG_ACC_STATE_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            ret = reg_field_write(buf.pktgen_cfg.unit, LONGAN_PKB_ACC_DEBUG_CTRLr,
                    LONGAN_DBG_ACC_PKB_ENf, &buf.pktgen_cfg.enable);
            break;
        case RTDRV_EXT_PKTGEN_PORT_STATE_SET:
            {
                uint32 myPort = 0, txcmd;
                uint32 readTime = 0;
                uint32 txDone = 0;
                copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));

                myPort = (0x1 << buf.pktgen_cfg.port);
                ret = reg_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_TX_GRP_CTRLr, LONGAN_GRP_TX_PORTf, &value);
                ret += reg_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_GLB_CTRLr, LONGAN_GRP_TX_CMDf, &txcmd);

                // disable port process
                if (DISABLED == buf.pktgen_cfg.enable)
                {
                    if (value & myPort)
                    {
                        if (RTDRV_EXT_SPG_TXCMD_START == txcmd)
                        {
                            //set only myport and txcmd stop_pause? or stop_reset ?
                            ret += reg_field_write(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_TX_GRP_CTRLr, LONGAN_GRP_TX_PORTf, &myPort);
                            txcmd = RTDRV_EXT_SPG_TXCMD_STOP;
                            ret += reg_field_write(buf.pktgen_cfg.unit, LONGAN_SPG_GLB_CTRLr, LONGAN_GRP_TX_CMDf, &txcmd);
                            // wait until this port status is TX finished
                            do {
                                ret += reg_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_GLOBAL_STSr, LONGAN_TX_DONE_PORTf, &txDone);
                                osal_time_mdelay(1);
                                readTime++;
                            } while ((txDone & myPort) == 0 && readTime < 20);
                            readTime = 0;

                            //set the valid port
                            value =  value & (~myPort);
                            ret += reg_field_write(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_TX_GRP_CTRLr,LONGAN_GRP_TX_PORTf, &value);
                            ret += reg_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_TX_GRP_CTRLr,LONGAN_GRP_TX_PORTf,&value);
                            while (value & myPort)
                            {
                                readTime++;
                                ret += reg_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_TX_GRP_CTRLr,LONGAN_GRP_TX_PORTf,&value);
                                if (readTime > 50)
                                    break;
                            }

                            txcmd = RTDRV_EXT_SPG_TXCMD_START;
                            ret += reg_field_write(buf.pktgen_cfg.unit, LONGAN_SPG_GLB_CTRLr,LONGAN_GRP_TX_CMDf,&txcmd);
                        }
                        else
                        {
                            //set the valid port
                            value =  value & (~myPort);
                            ret += reg_field_write(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_TX_GRP_CTRLr,LONGAN_GRP_TX_PORTf, &value);
                        }
                    }
                }
                else
                {
                    value = value | myPort;
                    ret += reg_field_write(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_TX_GRP_CTRLr,LONGAN_GRP_TX_PORTf, &value);
                }
            }
            break;
        case RTDRV_EXT_PKTGEN_PORT_IPG_LEN_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            buf.pktgen_cfg.value = buf.pktgen_cfg.value & 0xFFFFF;
            ret = reg_array_field_write(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_IPG_CTRLr,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, LONGAN_IPG_LENf, &buf.pktgen_cfg.value);
            break;
        case RTDRV_EXT_PKTGEN_PORT_TX_PKT_CNT_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            ret = reg_array_field_write(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_PKT_CNT_Hr,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, LONGAN_PKT_CNT_HIGHf, &buf.pktgen_cfg.pktlen_end);
            ret = reg_array_field_write(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_PKT_CNT_Lr,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, LONGAN_PKT_CNT_LOWf, &buf.pktgen_cfg.pktlen_start);
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_BADCRC_STATE_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL0r;
                field = LONGAN_BAD_CRC_EN_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL0r;
                field = LONGAN_BAD_CRC_EN_1f;
            }

            ret = reg_array_field_write(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.enable);

            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_DA_INC_STATE_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL0r;
                field = LONGAN_STREAM_DA_MOD_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL0r;
                field = LONGAN_STREAM_DA_MOD_1f;
            }

            ret = reg_array_field_write(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_SA_INC_STATE_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL0r;
                field = LONGAN_STREAM_SA_MOD_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL0r;
                field = LONGAN_STREAM_SA_MOD_1f;
            }

            ret = reg_array_field_write(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_LENTYPE_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL0r;
                field = LONGAN_STREAM_LEN_TYPE_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL0r;
                field = LONGAN_STREAM_LEN_TYPE_1f;
            }

            ret = reg_array_field_write(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_RANDOM_OFFSET_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL0r;
                field = LONGAN_STREAM_CONTENT_OFFSET_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL0r;
                field = LONGAN_STREAM_CONTENT_OFFSET_1f;
            }

            ret = reg_array_field_write(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_RANDOM_CONTENT_MODE_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL0r;
                field = LONGAN_STREAM_CONTENT_MOD_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL0r;
                field = LONGAN_STREAM_CONTENT_MOD_1f;
            }

            ret = reg_array_field_write(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_RANDOM_REPEAT_CONTENT_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL4r;
                field = LONGAN_STREAM_REPEAT_CONTENT_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL4r;
                field = LONGAN_STREAM_REPEAT_CONTENT_1f;
            }

            ret = reg_array_field_write(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_TX_PKT_CNT_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL1r;
                field = LONGAN_STREAM_PKT_CNT_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL1r;
                field = LONGAN_STREAM_PKT_CNT_1f;
            }

            ret = reg_array_field_write(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_PKT_LEN_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL2r;
                field = LONGAN_STREAM_LEN_RNG_START_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL2r;
                field = LONGAN_STREAM_LEN_RNG_START_1f;
            }
            ret = reg_array_field_write(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.pktlen_start);
            pktLen[buf.pktgen_cfg.port][buf.pktgen_cfg.stream_idx] = buf.pktgen_cfg.pktlen_start;

            if (RT_ERR_OK != ret)
                goto FAIL_EXIT;

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL2r;
                field = LONGAN_STREAM_LEN_RNG_END_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL2r;
                field = LONGAN_STREAM_LEN_RNG_END_1f;
            }
            ret = reg_array_field_write(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.pktlen_end);
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_SA_REPEAT_CNT_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL3r;
                field = LONGAN_STREAM_SA_REPEAT_CNT_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL3r;
                field = LONGAN_STREAM_SA_REPEAT_CNT_1f;
            }
            ret = reg_array_field_write(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_DA_REPEAT_CNT_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL3r;
                field = LONGAN_STREAM_DA_REPEAT_CNT_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL3r;
                field = LONGAN_STREAM_DA_REPEAT_CNT_1f;
            }
            ret = reg_array_field_write(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_SA_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            osal_memcpy(&pDiagExtPacketHdr[buf.pktgen_cfg.port][PKT_STREAM_IDX(buf.pktgen_cfg.stream_idx)][6], buf.pktgen_cfg.sa.octet, ETHER_ADDR_LEN);
            ret = RT_ERR_OK;

            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_DA_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            osal_memcpy(&pDiagExtPacketHdr[buf.pktgen_cfg.port][PKT_STREAM_IDX(buf.pktgen_cfg.stream_idx)][0], buf.pktgen_cfg.da.octet, ETHER_ADDR_LEN);
            ret = RT_ERR_OK;

            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_ETHTYPE_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            osal_memcpy(&pDiagExtPacketHdr[buf.pktgen_cfg.port][PKT_STREAM_IDX(buf.pktgen_cfg.stream_idx)][12], &buf.pktgen_cfg.etherType, 2);
            ret = RT_ERR_OK;
            break;

        case RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_ETHTYPE_STATE_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            include_ethtype_header[buf.pktgen_cfg.port][PKT_STREAM_IDX(buf.pktgen_cfg.stream_idx)] = buf.pktgen_cfg.enable;
            ret = RT_ERR_OK;
            break;

        case RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_ITAG_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            osal_memcpy(&iTag[buf.pktgen_cfg.port][PKT_STREAM_IDX(buf.pktgen_cfg.stream_idx)], &buf.pktgen_cfg.vlanHdr, 4);
            ret = RT_ERR_OK;

            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_ITAG_STATE_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            include_vlan_header[buf.pktgen_cfg.port][PKT_STREAM_IDX(buf.pktgen_cfg.stream_idx)] = buf.pktgen_cfg.enable;
            ret = RT_ERR_OK;

            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_OTAG_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            osal_memcpy(&oTag[buf.pktgen_cfg.port][PKT_STREAM_IDX(buf.pktgen_cfg.stream_idx)], &buf.pktgen_cfg.vlanHdr, 4);
            ret = RT_ERR_OK;

            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_OTAG_STATE_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            include_svlan_header[buf.pktgen_cfg.port][PKT_STREAM_IDX(buf.pktgen_cfg.stream_idx)] = buf.pktgen_cfg.enable;
            ret = RT_ERR_OK;

            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_PAYLOAD_TYPE_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            spg_payload_type[buf.pktgen_cfg.port][PKT_STREAM_IDX(buf.pktgen_cfg.stream_idx)] = buf.pktgen_cfg.patternType;
            ret = RT_ERR_OK;

            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_PAYLOAD_PATTERN_SET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            spg_payload_pattern[buf.pktgen_cfg.port][PKT_STREAM_IDX(buf.pktgen_cfg.stream_idx)] = buf.pktgen_cfg.pattern;
            ret = RT_ERR_OK;

            break;

        case RTDRV_EXT_PKTGEN_PORT_STREAM_OFFSET_PAYLOAD_SET:
            {
                uint8 pktbuf[4];
                copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
                _uint2buf(buf.pktgen_cfg.pattern, pktbuf);
                ret = pkt_stream_write(buf.pktgen_cfg.unit, (buf.pktgen_cfg.port * 2 + buf.pktgen_cfg.stream_idx),
                         buf.pktgen_cfg.len, 4, pktbuf);
            }
            break;

        case RTDRV_EXT_PKTGEN_PORT_STREAM_OFFSET_PAYLOAD8B_SET:
            {
                uint8 pktbuf[8];
                int32 i;
                uint32 value;
                copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
                for (i = 0; i < 2; i++)
                {
                    value = (uint32)(buf.pktgen_cfg.pattern8Btye[0] >> (32*(1-i)) & 0xffffffff);
                    _uint2buf(value, &pktbuf[4*i]);
                }
                ret = pkt_stream_write(buf.pktgen_cfg.unit, (buf.pktgen_cfg.port * 2 + buf.pktgen_cfg.stream_idx),
                         buf.pktgen_cfg.len, 8, pktbuf);
            }
            break;

        case RTDRV_EXT_PKTGEN_PORT_STREAM_OFFSET_PAYLOAD16B_SET:
            {
                uint8 pktbuf[16];
                int32 i, j;
                uint32 value;
                uint64 pattern;
                copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
                for (j = 0; j < 2; j++)
                {
                    pattern = buf.pktgen_cfg.pattern8Btye[j];
                    for (i = 0; i < 2; i++)
                    {
                        value = (uint32)(pattern >> (32*(1-i)) & 0xffffffff);
                        _uint2buf(value, &pktbuf[8*j + 4*i]);
                    }
                }
                ret = pkt_stream_write(buf.pktgen_cfg.unit, (buf.pktgen_cfg.port * 2 + buf.pktgen_cfg.stream_idx),
                         buf.pktgen_cfg.len, 16, pktbuf);
            }
            break;

        case RTDRV_EXT_PKTGEN_PORT_STREAM_OFFSET_PAYLOAD32B_SET:
            {
                uint8 pktbuf[32];
                int32 i, j;
                uint32 value;
                uint64 pattern;
                copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
                for (j = 0; j < 4; j++)
                {
                    pattern = buf.pktgen_cfg.pattern8Btye[j];
                    for (i = 0; i < 2; i++)
                    {
                        value = (uint32)(pattern >> (32*(1-i)) & 0xffffffff);
                        _uint2buf(value, &pktbuf[8*j + 4*i]);
                    }
                }
                ret = pkt_stream_write(buf.pktgen_cfg.unit, (buf.pktgen_cfg.port * 2 + buf.pktgen_cfg.stream_idx),
                         buf.pktgen_cfg.len, 32, pktbuf);
            }
            break;

        case RTDRV_EXT_PKTGEN_PORT_STREAM_PAYLOAD_SET:
            {
                rtdrv_ext_pktGenCfg_t        *pUser = user;
                uint8 *pBuf;
                copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
                pBuf = osal_alloc(buf.pktgen_cfg.len);
                if (NULL == pBuf)
                {
                    ret = RT_ERR_FAILED;
                }
                else
                {
                    copy_from_user(pBuf, pUser->pdata, buf.pktgen_cfg.len);
                    ret = pkt_stream_write(buf.pktgen_cfg.unit, (buf.pktgen_cfg.port * 2 + buf.pktgen_cfg.stream_idx),
                             buf.pktgen_cfg.value, buf.pktgen_cfg.len, pBuf);  //buf.pktgen_cfg.value is offset
                    osal_free(pBuf);
                }
            }
            break;

        case RTDRV_EXT_PKTGEN_PORT_TX:
            {
                uint32 txLen = 0;
                copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));

                /* Tx two stream */
               for (i = 0; i < PKTGEN_PORT_STREAM_NUM; i++)
               {
                    // if the stream not send, set the len = 0
                    if (0 == pktLen[buf.pktgen_cfg.port][i])
                        continue;

                    _pkt_buf_alloc(&pDiagExtPacket, PKTGEN_MAX_LEN);
                    osal_memset(pDiagExtPacket->data, 0, pDiagExtPacket->length);

                    osal_memcpy(pDiagExtPacket->data, &pDiagExtPacketHdr[buf.pktgen_cfg.port][i][0], 12);
                    payload_offset = 12;

                    if (include_svlan_header[buf.pktgen_cfg.port][i])
                    {
                        osal_memcpy(pDiagExtPacket->data + payload_offset, &oTag[buf.pktgen_cfg.port][i], 4);
                        payload_offset += 4;
                    }

                    if (include_vlan_header[buf.pktgen_cfg.port][i])
                    {
                        osal_memcpy(pDiagExtPacket->data + payload_offset, &iTag[buf.pktgen_cfg.port][i], 4);
                        payload_offset += 4;
                    }

                    if (include_ethtype_header[buf.pktgen_cfg.port][i])
                    {
                        osal_memcpy(pDiagExtPacket->data + payload_offset, &pDiagExtPacketHdr[buf.pktgen_cfg.port][i][12], 2);
                        payload_offset += 2;
                    }

                    txLen = pktLen[buf.pktgen_cfg.port][i];
                    if (pktLen[buf.pktgen_cfg.port][i] > PKTGEN_MAX_LEN)
                        txLen = PKTGEN_MAX_LEN;

                    _pkt_pattern_generate(buf.pktgen_cfg.port, i, pDiagExtPacket->data, payload_offset, txLen-payload_offset,
                        spg_payload_type[buf.pktgen_cfg.port][i]);
                    ret = pkt_stream_write(buf.pktgen_cfg.unit, (buf.pktgen_cfg.port * 2 + i), 0, txLen, pDiagExtPacket->data);

                    _pkt_buf_free(pDiagExtPacket);
                }
            }
            ret = RT_ERR_OK;

            break;
        case RTDRV_EXT_PKTGEN_PORT_TX_DYING_GASP:
                {
                    uint32 txLen = 0;
                    _pkt_buf_alloc(&pDiagExtPacket, PKTGEN_MAX_LEN);
                    osal_memset(pDiagExtPacket->data, 0, pDiagExtPacket->length);

                    osal_memcpy(pDiagExtPacket->data, &pDiagExtPacketHdr[buf.pktgen_cfg.port][i][0], 12);
                    payload_offset = 12;

                     /* Set OAM header */
                    pDiagExtPacket->data[12] = 0x88;
                    pDiagExtPacket->data[13] = 0x09;
                    pDiagExtPacket->data[14] = 0x03;
                    pDiagExtPacket->data[15] = 0x00;
                    pDiagExtPacket->data[16] = 0x52;
                    pDiagExtPacket->data[17] = 0x00;
                    payload_offset = 18;

                    txLen = pktLen[buf.pktgen_cfg.port][0];
                    if (pktLen[buf.pktgen_cfg.port][0] > PKTGEN_MAX_LEN)
                        txLen = PKTGEN_MAX_LEN;

                    _pkt_pattern_generate(buf.pktgen_cfg.port, 0, pDiagExtPacket->data, payload_offset, txLen-payload_offset,
                        spg_payload_type[buf.pktgen_cfg.port][0]);
                    ret = pkt_stream_write(buf.pktgen_cfg.unit, (buf.pktgen_cfg.port * 2 + 0), 0, txLen, pDiagExtPacket->data);

                    _pkt_buf_free(pDiagExtPacket);
                }
            ret = RT_ERR_OK;
            break;

        case RTDRV_EXT_PKTGEN_PACKET_BUFFER_TEST:
                {
                    #if EXT_SRAM_PKT_TEST
                    rtdrv_ext_bufferTest_t  testItem;
                    copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
                    testItem = buf.pktgen_cfg.value;
                    pktbuf_rw_test(buf.pktgen_cfg.unit, testItem);
                    #endif
                }
            ret = RT_ERR_OK;
            break;

       case RTDRV_EXT_PKTGEN_PACKET_BUFFER_SECT_SET:
            {
                #if EXT_SRAM_PKT_TEST
                copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
                ret = pkt_strem_buffer_section_set(buf.pktgen_cfg.unit, buf.pktgen_cfg.value);
                #endif
            }
            break;
#endif


        default:
            break;
    }

FAIL_EXIT:
    copy_to_user(&((rtdrv_msgHdr_t *)user_in)->ret_code, &ret, sizeof(ret));

    return ret;
}

/* Function Name:
 *      do_rtdrv_ext_get_ctl
 * Description:
 *      This function is called whenever a process tries to do getsockopt
 * Input:
 *      *sk   - network layer representation of sockets
 *      cmd   - ioctl commands
 * Output:
 *      *user - data buffer handled between user and kernel space
 *      len   - data length
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32 do_rtdrv_ext_get_ctl(struct sock *sk, int cmd, void *user_in, int *len)
{
    void                *user = (char *)user_in + sizeof(rtdrv_msgHdr_t);
    int32   ret = RT_ERR_FAILED;
    rtdrv_ext_union_t   buf;
    uint32  reg, field;
    #ifdef CONFIG_SDK_MODEL_MODE
    uint32  value;
    #endif

    if (user); /* to avoid compile warning */

    switch(cmd)
    {
    /** INIT **/
    /** L2 **/
    /** PORT **/

    /** VLAN **/
    /** STP **/
    /** REG **/
    /** COUNTER **/
    /** TRAP **/
    /** FILTER **/
    /** PIE **/
    /** QOS **/
    /** TRUNK **/
    /** DOT1X **/
    /** FLOWCTRL **/
    /** RATE **/
    /** SVLAN **/
    /** SWITCH **/

    /** NIC **/
    /** MPLS **/
    /** EEE **/
    /** IOL **/
    /** MODEL TEST **/
#ifdef CONFIG_SDK_MODEL_MODE
        case RTDRV_EXT_MODEL_TARGET_GET:
            ret = vmac_getTarget(&value);
            buf.unit_cfg.data = value;
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_ext_unitCfg_t));
            break;
        case RTDRV_EXT_MODEL_REG_ACCESS_GET:
             vmac_getRegAccessType(&value);
            buf.unit_cfg.data = value;
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_ext_unitCfg_t));
            ret = RT_ERR_OK;
            break;
#endif

        /*** packet generation ***/
        case RTDRV_EXT_PKTGEN_TX_CMD_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            ret = reg_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_GLB_CTRLr,
                    LONGAN_GRP_TX_CMDf, &buf.pktgen_cfg.value);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_STATE_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            ret = reg_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_GLB_CTRLr,
                    LONGAN_SPG_MODEf, &buf.pktgen_cfg.enable);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_DBG_ACC_STATE_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            ret = reg_field_read(buf.pktgen_cfg.unit, LONGAN_PKB_ACC_DEBUG_CTRLr,
                    LONGAN_DBG_ACC_PKB_ENf,&buf.pktgen_cfg.enable);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_PORT_STATE_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            ret = reg_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_TX_GRP_CTRLr,
                    LONGAN_GRP_TX_PORTf,&buf.pktgen_cfg.enable);
            if (buf.pktgen_cfg.enable & ((0x1) << buf.pktgen_cfg.port))
                buf.pktgen_cfg.enable = ENABLED;
            else
                buf.pktgen_cfg.enable = DISABLED;
            #if 0
            ret = reg_array_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_TX_GRP_CTRLr,
                buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, LONGAN_GRP_TX_PORTf,&buf.pktgen_cfg.enable);
            #endif
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_PORT_TX_DONE_STATE_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            ret = reg_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_GLOBAL_STSr,
                    LONGAN_TX_DONE_PORTf,&buf.pktgen_cfg.value);
            if (buf.pktgen_cfg.value & ((0x1) << buf.pktgen_cfg.port))
                buf.pktgen_cfg.value = 1;
            else
                buf.pktgen_cfg.value = 0;

            #if 0
            ret = reg_array_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_GLOBAL_STSr,
                buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, LONGAN_TX_DONE_PORTf,&buf.pktgen_cfg.value);
            #endif
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_PORT_IPG_LEN_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            ret = reg_array_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_IPG_CTRLr,
                buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, LONGAN_IPG_LENf,&buf.pktgen_cfg.value);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_PORT_TX_PKT_CNT_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            ret = reg_array_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_PKT_CNT_Hr,
                buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, LONGAN_PKT_CNT_HIGHf,&buf.pktgen_cfg.pktlen_end);
            ret += reg_array_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_PKT_CNT_Lr,
                buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, LONGAN_PKT_CNT_LOWf,&buf.pktgen_cfg.pktlen_start);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_PORT_PKT_CNT_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            ret = reg_array_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_PKT_CNT_DBG_Hr,
                buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, LONGAN_PKT_CNT_DBG_HIGHf,&buf.pktgen_cfg.pktlen_end);
            ret += reg_array_field_read(buf.pktgen_cfg.unit, LONGAN_SPG_PORT_PKT_CNT_DBG_Lr,
                buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, LONGAN_PKT_CNT_DBG_LOWf,&buf.pktgen_cfg.pktlen_start);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_BADCRC_STATE_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL0r;
                field = LONGAN_BAD_CRC_EN_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL0r;
                field = LONGAN_BAD_CRC_EN_1f;
            }
            ret = reg_array_field_read(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.enable);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_SA_INC_STATE_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL0r;
                field = LONGAN_STREAM_SA_MOD_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL0r;
                field = LONGAN_STREAM_SA_MOD_1f;
            }

            ret = reg_array_field_read(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_DA_INC_STATE_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL0r;
                field = LONGAN_STREAM_DA_MOD_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL0r;
                field = LONGAN_STREAM_DA_MOD_1f;
            }

            ret = reg_array_field_read(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_LENTYPE_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL0r;
                field = LONGAN_STREAM_LEN_TYPE_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL0r;
                field = LONGAN_STREAM_LEN_TYPE_1f;
            }

            ret = reg_array_field_read(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_RANDOM_OFFSET_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL0r;
                field = LONGAN_STREAM_CONTENT_OFFSET_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL0r;
                field = LONGAN_STREAM_CONTENT_OFFSET_1f;
            }

            ret = reg_array_field_read(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_RANDOM_CONTENT_MODE_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL0r;
                field = LONGAN_STREAM_CONTENT_MOD_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL0r;
                field = LONGAN_STREAM_CONTENT_MOD_1f;
            }

            ret = reg_array_field_read(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_RANDOM_REPEAT_CONTENT_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL4r;
                field = LONGAN_STREAM_REPEAT_CONTENT_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL4r;
                field = LONGAN_STREAM_REPEAT_CONTENT_1f;
            }

            ret = reg_array_field_read(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_TX_PKT_CNT_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL1r;
                field = LONGAN_STREAM_PKT_CNT_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL1r;
                field = LONGAN_STREAM_PKT_CNT_1f;
            }

            ret = reg_array_field_read(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_PKT_LEN_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL2r;
                field = LONGAN_STREAM_LEN_RNG_START_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL2r;
                field = LONGAN_STREAM_LEN_RNG_START_1f;
            }
            ret = reg_array_field_read(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.pktlen_start);
            if (RT_ERR_OK != ret)
                goto FAIL_EXIT;

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL2r;
                field = LONGAN_STREAM_LEN_RNG_END_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL2r;
                field = LONGAN_STREAM_LEN_RNG_END_1f;
            }
            ret = reg_array_field_read(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.pktlen_end);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_SA_REPEAT_CNT_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL3r;
                field = LONGAN_STREAM_SA_REPEAT_CNT_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL3r;
                field = LONGAN_STREAM_SA_REPEAT_CNT_1f;
            }
            ret = reg_array_field_read(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_PORT_STREAM_DA_REPEAT_CNT_GET:
            copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
            if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
            {
                ret = RT_ERR_OUT_OF_RANGE;
                goto FAIL_EXIT;
            }

            if (0 == buf.pktgen_cfg.stream_idx)
            {
                reg = LONGAN_SPG_PORT_STREAM0_CTRL3r;
                field = LONGAN_STREAM_DA_REPEAT_CNT_0f;
            }
            else
            {
                reg = LONGAN_SPG_PORT_STREAM1_CTRL3r;
                field = LONGAN_STREAM_DA_REPEAT_CNT_1f;
            }
            ret = reg_array_field_read(buf.pktgen_cfg.unit, reg,
                    buf.pktgen_cfg.port, REG_ARRAY_INDEX_NONE, field, &buf.pktgen_cfg.value);
            copy_to_user(user, &buf.pktgen_cfg, sizeof(rtdrv_ext_pktGenCfg_t));
            break;
        case RTDRV_EXT_PKTGEN_STREAM_GET:
           {
                uint8 *pPktBuf = NULL;
                int32 i;
                copy_from_user(&buf.pktgen_cfg, user, sizeof(rtdrv_ext_pktGenCfg_t));
                if (buf.pktgen_cfg.stream_idx >= PKTGEN_PORT_STREAM_NUM)
                {
                    ret = RT_ERR_OUT_OF_RANGE;
                    goto FAIL_EXIT;
                }

                pPktBuf = osal_alloc(buf.pktgen_cfg.len);
                ret = pkt_stream_read(buf.pktgen_cfg.unit, (buf.pktgen_cfg.port * 2 + buf.pktgen_cfg.stream_idx), 0, buf.pktgen_cfg.len, pPktBuf);
                osal_printf("Stream index %d data:\n", buf.pktgen_cfg.stream_idx);
                for (i = 0; i < buf.pktgen_cfg.len; i++)
                {
                    if (0 == i%16)
                        osal_printf("0x%04x ", i);

                    osal_printf("%02x ", pPktBuf[i]);

                    if ((i+1)%16 == 0)
                        osal_printf("\n");

                    if ((i+1)%256 == 0)
                        osal_printf("\n");
                }
                osal_printf("\n\n");
                osal_free(pPktBuf);
            }
            break;

        default:
            break;
    }

FAIL_EXIT:
    copy_to_user(&((rtdrv_msgHdr_t *)user_in)->ret_code, &ret, sizeof(ret));

    return 0;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,6,0))
struct nf_sockopt_ops rtdrv_ext_sockopts = {
    { NULL, NULL }, PF_INET,
    RTDRV_EXT_BASE_CTL, RTDRV_EXT_SET_MAX+1, do_rtdrv_ext_set_ctl, NULL,
    RTDRV_EXT_BASE_CTL, RTDRV_EXT_GET_MAX+1, do_rtdrv_ext_get_ctl, NULL
};
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
struct nf_sockopt_ops rtdrv_ext_sockopts = {
    { NULL, NULL }, PF_INET,
    RTDRV_EXT_BASE_CTL, RTDRV_EXT_SET_MAX+1, do_rtdrv_ext_set_ctl,
    RTDRV_EXT_BASE_CTL, RTDRV_EXT_GET_MAX+1, do_rtdrv_ext_get_ctl
};
#endif

