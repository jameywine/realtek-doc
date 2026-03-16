

/*Default parameter*/
#if (defined(CONFIG_SDK_RTL8295R) && defined(CONFIG_SDK_RTL8390))
phy_8295_pageRegVal_t rtl8295r_8390_s18431_default[] = {
    {957, 23, 0x0342},
    {958, 16, 0x01E4},
};

phy_8295_pageRegVal_t rtl8295r_8390_s1dacLong_default[] = {
    {957, 23, 0x0742},
    {958, 16, 0x1700},
};

phy_8295_rxCaliConf_t rtl8295r_8390_rxCaliConf_default =
        {
            .s1rxCaliDacLongCableOffset = 3,
        };
#endif /* (defined(CONFIG_SDK_RTL8295R) && defined(CONFIG_SDK_RTL8390)) */

#if (defined(CONFIG_SDK_RTL8295R) && defined(CONFIG_SDK_RTL9300))
phy_8295_pageRegVal_t rtl8295r_9300_s18431_default[] = {
    {957, 23, 0x0342},
    {958, 16, 0x01E4},
};

phy_8295_pageRegVal_t rtl8295r_9300_s1dacLong_default[] = {
    {957, 23, 0x0742},
    {958, 16, 0x1700},
};

phy_8295_rxCaliConf_t rtl8295r_9300_rxCaliConf_default =
        {
            .s1rxCaliDacLongCableOffset = 3,
        };
#endif /* (defined(CONFIG_SDK_RTL8295R) && defined(CONFIG_SDK_RTL9300)) */



/*Customize parameter*/
#if (defined(CONFIG_SDK_RTL8295R) && defined(CONFIG_SDK_RTL8390))
phy_8295_pageRegVal_t rtl8295r_8390_s18431_myParam[] = {
    {957, 23, 0x0342},
    {958, 16, 0x01E4},
};
phy_8295_pageRegVal_t rtl8295r_8390_s1dacLong_myParam[] = {
    {957, 23, 0x0742},
    {958, 16, 0x1700},
};

phy_8295_rxCaliConf_t rtl8295r_8390_rxCaliConf_myParam =
        {
            .s1rxCaliDacLongCableOffset = 3,
        };
#endif

#if (defined(CONFIG_SDK_RTL8295R) && defined(CONFIG_SDK_RTL9300))
phy_8295_pageRegVal_t rtl8295r_9300_s18431_myParam[] = {
    {957, 23, 0x0342},
    {958, 16, 0x01E4},
};
phy_8295_pageRegVal_t rtl8295r_9300_s1dacLong_myParam[] = {
    {957, 23, 0x0742},
    {958, 16, 0x1700},
};

phy_8295_rxCaliConf_t rtl8295r_9300_rxCaliConf_myParam =
        {
            .s1rxCaliDacLongCableOffset = 3,
        };
#endif

