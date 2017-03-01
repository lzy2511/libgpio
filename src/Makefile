default:all

all:tips lib

tips:
	-@echo  "\033[32m Building libgpio ... \033[0m"

ifndef TOP_DIR
TOP_DIR = $(shell pwd)/../../
endif

include ../env.mk

INC+= -I../include

LD:= -lpthread

CFLAGS+=$(INC)
LDFLAGS+=$(LD)

lib:gpio.o 
	-@mkdir -p ../lib
	$(Q)$(AR) rucs ../lib/libgpio.a $^ 
	@$(E) "  AR " $<
	$(Q)$(CC) -shared -fPIC -o ../lib/libgpio.so $^
	@$(E) "  Shared " $<

%.o:%.c
	$(Q)$(CC) -g -c $(CFLAGS) $< -o $@
	@$(E) "  CC " $<

clean:
	-@$(RM) *.o *.a ../lib/*.a ../lib/*.so

install:
	-@install -v ../lib/*.a $(DST_DIR)
	-@install -v ../lib/*.so $(DST_DIR)

.PHONY:clean
