############################################################################
SDK_AUTOCONG_H=autoconf.h
SDK_INC_COMMON := ./include/configs
SDK_SCRIPTSDIR := scripts

HOSTCC       = gcc
HOSTCXX      = g++
HOSTCFLAGS   = -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu89
HOSTCXXFLAGS = -O2
ifeq ($(shell $(HOSTCC) -v 2>&1 | grep -c "clang version"), 1)
	HOSTCFLAGS  += -Wno-unused-value -Wno-unused-parameter \
		                       -Wno-missing-field-initializers -fno-delete-null-pointer-checks
endif

KCONFIG_CONFIG       = ../.config

export HOSTCC
export HOSTCFLAGS
export HOSTCXX
export HOSTCXXFLAGS
export KCONFIG_CONFIG

all:

clean:
	@rm -f .config .config.old $(SDK_INC_COMMON)/$(SDK_AUTOCONG_H)
	@rm -rf include
	#@make -f ./$(SDK_SCRIPTSDIR)/Makefile.clean __clean

oldconfig:

config:
	@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build obj=$(SDK_SCRIPTSDIR)/basic
	#@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build obj=$(SDK_SCRIPTSDIR)/kconfig config
	@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build obj=$(SDK_SCRIPTSDIR)/kconfig oldconfig
	@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build obj=$(SDK_SCRIPTSDIR)/kconfig silentoldconfig

menuconfig:
	@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build obj=$(SDK_SCRIPTSDIR)/basic
	@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build obj=$(SDK_SCRIPTSDIR)/kconfig menuconfig
	@make -f ./$(SDK_SCRIPTSDIR)/Makefile.build obj=$(SDK_SCRIPTSDIR)/kconfig silentoldconfig

sdkconfig: menuconfig

############################################################################
