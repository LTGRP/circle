#
# Makefile
#

CIRCLEHOME = ../../..

OBJS	= Arduino.o main.o

libarduino.a: $(OBJS)
	rm -f $@
	$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
