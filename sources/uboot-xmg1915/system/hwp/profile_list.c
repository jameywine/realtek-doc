/*
 * Copyright (C) 2009-2016 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 81060 $
 * $Date: 2017-08-07 17:47:52 +0800 (Mon, 07 Aug 2017) $
 *
 * Purpose : Model list.
 *           This file shall be custom defined.
 * Feature :
 *
 */

/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#include <common/rt_error.h>
#include <osal/lib.h>
#include <osal/print.h>
#include <hwp/hw_profile.h>
#include <hal/chipdef/chipdef.h>
#include <drv/gpio/generalCtrl_gpio.h>
#include <common/debug/rt_log.h>
#include <common/util/rt_util.h>

/* Include hardware profile files of supported boards */

#if defined(CONFIG_SDK_RTL9300)
    #include <hwp/hw_profiles/rtl9301_2x8214QF_4xge.c>
    #include <hwp/hw_profiles/rtl9301_3x8218b_4xge.c>
    #include <hwp/hw_profiles/rtl9301_8218b_4xge.c>
    #include <hwp/hw_profiles/rtl9301_8218b_4xge_cascade.c>
    #include <hwp/hw_profiles/rtl9301_2x8214QF_4xge_cascade.c>
    #include <hwp/hw_profiles/rtl9301_14QF_4xge_18b_4xge_cascade.c>
    #include <hwp/hw_profiles/rtl9303_8xge.c>
    #include <hwp/hw_profiles/rtl9301_3x8218d_4xge.c>
    #include <hwp/hw_profiles/rtl9301_6x8214QF_4xge.c>
    #include <hwp/hw_profiles/rtl9301_6x8218d_2x8295r_cascade.c>
    #include <hwp/hw_profiles/rtl9301_2x8218b_4x8218d_2x8295r_cascade.c>
    #include <hwp/hw_profiles/rtl9303_2xcust1.c>
    #include <hwp/hw_profiles/rtl9302c_4xcust1.c>
    #include <hwp/hw_profiles/rtl9302b_2x8218d_2xcust1_4xge.c>

#endif

#if defined(CONFIG_SDK_RTL8390)
  #include <hwp/hw_profiles/rtl8396m_8218b_8214qf_8295r_C45_demo.c>
  #include <hwp/hw_profiles/rtl8396m_8218b_8214qf_8295r_demo.c>
  #include <hwp/hw_profiles/rtl8396m_8214qf_8295r_C45_demo.c>
  #include <hwp/hw_profiles/rtl8396m_8214qf_8295r_es_demo.c>
  #include <hwp/hw_profiles/rtl8391m_demo.c>
  #include <hwp/hw_profiles/rtl8391m_14c_demo.c>
  #include <hwp/hw_profiles/rtl8392m_demo.c>
  #include <hwp/hw_profiles/rtl8393m_demo.c>
  #include <hwp/hw_profiles/rtl8396m_demo.c>
  #include <hwp/hw_profiles/rtl8353m_demo.c>
  #include <hwp/hw_profiles/rtl8353m_qa.c>
  #include <hwp/hw_profiles/rtl8353m_14b_demo.c>
  #include <hwp/hw_profiles/rtl8351m_demo.c>
#endif /* CONFIG_SDK_RTL8390 */

