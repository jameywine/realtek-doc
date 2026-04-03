
#ifdef __BOOTLOADER__
#include <uboot/uboot_util.h>
#if (UBOOT_VER_CALC(U_BOOT_VERSION_NUM, U_BOOT_VERSION_NUM_PATCH) < UBOOT_VER_CALC(2014, 10))
  #include <configs/autoconf.h>
#else
  #include <generated/autoconf.h>
#endif
#else
  #include <common/sdk_autoconf.h>
#endif

