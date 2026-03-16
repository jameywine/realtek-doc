/*
 * Copyright 2012, Realtek Semiconductor Corp.
 *
 * drivers/usb/host/rtl9300-res.c
 *
 * $Author: cathy $
 *
 * USB resource for EHCI hcd
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
//#include <bspchip.h>
#ifdef CONFIG_PCI
#include <linux/dma-mapping.h>
#endif
#include <platform.h>

MODULE_DESCRIPTION("Realtek USB platform device driver");
MODULE_LICENSE("GPL");


#define USB2_PHY_DELAY __delay(200)

#if defined(CONFIG_USB_EHCI_HCD) || defined(CONFIG_USB_EHCI_HCD_MODULE)
static void usb_release(struct device *dev)
{
    /* normally not freed */
    dev->parent = NULL;
}

#endif

#if defined(CONFIG_USB_EHCI_HCD) || defined(CONFIG_USB_EHCI_HCD_MODULE)
static struct resource rtk_gen1_ehci_resources[] = {
    {
        #ifndef CONFIG_PCI
        .start  = RTK_EHCI_BASE,
        .end    = RTK_EHCI_BASE + 0x0000EFFF,
        #else
        .start	= PADDR(RTK_EHCI_BASE),
        .end	= PADDR(RTK_EHCI_BASE) + 0x0000EFFF,
        #endif
        .flags	= IORESOURCE_MEM,
    },
    {
        .start	= USB_H2_IRQ,
        .flags	= IORESOURCE_IRQ,
    },
};

static struct platform_device rtk_gen1_ehci = {
    .name = "rtk_gen1-ehci",
    .id	= -1,
    .dev = {
        .release = usb_release,
        #ifndef CONFIG_PCI
        .dma_mask = (void *)~0,
        .coherent_dma_mask = ~0,
        #else
        .dma_mask = &rtk_gen1_ehci_dmamask,
       .coherent_dma_mask = DMA_BIT_MASK(32),
        #endif
    },
    .num_resources = ARRAY_SIZE(rtk_gen1_ehci_resources),
    .resource = rtk_gen1_ehci_resources,
};
#endif //CONFIG_USB_EHCI_HCD || CONFIG_USB_EHCI_HCD_MODULE

void ehci_phy_write(unsigned char phy_port, unsigned char reg, unsigned char val)
{
    unsigned int tmp = REG32(RTK_USB_PHY_CTRL);

    if (phy_port != 1) {
        printk("EHCI: phy port should be 1!\n");
        return;
    }

    if ((reg < 0xE0) || (reg > 0xF6) || ((reg>0xE7)&&(reg<0xF0))) {
        printk("EHCI: Wrong register address: 0x%02x\n", reg);
        return;
    }

    //ehci macPort 0 at PHY1
    tmp = tmp & 0xFF00FF00;
    REG32(RTK_USB_PHY_CTRL) = (val << 16) | tmp; USB2_PHY_DELAY;
    REG32(BSP_EHCI_UTMI_CTRL) = ((reg & 0x0F) << 16) | 0x00300000; USB2_PHY_DELAY;
    REG32(BSP_EHCI_UTMI_CTRL) = ((reg & 0x0F) << 16) | 0x00200000; USB2_PHY_DELAY;
    REG32(BSP_EHCI_UTMI_CTRL) = ((reg & 0x0F) << 16) | 0x00300000; USB2_PHY_DELAY;
    REG32(BSP_EHCI_UTMI_CTRL) = ((reg & 0xF0) << 12) | 0x00300000; USB2_PHY_DELAY;
    REG32(BSP_EHCI_UTMI_CTRL) = ((reg & 0xF0) << 12) | 0x00200000; USB2_PHY_DELAY;
    REG32(BSP_EHCI_UTMI_CTRL) = ((reg & 0xF0) << 12) | 0x00300000; USB2_PHY_DELAY;

    return;
}
EXPORT_SYMBOL(ehci_phy_write);

unsigned char ehci_phy_read(unsigned char phy_port, unsigned char reg)
{
    unsigned char val;

    if (phy_port != 1) {
        printk("EHCI: phy port should be 1!\n");
        return 0;
    }

    if ((reg < 0xE0) || (reg > 0xF6) || ((reg>0xE7)&&(reg<0xF0))) {
        printk("EHCI: Wrong register address: 0x%02x\n", reg);
        return 0;
    }

    reg = reg - 0x20;

    //ehci mac port 0 at PHY1
    REG32(BSP_EHCI_UTMI_CTRL) = ((reg & 0x0F) << 16) | 0x00300000; USB2_PHY_DELAY;
    REG32(BSP_EHCI_UTMI_CTRL) = ((reg & 0x0F) << 16) | 0x00200000; USB2_PHY_DELAY;
    REG32(BSP_EHCI_UTMI_CTRL) = ((reg & 0x0F) << 16) | 0x00300000; USB2_PHY_DELAY;
    REG32(BSP_EHCI_UTMI_CTRL) = ((reg & 0xF0) << 12) | 0x00300000; USB2_PHY_DELAY;
    REG32(BSP_EHCI_UTMI_CTRL) = ((reg & 0xF0) << 12) | 0x00200000; USB2_PHY_DELAY;
    REG32(BSP_EHCI_UTMI_CTRL) = ((reg & 0xF0) << 12) | 0x00300000; USB2_PHY_DELAY;

    val = (REG32(BSP_EHCI_UTMI_CTRL) & 0xFF000000) >> 24;
    printk("EHCI: phy port %d, phy(0x%02x) = 0x%02x\n\n", phy_port, reg + 0x20, val);

    return val;
}
EXPORT_SYMBOL(ehci_phy_read);


int rtk_gen1_hcd_cs_init (void)
{
    int retval = 0;
    printk("%s: rtk_gen1_hcd_cs_init()!\n", __FUNCTION__);

#if defined(CONFIG_USB_EHCI_HCD) || defined(CONFIG_USB_EHCI_HCD_MODULE)
    retval = platform_device_register(&rtk_gen1_ehci);
    if (retval) {
        printk("%s: fail to register rtk_gen1_ehci! (%d [%x])\n", __FUNCTION__,retval,retval);
        return retval;
    }
    printk("%s: register rtk_gen1_ehci ok!\n", __FUNCTION__);
#endif //CONFIG_USB_EHCI_HCD || CONFIG_USB_EHCI_HCD_MODULE

    return retval;
}
//module_init (rtk_gen1_hcd_cs_init);

static void __exit rtk_gen1_hcd_cs_exit(void)
{

#if defined(CONFIG_USB_EHCI_HCD) || defined(CONFIG_USB_EHCI_HCD_MODULE)
    platform_device_unregister(&rtk_gen1_ehci);
#endif //CONFIG_USB_EHCI_HCD || CONFIG_USB_EHCI_HCD_MODULE
    return;
}
module_exit(rtk_gen1_hcd_cs_exit);

