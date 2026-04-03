############################################################################
SDK_AUTOCONG_H=sdk_autoconf.h
SDK_INC_COMMON := ../system/include/common
SDK_SCRIPTSDIR := scripts
MAIN_KCONFIG ?=
MAIN_DOTCONFIG ?=

HOSTCC       = gcc
HOSTCXX      = g++
HOSTCFLAGS   = -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu89
HOSTCXXFLAGS = -O2
ifeq ($(shell $(HOSTCC) -v 2>&1 | grep -c "clang version"), 1)
	HOSTCFLAGS  += -Wno-unused-value -Wno-unused-parameter \
		                       -Wno-missing-field-initializers -fno-delete-null-pointer-checks
endif

export HOSTCC
export HOSTCFLAGS
export HOSTCXX
export HOSTCXXFLAGS

all:

clean:
	@rm -f .config .config.old $(SDK_INC_COMMON)/$(SDK_AUTOCONG_H)
	@rm -rf include
	#@make -f ./$(SDK_SCRIPTSDIR)/Makefile.clean __clean

oldconfig:

config:
	@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build obj=$(SDK_SCRIPTSDIR)/basic
	#@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build obj=$(SDK_SCRIPTSDIR)/kconfig config
	#@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build obj=$(SDK_SCRIPTSDIR)/kconfig oldconfig
	@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build obj=$(SDK_SCRIPTSDIR)/kconfig silentoldconfig
	@cp ./include/generated/autoconf.h $(SDK_INC_COMMON)/$(SDK_AUTOCONG_H)

menuconfig:
	@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build obj=$(SDK_SCRIPTSDIR)/basic
	@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build obj=$(SDK_SCRIPTSDIR)/kconfig menuconfig
	@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build obj=$(SDK_SCRIPTSDIR)/kconfig silentoldconfig
	@cp ./include/generated/autoconf.h $(SDK_INC_COMMON)/$(SDK_AUTOCONG_H)

main_menuconfig:
	@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build KBUILD_KCONFIG=$(MAIN_KCONFIG) KCONFIG_CONFIG=$(MAIN_DOTCONFIG) obj=$(SDK_SCRIPTSDIR)/basic
	@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build KBUILD_KCONFIG=$(MAIN_KCONFIG) KCONFIG_CONFIG=$(MAIN_DOTCONFIG) obj=$(SDK_SCRIPTSDIR)/kconfig menuconfig

main_config:
	@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build KBUILD_KCONFIG=$(MAIN_KCONFIG) KCONFIG_CONFIG=$(MAIN_DOTCONFIG) obj=$(SDK_SCRIPTSDIR)/basic
	@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build KBUILD_KCONFIG=$(MAIN_KCONFIG) KCONFIG_CONFIG=$(MAIN_DOTCONFIG) obj=$(SDK_SCRIPTSDIR)/kconfig oldconfig


sdkconfig: menuconfig

############################################################################

