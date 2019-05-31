#
# Makefile
#


OBJS	= main.o kernel.o

LIBS	= $(CIRCLE_PATH)/lib/libcircle.a

EXTRACLEAN = sound.h

include $(CIRCLE_PATH)/Rules.mk

kernel.o: sound.h

sound.h: sound.h.gz
	gunzip -c sound.h.gz > sound.h
