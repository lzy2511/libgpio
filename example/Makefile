default:all

all:tips test_gpio

tips:
	-@echo  "\033[32m Building gpio example ... \033[0m"

ifndef TOP_DIR
TOP_DIR = $(shell pwd)/../../
endif

include ../env.mk

INC+= -I../include

LD:= -L../lib -lgpio -lpthread

CFLAGS+=$(INC)
LDFLAGS+=$(LD)

test_gpio:test.o
	$(Q)$(CC) $^ -o $@ $(LD)
	@$(E) "  Gen " $@

%.o:%.c
	$(Q)$(CC) -g -c $(CFLAGS) $< -o $@
	@$(E) "  CC " $<

clean:
	-@$(RM) *.o test_gpio

install:
	-@echo ""

.PHONY:clean
