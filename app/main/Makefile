#
# Makefile
#

OBJS	= kernel.o Engine0.o
	#controlwindow.o \
	#scopewindow.o

LIBS	= $(CIRCLE_PATH)/lib/libcircle.a \
	  $(CIRCLE_PATH)/lib/usb/libusb.a \
	  $(CIRCLE_PATH)/lib/input/libinput.a \
	  $(CIRCLE_PATH)/lib/fs/libfs.a \
	  $(CIRCLE_PATH)/app/lib/core/libarduino.a \
	  $(CIRCLE_PATH)/addon/lvgl/liblvgl.a
	  #$(CIRCLE_PATH)/addon/ugui/libugui.a

include ../../Rules.mk
