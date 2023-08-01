CC := ThumbPatch-thumb-elf-gcc
AR := ThumbPatch-thumb-elf-ar

.SFILES    =   
.CFILES    =   AgbRFU_sio32intr.c AgbRFU_STWI.c AgbRFU_Sio32ID.c AgbRFU_LL.c
.OFILES    =   $(.SFILES:.s=.o) $(.CFILES:.c=.o)

AGBINC	=	$(AGBDIR)/include
AGBLIB	=	$(AGBDIR)/lib

ASFLAGS    =          -I$(AGBINC) -mthumb-interwork
CFLAGS     =   -g -O2 -I$(AGBINC) -mthumb-interwork \
               -nostdlib -Wall -Werror #-DNDEBUG

DEPENDFILE =   libagbrfu.dep
TARGET_LIB =   libagbrfu.a


$(TARGET_LIB): $(.OFILES) Makefile $(DEPENDFILE)
	$(AR) rcs $@ $(.OFILES)

AgbRFU_sio32intr.o: CC := arm-elf-gcc
AgbRFU_Sio32ID.o: CFLAGS += -Wno-uninitialized


.PHONY: all clean depend


all:    clean $(TARGET_LIB)

clean:
	-rm $(.OFILES) $(DEPENDFILE) $(TARGET_LIB)

depend:
	$(CC) $(CFLAGS) -M $(.CFILES) > $(DEPENDFILE)

$(DEPENDFILE): 
	$(CC) $(CFLAGS) -M $(.CFILES) > $(DEPENDFILE)

include $(DEPENDFILE)
