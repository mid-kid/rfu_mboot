CC = thumb-elf-gcc
AS = thumb-elf-as
OBJCOPY = thumb-elf-objcopy

.SFILES    =   crt0_multi_boot.s
.CFILES    =   
.OFILES    =   $(.SFILES:.s=.o) $(.CFILES:.c=.o)

AGBINC	=	$(AGBDIR)/include
AGBLIB	=	$(AGBDIR)/lib

ASFLAGS    =          -I$(AGBINC) -mthumb-interwork
CFLAGS     =   -g -O2 -I$(AGBINC) -mthumb-interwork \
               -nostdlib #-DNDEBUG
LDFLAGS    +=  -Map $(MAPFILE) -nostartfiles \
               -Ttext 0x02000000 -Tbss 0x03000000 \
               -L$(AGBLIB) -lagbsyscall -lisagbprn

MAPFILE    =   client.map
TARGET_ELF =   client.elf
TARGET_BIN =   client.bin

COMP_OFILE =   demo_LZ.o
COMP_BIN   =   demo_LZ.bin

DEMO_BIN   =   demo.bin


default:
	@$(MAKE) -f MakefileDemo
	@$(MAKE) -f Makefile $(TARGET_BIN)
	@$(MAKE) -f Makefile compare


$(TARGET_BIN): $(TARGET_ELF)
	$(OBJCOPY) -v -O binary $< $@

$(TARGET_ELF): $(.OFILES) Makefile $(COMP_OFILE)
	@echo > $(MAPFILE)
	$(CC) -g -o $@ $(.OFILES) $(COMP_OFILE) -Wl,$(LDFLAGS)

$(COMP_OFILE): $(COMP_BIN)
	$(OBJCOPY) -I binary -O elf32-little $< $@

$(COMP_BIN): $(DEMO_BIN)
	agbcomp -b -l $<


.PHONY: all compare clean depend


all:    clean default

compare:
	md5sum -c client.md5

clean:
	@$(MAKE) -f MakefileDemo clean
	-rm $(.OFILES) $(MAPFILE) $(TARGET_ELF) $(TARGET_BIN) \
				$(COMP_OFILE) $(COMP_BIN)

depend:
	@$(MAKE) -f MakefileDemo depend


include Gasdepend
