CC = thumb-elf-gcc
AS = thumb-elf-as
OBJCOPY = thumb-elf-objcopy

.SFILES    =   crt0.s
.CFILES    =
.OFILES    =   $(.SFILES:.s=.o) $(.CFILES:.c=.o)

AGBINC	   =	$(AGBDIR)/include
AGBLIB	   =	$(AGBDIR)/lib

ASFLAGS    =          -I$(AGBINC) -mthumb-interwork
CFLAGS     =   -g -O2 -I$(AGBINC) -mthumb-interwork \
               -nostdlib #-DNDEBUG
LDFLAGS    +=  -Map $(MAPFILE) -nostartfiles \
               -Ttext 0x08000000 -Tbss 0x03000000 \
               -L$(AGBLIB) -lagbsyscall

DEPENDFILE   =   multi_boot.dep
MAPFILE      =   multi_boot.map
TARGET_ELF   =   multi_boot.elf
TARGET_BIN   =   multi_boot.bin
		
CLIENT_OFILE =   demo_LZ.o


default:
	@$(MAKE) -f MakefileClient
	@$(MAKE) $(TARGET_BIN)


$(TARGET_BIN): $(TARGET_ELF)
	$(OBJCOPY) -v -O binary $< $@

$(TARGET_ELF): $(.OFILES) Makefile $(DEPENDFILE) $(CLIENT_OFILE)
	@echo > $(MAPFILE)
	$(CC) -g -o $@ $(.OFILES) $(CLIENT_OFILE) -Wl,$(LDFLAGS)


.PHONY: all clean depend


all:    clean default

clean:
	@$(MAKE) -f MakefileClient clean
	-rm $(.OFILES) $(DEPENDFILE) $(MAPFILE) $(TARGET_ELF) $(TARGET_BIN)

depend:
	@$(MAKE) -f MakefileClient depend
	$(CC) $(CFLAGS) -M $(.CFILES) > $(DEPENDFILE)

$(DEPENDFILE): 
	true $(CC) $(CFLAGS) -M $(.CFILES) > $(DEPENDFILE)

include Gasdepend
include $(DEPENDFILE)
