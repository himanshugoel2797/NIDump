#
# Copyright (c) 2015 Sergi Granell (xerpi)
# based on Cirne's vita-toolchain test Makefile
#

TARGET = nidump
OBJS   = main.o draw.o font_data.o ModuleUtils.o menuAPI.o utils.o

STUBS = libSceIofilemgr.a libSceModulemgr.a libSceThreadmgr.a libSceDisplay.a libSceGxm.a libSceSysmem.a libSceCtrl.a

NIDS_DB = sample-db.json

PREFIX  = arm-none-eabi
CC      = $(PREFIX)-gcc
READELF = $(PREFIX)-readaelf
OBJDUMP = $(PREFIX)-objdump
CFLAGS  = -Wall -std=c99 -nostartfiles -nostdlib -I$(PSP2SDK)/include

STUBS_FULL = $(addprefix stubs/, $(STUBS))

all: $(TARGET).velf

$(TARGET).velf: $(TARGET).elf
	$(PSP2SDK)/bin/vita-elf-create $(TARGET).elf $(TARGET).velf $(NIDS_DB)

$(TARGET).elf: $(OBJS) $(STUBS_FULL)
	$(CC) -Wl,-q -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(STUBS_FULL):
	@mkdir -p stubs
	$(PSP2SDK)/bin/vita-libs-gen $(NIDS_DB) stubs/
	$(MAKE) -C stubs $(notdir $@)

clean:
	@rm -rf $(TARGET).elf $(TARGET).velf $(OBJS) stubs

copy: $(TARGET).velf
	@cp $(TARGET).velf ~/shared/vitasample.elf
