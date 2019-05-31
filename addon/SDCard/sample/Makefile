#
# Makefile
#

OBJS	= main.o kernel.o

LIBS	= ../libsdcard.a \
	  $(CIRCLE_PATH)/lib/fs/fat/libfatfs.a \
	  $(CIRCLE_PATH)/lib/fs/libfs.a \
	  $(CIRCLE_PATH)/lib/libcircle.a

include $(CIRCLE_PATH)/Rules.mk
