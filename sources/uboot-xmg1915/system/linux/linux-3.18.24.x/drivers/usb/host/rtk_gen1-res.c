/*
 * Copyright 2012, Realtek Semiconductor Corp.
 *
 * drivers/usb/host/rtk_gen1-res.c
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
#include <bspchip.h>
//#include "platform.h"


MODULE_DESCRIPTION("Realtek USB platform device driver");
MODULE_LICENSE("GPL");


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
        .start	= BSP_USB_H2_IRQ, /* BSP_IRQ_ICTL_BASE + USB_H2_IRQ */
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

