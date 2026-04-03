deps_config := \
	./Kconfig.sdk \
	Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
