#
# Makefile
#

CPPFLAGS += -Wno-write-strings

OBJS	= main.o kernel.o controlwindow.o scopewindow.o scopeconfig.o timeline.o recorder.o sampler.o

LIBS	= ../libugui.a \
	  $(CIRCLE_PATH)/lib/usb/libusb.a \
	  $(CIRCLE_PATH)/lib/input/libinput.a \
	  $(CIRCLE_PATH)/lib/fs/libfs.a \
	  $(CIRCLE_PATH)/lib/libcircle.a

include $(CIRCLE_PATH)/Rules.mk
