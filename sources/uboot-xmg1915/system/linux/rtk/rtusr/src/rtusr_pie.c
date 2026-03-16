/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * $Revision: 79582 $
 * $Date: 2017-06-13 16:50:29 +0800 (Tue, 13 Jun 2017) $
 *
 * Purpose : Realtek Switch SDK Rtusr API Module
 *
 * Feature : The file have include the following module and sub-modules
 *           1) PIE
 *
 */

#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <rtk/pie.h>
#include <rtusr_util.h>
#include <rtdrv/rtdrv_netfilter.h>

int32
rtk_pie_meterIncludeIfg_get(uint32 unit, rtk_enable_t *pIfg_include)
{
    rtdrv_pieCfg_t pie_cfg;

    pie_cfg.unit = unit;
    GETSOCKOPT(RTDRV_PIE_METER_INCLUDE_IFG_GET, &pie_cfg, rtdrv_pieCfg_t, 1);
    *pIfg_include = pie_cfg.ifg_include;

    return RT_ERR_OK;
}

int32
rtk_pie_meterIncludeIfg_set(uint32 unit, rtk_enable_t ifg_include)
{
    rtdrv_pieCfg_t pie_cfg;

    pie_cfg.unit = unit;
    pie_cfg.ifg_include = ifg_include;
    SETSOCKOPT(RTDRV_PIE_METER_INCLUDE_IFG_SET, &pie_cfg, rtdrv_pieCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtk_pie_meterExceed_get(
    uint32  unit,
    uint32  meterIdx,
    uint32  *pIsExceed)
{
    rtdrv_pieCfg_t pie_cfg;

    pie_cfg.unit = unit;
    pie_cfg.meterIdx = meterIdx;
    GETSOCKOPT(RTDRV_PIE_METER_EXCEED_GET, &pie_cfg, rtdrv_pieCfg_t, 1);
    *pIsExceed = pie_cfg.isExceed;

    return RT_ERR_OK;
}

int32
rtk_pie_meterExceedAggregation_get(
    uint32  unit,
    uint32  *pExceedMask)
{
    rtdrv_pieCfg_t pie_cfg;

    pie_cfg.unit = unit;
    GETSOCKOPT(RTDRV_PIE_METER_EXCEED_AGGREGATION_GET, &pie_cfg, rtdrv_pieCfg_t, 1);
    *pExceedMask = pie_cfg.exceedMask;

    return RT_ERR_OK;
}

int32
rtk_pie_meterEntry_get(
    uint32  unit,
    uint32  meterIdx,
    rtk_pie_meterEntry_t   *pMeterEntry)
{
    rtdrv_pieCfg_t pie_cfg;

    pie_cfg.unit = unit;
    pie_cfg.meterIdx = meterIdx;
    GETSOCKOPT(RTDRV_PIE_METER_ENTRY_GET, &pie_cfg, rtdrv_pieCfg_t, 1);
    *pMeterEntry = pie_cfg.meterEntry;

    return RT_ERR_OK;
}

int32
rtk_pie_meterEntry_set(
    uint32  unit,
    uint32  meterIdx,
    rtk_pie_meterEntry_t   *pMeterEntry)
{
    rtdrv_pieCfg_t pie_cfg;

    pie_cfg.unit = unit;
    pie_cfg.meterIdx = meterIdx;
    pie_cfg.meterEntry = *pMeterEntry;
    SETSOCKOPT(RTDRV_PIE_METER_ENTRY_SET, &pie_cfg, rtdrv_pieCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtk_pie_blockLookupEnable_get(uint32 unit, uint32 block_idx, rtk_enable_t *pEnable)
{
    rtdrv_pieCfg_t pie_cfg;

    pie_cfg.unit = unit;
    pie_cfg.blockIdx = block_idx;

    GETSOCKOPT(RTDRV_PIE_BLOCKLOOKUPENABLE_GET, &pie_cfg, rtdrv_pieCfg_t, 1);
    *pEnable = pie_cfg.status;

    return RT_ERR_OK;
}    /* end of rtk_pie_blockLookupEnable_get */

int32
rtk_pie_blockLookupEnable_set(uint32 unit, uint32 block_idx, rtk_enable_t enable)
{
    rtdrv_pieCfg_t pie_cfg;

    pie_cfg.unit = unit;
    pie_cfg.blockIdx = block_idx;
    pie_cfg.status = enable;

    SETSOCKOPT(RTDRV_PIE_BLOCKLOOKUPENABLE_SET, &pie_cfg, rtdrv_pieCfg_t, 1);

    return RT_ERR_OK;
}    /* end of rtk_pie_blockLookupEnable_set */

int32
rtk_pie_blockGrouping_get(uint32 unit, uint32 block_idx, uint32* group_id,
    uint32* logic_id)
{
    rtdrv_pieCfg_t cfg;

    cfg.unit = unit;
    cfg.blockIdx = block_idx;

    GETSOCKOPT(RTDRV_PIE_BLOCKGROUPING_GET, &cfg, rtdrv_pieCfg_t, 1);

    *group_id = cfg.grpId;
    *logic_id = cfg.logicId;

    return RT_ERR_OK;
}    /* end of rtk_pie_blockGroupEnable_get */

int32
rtk_pie_blockGrouping_set(uint32 unit, uint32 block_idx, uint32 group_id,
    uint32 logic_id)
{
    rtdrv_pieCfg_t cfg;

    cfg.unit = unit;
    cfg.blockIdx = block_idx;
    cfg.grpId = group_id;
    cfg.logicId = logic_id;

    SETSOCKOPT(RTDRV_PIE_BLOCKGROUPING_SET, &cfg, rtdrv_pieCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_pie_blockGrouping_set */

int32
rtk_pie_template_get(uint32 unit, uint32 template_idx,
    rtk_pie_template_t *pTemplate)
{
    rtdrv_pieCfg_t pie_cfg;

    pie_cfg.unit = unit;
    pie_cfg.index = template_idx;

    GETSOCKOPT(RTDRV_PIE_TEMPLATE_GET, &pie_cfg, rtdrv_pieCfg_t, 1);
    osal_memcpy(pTemplate, &pie_cfg.template, sizeof(rtk_pie_template_t));

    return RT_ERR_OK;
}    /* end of rtk_pie_template_get */

int32
rtk_pie_template_set(uint32 unit, uint32 template_idx, rtk_pie_template_t *pTemplate)
{
    rtdrv_pieCfg_t pie_cfg;

    pie_cfg.unit = unit;
    pie_cfg.index = template_idx;
    osal_memcpy(&pie_cfg.template, pTemplate, sizeof(rtk_pie_template_t));

    SETSOCKOPT(RTDRV_PIE_TEMPLATE_SET, &pie_cfg, rtdrv_pieCfg_t, 1);

    return RT_ERR_OK;
}    /* end of rtk_pie_template_set */

int32
rtk_pie_templateField_check(uint32 unit, rtk_pie_phase_t phase,
    rtk_pie_templateFieldType_t type)
{
    rtdrv_pieCfg_t pie_cfg;

    pie_cfg.unit = unit;
    pie_cfg.phase = phase;
    pie_cfg.field_type = type;
    GETSOCKOPT(RTDRV_PIE_TEMPLATEFIELD_CHECK, &pie_cfg, rtdrv_pieCfg_t, 1);

    return RT_ERR_OK;
}    /* end of rtk_pie_templateField_check */

int32
rtk_pie_rangeCheckIp_get(uint32 unit, uint32 index, rtk_pie_rangeCheck_ip_t *pData)
{
    rtdrv_rangeCheckCfg_t pie_cfg;

    pie_cfg.unit = unit;
    pie_cfg.index = index;

    GETSOCKOPT(RTDRV_PIE_RANGECHECKIP_GET, &pie_cfg, rtdrv_rangeCheckCfg_t, 1);
    osal_memcpy(pData, &pie_cfg.range_ip, sizeof(rtk_pie_rangeCheck_ip_t));

    return RT_ERR_OK;
}    /* end of rtk_pie_rangeCheckIp_get */

int32
rtk_pie_rangeCheckIp_set(uint32 unit, uint32 index, rtk_pie_rangeCheck_ip_t *pData)
{
    rtdrv_rangeCheckCfg_t pie_cfg;

    pie_cfg.unit = unit;
    pie_cfg.index = index;
    osal_memcpy(&pie_cfg.range_ip, pData, sizeof(rtk_pie_rangeCheck_ip_t));

    SETSOCKOPT(RTDRV_PIE_RANGECHECKIP_SET, &pie_cfg, rtdrv_rangeCheckCfg_t, 1);

    return RT_ERR_OK;
}    /* end of rtk_pie_rangeCheckIp_set */

int32
rtk_pie_rangeCheck_get(uint32 unit, uint32 index,
    rtk_pie_rangeCheck_t *pData)
{
    rtdrv_rangeCheckCfg_t cfg;

    cfg.unit = unit;
    cfg.index = index;

    GETSOCKOPT(RTDRV_PIE_RANGECHECK_GET, &cfg, rtdrv_rangeCheckCfg_t, 1);
    osal_memcpy(pData, &cfg.range, sizeof(rtk_pie_rangeCheck_t));

    return RT_ERR_OK;
}   /* end of rtk_pie_rangeCheck_set */

int32
rtk_pie_rangeCheck_set(uint32 unit, uint32 index,
    rtk_pie_rangeCheck_t *pData)
{
    rtdrv_rangeCheckCfg_t cfg;

    cfg.unit = unit;
    cfg.index = index;
    osal_memcpy(&cfg.range, pData, sizeof(rtk_pie_rangeCheck_t));

    SETSOCKOPT(RTDRV_PIE_RANGECHECK_SET, &cfg, rtdrv_rangeCheckCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_pie_rangeCheck_set */

int32
rtk_pie_fieldSelector_get(
    uint32                       unit,
    uint32                       fs_idx,
    rtk_pie_fieldSelector_data_t *pFs)
{
    rtdrv_pieCfg_t pie_cfg;

    pie_cfg.unit = unit;
    pie_cfg.index = fs_idx;

    GETSOCKOPT(RTDRV_PIE_FIELDSELECTOR_GET, &pie_cfg, rtdrv_pieCfg_t, 1);
    osal_memcpy(pFs, &pie_cfg.fs, sizeof(rtk_pie_fieldSelector_data_t));

    return RT_ERR_OK;
}    /* end of rtk_pie_fieldSelector_get */

int32
rtk_pie_fieldSelector_set(
    uint32                       unit,
    uint32                       fs_idx,
    rtk_pie_fieldSelector_data_t *pFs)
{
    rtdrv_pieCfg_t pie_cfg;

    pie_cfg.unit = unit;
    pie_cfg.index = fs_idx;
    osal_memcpy(&pie_cfg.fs, pFs, sizeof(rtk_pie_fieldSelector_data_t));

    SETSOCKOPT(RTDRV_PIE_FIELDSELECTOR_SET, &pie_cfg, rtdrv_pieCfg_t, 1);

    return RT_ERR_OK;
}    /* end of rtk_pie_fieldSelector_set */

int32
rtk_pie_phase_get(uint32 unit, uint32 block_idx, rtk_pie_phase_t *phase)
{
    rtdrv_pieCfg_t pie_cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == phase), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memcpy(&pie_cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&pie_cfg.blockIdx, &block_idx, sizeof(uint32));
    GETSOCKOPT(RTDRV_PIE_PHASE_GET, &pie_cfg, rtdrv_pieCfg_t, 1);
    osal_memcpy(phase, &pie_cfg.phase, sizeof(rtk_pie_phase_t));

    return RT_ERR_OK;
}

int32
rtk_pie_phase_set(uint32 unit, uint32 block_idx, rtk_pie_phase_t phase)
{
    rtdrv_pieCfg_t pie_cfg;

    /* function body */
    osal_memcpy(&pie_cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&pie_cfg.blockIdx, &block_idx, sizeof(uint32));
    osal_memcpy(&pie_cfg.phase, &phase, sizeof(rtk_pie_phase_t));
    SETSOCKOPT(RTDRV_PIE_PHASE_SET, &pie_cfg, rtdrv_pieCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtk_pie_templateVlanSel_get(
    uint32                      unit,
    rtk_pie_phase_t             phase,
    uint32                      preTemplate_idx,
    rtk_pie_templateVlanSel_t   *pVlanSel)
{
    rtdrv_pieCfg_t pie_cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pVlanSel), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memcpy(&pie_cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&pie_cfg.phase, &phase, sizeof(rtk_pie_phase_t));
    osal_memcpy(&pie_cfg.templateIdx, &preTemplate_idx, sizeof(uint32));
    GETSOCKOPT(RTDRV_PIE_TEMPLATE_VLANSEL_GET, &pie_cfg, rtdrv_pieCfg_t, 1);
    osal_memcpy(pVlanSel, &pie_cfg.vlanSel, sizeof(rtk_pie_templateVlanSel_t));

    return RT_ERR_OK;
}

int32
rtk_pie_templateVlanSel_set(
    uint32                      unit,
    rtk_pie_phase_t             phase,
    uint32                      preTemplate_idx,
    rtk_pie_templateVlanSel_t   vlanSel)
{
    rtdrv_pieCfg_t pie_cfg;

    /* function body */
    osal_memcpy(&pie_cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&pie_cfg.phase, &phase, sizeof(rtk_pie_phase_t));
    osal_memcpy(&pie_cfg.templateIdx, &preTemplate_idx, sizeof(uint32));
    osal_memcpy(&pie_cfg.vlanSel, &vlanSel, sizeof(rtk_pie_templateVlanSel_t));
    SETSOCKOPT(RTDRV_PIE_TEMPLATE_VLANSEL_SET, &pie_cfg, rtdrv_pieCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtk_pie_meterDpSel_get(
    uint32                  unit,
    rtk_pie_meterDpSel_t    *pDpSel)
{
    rtdrv_pieCfg_t pie_cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pDpSel), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memcpy(&pie_cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_PIE_METER_DPSEL_GET, &pie_cfg, rtdrv_pieCfg_t, 1);
    osal_memcpy(pDpSel, &pie_cfg.dpSel, sizeof(rtk_pie_meterDpSel_t));

    return RT_ERR_OK;
}

int32
rtk_pie_meterDpSel_set(
    uint32                  unit,
    rtk_pie_meterDpSel_t    dpSel)
{
    rtdrv_pieCfg_t pie_cfg;

    /* function body */
    osal_memcpy(&pie_cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&pie_cfg.dpSel, &dpSel, sizeof(rtk_pie_meterDpSel_t));
    SETSOCKOPT(RTDRV_PIE_METER_DPSEL_SET, &pie_cfg, rtdrv_pieCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtk_pie_arpMacSel_get(uint32 unit, rtk_pie_arpMacSel_t *pArpMacSel)
{
    rtdrv_pieCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pArpMacSel), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_PIE_ARPMACSEL_GET, &cfg, rtdrv_pieCfg_t, 1);
    osal_memcpy(pArpMacSel, &cfg.arpMacSel, sizeof(rtk_pie_arpMacSel_t));

    return RT_ERR_OK;
}   /* end of rtk_pie_arpMacSel_get */

int32
rtk_pie_arpMacSel_set(uint32 unit, rtk_pie_arpMacSel_t arpMacSel)
{
    rtdrv_pieCfg_t cfg;

    /* function body */
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.arpMacSel, &arpMacSel, sizeof(rtk_pie_arpMacSel_t));
    SETSOCKOPT(RTDRV_PIE_ARPMACSEL_SET, &cfg, rtdrv_pieCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_pie_arpMacSel_set */

int32
rtk_pie_intfSel_get(uint32 unit, rtk_pie_intfSel_t *intfSel)
{
    rtdrv_pieCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == intfSel), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_PIE_INTFSEL_GET, &cfg, rtdrv_pieCfg_t, 1);
    osal_memcpy(intfSel, &cfg.intfSel, sizeof(rtk_pie_intfSel_t));

    return RT_ERR_OK;
}   /* end of rtk_pie_intfSel_get */

int32
rtk_pie_intfSel_set(uint32 unit, rtk_pie_intfSel_t intfSel)
{
    rtdrv_pieCfg_t cfg;

    /* function body */
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.intfSel, &intfSel, sizeof(rtk_pie_intfSel_t));
    SETSOCKOPT(RTDRV_PIE_INTFSEL_SET, &cfg, rtdrv_pieCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_pie_intfSel_set */

int32
rtk_pie_templateVlanFmtSel_get(uint32 unit, rtk_pie_phase_t phase,
    uint32 template_idx, rtk_pie_templateVlanFmtSel_t *pVlanFmtSel)
{
    rtdrv_pieCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pVlanFmtSel), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.phase, &phase, sizeof(rtk_pie_phase_t));
    osal_memcpy(&cfg.templateIdx, &template_idx, sizeof(uint32));
    GETSOCKOPT(RTDRV_PIE_TEMPLATEVLANFMTSEL_GET, &cfg, rtdrv_pieCfg_t, 1);
    osal_memcpy(pVlanFmtSel, &cfg.vlanFmtSel, sizeof(rtk_pie_templateVlanFmtSel_t));

    return RT_ERR_OK;
}   /* end of rtk_pie_templateVlanFmtSel_get */

int32
rtk_pie_templateVlanFmtSel_set(uint32 unit, rtk_pie_phase_t phase,
    uint32 template_idx, rtk_pie_templateVlanFmtSel_t vlanFmtSel)
{
    rtdrv_pieCfg_t cfg;

    /* function body */
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.phase, &phase, sizeof(rtk_pie_phase_t));
    osal_memcpy(&cfg.templateIdx, &template_idx, sizeof(uint32));
    osal_memcpy(&cfg.vlanFmtSel, &vlanFmtSel, sizeof(rtk_pie_templateVlanFmtSel_t));
    SETSOCKOPT(RTDRV_PIE_TEMPLATEVLANFMTSEL_SET, &cfg, rtdrv_pieCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_pie_templateVlanFmtSel_set */

int32
rtk_pie_meterTrtcmType_get(uint32 unit, rtk_pie_meterTrtcmType_t *type)
{
    rtdrv_pieCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == type), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_pieCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_PIE_METERTRTCMTYPE_GET, &cfg, rtdrv_pieCfg_t, 1);
    osal_memcpy(type, &cfg.type, sizeof(rtk_pie_meterTrtcmType_t));

    return RT_ERR_OK;
}   /* end of rtk_pie_meterTrtcmType_get */

int32
rtk_pie_meterTrtcmType_set(uint32 unit, rtk_pie_meterTrtcmType_t type)
{
    rtdrv_pieCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_pieCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_pie_meterTrtcmType_t));
    SETSOCKOPT(RTDRV_PIE_METERTRTCMTYPE_SET, &cfg, rtdrv_pieCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_pie_meterTrtcmType_set */

