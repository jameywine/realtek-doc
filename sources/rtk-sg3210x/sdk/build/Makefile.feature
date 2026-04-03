#
# Copyright (C) 2020 Realtek Semiconductor Corp.
#
# Makefile for SDK Feature Module
#

#
# include configuration file
#
sinclude $(SDK_CONFIG)


FEATURE_ALL_OBJ :=

ifeq ($(CONFIG_SDK_MINISDK_NET),y)
    FEATURE_ALL_OBJ += feature_net.o
endif

ifeq ($(CONFIG_SDK_MINISDK_PHY),y)
    FEATURE_ALL_OBJ += feature_phy.o
endif

