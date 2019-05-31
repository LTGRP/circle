#
# Makefile
#



OBJS	= gfx_mk.o

LIBS	= $(CIRCLE_PATH)/lib/libcircle.a

libugfx.a: $(OBJS)
	rm -f $@
	$(AR) cr $@ $(OBJS)

include $(CIRCLE_PATH)/Rules.mk
