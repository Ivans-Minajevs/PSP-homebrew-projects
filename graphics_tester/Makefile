TARGET = graphics
OBJS = main.o

CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)



EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Graphics tester
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

LIBS = -lpspdebug -lpspgum -lpspgu -lpspdisplay -lpspge -lpspctrl -lc -lm -lpspuser