#if defined(CONFIG_SDK_RTL8380)
  #include <hwp/hw_profiles/rtl8382m_8218b_intphy_8218b_8214fc_demo.c>
  #include <hwp/hw_profiles/rtl8382m_8218d_intphy_8218d_8214fc_demo.c>
  #include <hwp/hw_profiles/rtl8382m_8218b_intphy_8218b_2fib_1g_demo.c>
  #include <hwp/hw_profiles/rtl8382m_8218b_intphy_8218b_8214b_demo.c>
  #include <hwp/hw_profiles/rtl8382m_8218b_intphy_8218fb_demo.c>
  #include <hwp/hw_profiles/rtl8380m_intphy_2fib_1g_demo.c>
  #include <hwp/hw_profiles/rtl8332m_8208l_intphy_8208l_8214b_demo.c>
  #include <hwp/hw_profiles/rtl8332m_8208l_intphy_8208l_8214c_demo.c>
  #include <hwp/hw_profiles/rtl8330m_intphy_2fib_1g_demo.c>
  #include <hwp/hw_profiles/rtl8330m_intphy_8212b_2ge_demo.c>
  #include <hwp/hw_profiles/rtl8330m_intphy_8212b_demo.c>
  #include <hwp/hw_profiles/rtl8381m_intphy_8214c_2fib_1g_demo.c>
  #include <hwp/hw_profiles/rtl8381m_intphy_8214fc_2fib_1g_demo.c>
#endif /* CONFIG_SDK_RTL8380 */

#if defined(CONFIG_SDK_RTL9310)
    #include <hwp/hw_profiles/rtl9311_6x8218d_6xge.c>
    #include <hwp/hw_profiles/rtl9313_12xge.c>
#endif  /* CONFIG_SDK_RTL9310 */

#include <hwp/hw_profile_internal.c>


/*
 * Data Declaration
 */

/* list of supported hardware profiles */
const hwp_hwProfile_t *hwp_hwProfile_list[] =
{
#if defined(CONFIG_SDK_RTL9300)
    &rtl9301_2x8214qf_4xge,
    &rtl9301_3x8218b_4xge,
    &rtl9301_8218b_4xge,
    &rtl9301_8218b_4xge_cascade,
    &rtl9301_2x8214qf_4xge_cascade,
    &rtl9301_14qf_4xge_18b_4xge_cascade,
    &rtl9301_3x8218d_4xge,
    &rtl9301_6x8214qf_4xge,
    &rtl9301_6x8218d_2x8295r_cascade,
    &rtl9301_2x8218b_4x8218d_2x8295r_cascade,
    &rtl9302b_2x8218d_2xcust1_4xge,
    &rtl9302c_4xcust1,
    &rtl9303_2xcust1,
    &rtl9303_8xge,
#endif

#if defined(CONFIG_SDK_RTL8390)
    &rtl8396m_8218b_8214qf_8295r_c45_demo,
    &rtl8396m_8218b_8214qf_8295r_demo,
    &rtl8396m_8214qf_8295r_c45_demo,
    &rtl8396m_8214qf_8295r_es_demo,
    &rtl8391m_demo,
    &rtl8391m_14c_demo,
    &rtl8393m_demo,
    &rtl8396m_demo,
    &rtl8353m_demo,
    &rtl8353m_qa,
    &rtl8353m_8214b_demo,
    &rtl8351m_demo,
    &rtl8392m_demo,
#endif

#if defined(CONFIG_SDK_RTL8380)
    &rtl8382m_8218b_intphy_8218b_8214fc_demo,
    &rtl8382m_8218d_intphy_8218d_8214fc_demo,
    &rtl8382m_8218b_intphy_8218fb_demo,
    &rtl8382m_8218b_intphy_8218b_8214b_demo,
    &rtl8382m_8218b_intphy_8218b_2fib_1g_demo,
    &rtl8381m_intphy_8214fc_2fib_1g_demo,
    &rtl8381m_intphy_8214c_2fib_1g_demo,
    &rtl8380m_intphy_2fib_1g_demo,
    &rtl8332m_8208l_intphy_8208l_8214b_demo,
    &rtl8332m_8208l_intphy_8208l_8214c_demo,
    &rtl8330m_intphy_8212b_demo,
    &rtl8330m_intphy_2fib_1g_demo,
    &rtl8330m_intphy_8212b_2ge_demo,
#endif
#if defined(CONFIG_SDK_RTL9310)
    &rtl9311_6x8218d_6xge,
    &rtl9313_12xge,
#endif  /* CONFIG_SDK_RTL9310 */
    RTK_INTERNAL_PROFILE
    NULL,
}; /* end hwp_hwProfile_list */


