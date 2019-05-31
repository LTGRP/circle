#
# Makefile
#





#LIBS	= $(CIRCLE_PATH)/lib/libcircle.a


include lvgl/lv_core/lv_core.mk
include lvgl/lv_hal/lv_hal.mk
include lvgl/lv_objx/lv_objx.mk
include lvgl/lv_misc/lv_fonts/lv_fonts.mk
include lvgl/lv_misc/lv_misc.mk
include lvgl/lv_themes/lv_themes.mk
include lvgl/lv_draw/lv_draw.mk


STDLIB_SUPPORT ?= 1

# set this to "softfp" if you want to link specific libraries
FLOAT_ABI ?= hard
RASPPI	?= 3
PREFIX	?= arm-none-eabi-
CC	= $(PREFIX)gcc
CPP	= $(PREFIX)g++
AS	= $(CC)
LD	= $(PREFIX)ld
AR	= $(PREFIX)ar


ifeq ($(strip $(RASPPI)),1)
ARCH	?= -march=armv6k -mtune=arm1176jzf-s -marm -mfpu=vfp -mfloat-abi=$(FLOAT_ABI)
TARGET	?= kernel
else ifeq ($(strip $(RASPPI)),2)
ARCH	?= -march=armv7-a -marm -mfpu=neon-vfpv4 -mfloat-abi=$(FLOAT_ABI)
TARGET	?= kernel7
else
ARCH	?= -march=armv8-a -mtune=cortex-a53 -marm -mfpu=neon-fp-armv8 -mfloat-abi=$(FLOAT_ABI)
TARGET	?= kernel8-32
endif

ifeq ($(strip $(STDLIB_SUPPORT)),3)
LIBSTDCPP != $(CPP) $(ARCH) -print-file-name=libstdc++.a
EXTRALIBS += $(LIBSTDCPP)
LIBGCC_EH != $(CPP) $(ARCH) -print-file-name=libgcc_eh.a
ifneq ($(strip $(LIBGCC_EH)),libgcc_eh.a)
EXTRALIBS += $(LIBGCC_EH)
endif
else
CPPFLAGS  += -fno-exceptions -fno-rtti -nostdinc++
endif

ifeq ($(strip $(STDLIB_SUPPORT)),0)
CFLAGS	  += -nostdinc
else
LIBGCC	  != $(CPP) $(ARCH) -print-file-name=libgcc.a
EXTRALIBS += $(LIBGCC)
endif

OPTIMIZE ?= -O2

INCLUDE	+= -I $(CIRCLE_PATH)/include -I $(CIRCLE_PATH)/addon -I $(CIRCLE_PATH)/app/lib -I $(CIRCLE_PATH)/app/lib/core -I ./ -I $(CIRCLE_PATH)/addon/lvgl

AFLAGS	+= $(ARCH) -DRASPPI=$(RASPPI) -DSTDLIB_SUPPORT=$(STDLIB_SUPPORT) $(INCLUDE)
CFLAGS	+= $(ARCH) -Wall -fsigned-char -ffreestanding \
	   -D__circle__ -DRASPPI=$(RASPPI) -DSTDLIB_SUPPORT=$(STDLIB_SUPPORT) \
	   $(INCLUDE) $(OPTIMIZE) -g #-DNDEBUG

OBJEXT ?= .o

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))



SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

liblvgl.a: $(OBJS)
	rm -f $@
	$(AR) cr $@ $(OBJS)

all: clean default

%.o: %.c
	@$(CC)  $(CFLAGS) -c $< -o $@
	@echo "CC $<"
	@echo "CFLAGS $<"

clean: 
	rm -f $(BIN) $(AOBJS) $(COBJS) $(MAINOBJ)


