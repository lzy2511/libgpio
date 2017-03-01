CROSS_COMPILE=/opt/buildroot-master/output/host/usr/bin/arm-linux-
CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar
INC=-I$(TOP_DIR)/libgpio

ifeq ($(V), 1)
	Q=
	E=true
else 
	Q=@
	E=echo
endif

export CC AR 

