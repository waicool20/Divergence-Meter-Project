###############################################################################
#
# AVR MAKEFILE TEMPLATE BY WAICOOL20
#
###############################################################################
#
# PROJECT OPTIONS
#
# PROJECT_NAME: 					Name of project, used for final output file names
# SOURCE:									List source files here with extension e.g. test.c
# MCU:										Target microcontroller
# MCU_FREQ:								Microcontroller clock frequency in Hz 
#
###############################################################################
PROJECT_NAME = Divergence_Meter
CSOURCE = Divergence_Meter.c ShiftRegister.c Display.c
ASMSOURCE = i2cmaster.S
MCU = attiny861a
MCU_FREQ = 8000000UL

###############################################################################
#
# PROGRAMMING OPTIONS
#
# AVRDUDE: 								Command to execute avrdude
# AVRDUDE_PROGRAMMER:			Programming hardware you use
# AVRDUDE_PORT:						Port used for programming
# AVRDUDE_PROCESSOR:			Processor family of MCU (Could be same name as MCU,
#													its here just in case)
#
###############################################################################
AVRDUDE = avrdude
AVRDUDE_PROCESSOR = t861
AVRDUDE_PROGRAMMER = pickit2
AVRDUDE_PORT = usb

AVRDUDE_GENERAL_COMMAND = "$(AVRDUDE) -c $(AVRDUDE_PROGRAMMER) -P $(AVRDUDE_PORT) -p $(AVRDUDE_PROCESSOR) -v"


###############################################################################
#
# FILENAME DEFINITIONS
#
###############################################################################
ELF_FILE = "$(PROJECT_NAME).elf"
MAP_FILE = "$(PROJECT_NAME).map"
LST_FILE = "$(PROJECT_NAME).lst"

OBJ_FILES = $(CSOURCE:.c=.o) $(ASMSOURCE:.S=.o)

FLASH_FILE = "$(PROJECT_NAME)_FLASH.hex"
EEPROM_FILE = "$(PROJECT_NAME)_EEPROM.hex"
FUSES_FILE = "$(PROJECT_NAME)_FUSES.hex"
LFUSE_FILE = "$(PROJECT_NAME)_LFUSE.hex"
HFUSE_FILE = "$(PROJECT_NAME)_HFUSE.hex"
EFUSE_FILE = "$(PROJECT_NAME)_EFUSE.hex"

BFLASH_FILE = "$(PROJECT_NAME)_FLASH.bin"
BEEPROM_FILE = "$(PROJECT_NAME)_EEPROM.bin"
BFUSES_FILE = "$(PROJECT_NAME)_FUSES.bin"
BLFUSE_FILE = "$(PROJECT_NAME)_LFUSE.bin"
BHFUSE_FILE = "$(PROJECT_NAME)_HFUSE.bin"
BEFUSE_FILE = "$(PROJECT_NAME)_EFUSE.bin"

SFLASH_FILE = "$(PROJECT_NAME)_FLASH.srec"
SEEPROM_FILE = "$(PROJECT_NAME)_EEPROM.srec"
SFUSES_FILE = "$(PROJECT_NAME)_FUSES.srec"
SLFUSE_FILE = "$(PROJECT_NAME)_LFUSE.srec"
SHFUSE_FILE = "$(PROJECT_NAME)_HFUSE.srec"
SEFUSE_FILE = "$(PROJECT_NAME)_EFUSE.srec"


###############################################################################
#
# COMPILER OPTIONS
#
# CC:											Command to execute avr-gcc
# OBJCOPY:								Command to execute avr-objcopy
# OBJDUMP:								Command to execute avr-objdump
# OPT:										Code optimization level [0,1,2,3,s]
# CFLAGS:									Switches for when compiling C
# ASMFLAGS:								Switches for when compiling Assembly
# LDFLAGS:								Switches to pass onto linker
# 
# See "avr-gcc --help" for more information on switches
#
###############################################################################
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
OPT = 3

#C flags
CFLAGS = -g
CFLAGS += -DF_CPU=$(MCU_FREQ)
CFLAGS += -O$(OPT)
CFLAGS += -Wall
CFLAGS += -Werror
CFLAGS += -Wextra
CFLAGS += -std=c99
CFLAGS += -mmcu=$(MCU)

#Assembly flags
ASMFLAGS = -g
ASMFLAGS += -DF_CPU=$(MCU_FREQ) 
ASMFLAGS += -Wall
ASMFLAGS += -Werror
ASMFLAGS += -Wextra
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -mmcu=$(MCU)

#Linker Flags
LDFLAGS = -Wl,-Map,$(MAP_FILE)

###############################################################################
#
# TARGET DEFINITIONS
#
###############################################################################
# Define phony targets (Always run)
.PHONY: basic all elf listings hex bin srec program_all program_eeprom program_flash program_fuses clean


###############################################################################
#
# Build types
#						
# basic:									Only creates hex files
# all:										Create hex,bin and srec files
basic: elf hex listings
all: elf hex bin srec listings


###############################################################################
#
# File format output targets
#
# elf:										ELF file format
# hex:										INTEL HEX file format
# bin:										BINARY file format
# srec:										MOTOROLA S-record file format
#
# AVRDUDE 6.1 above should be able to read all file formats
#
###############################################################################
elf: $(OBJ_FILES)
	$(info Generating elf file...)
	@$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(ELF_FILE) 
	
%.o: %.c
	$(info Generating object file for $<)
	@$(CC) -c $(CFLAGS) $< -o $@
%.o: %.S
	$(info Generating object file for $<)
	@$(CC) -c $(CFLAGS) $< -o $@
	
hex: elf
	$(info Generating hex files...)
	@$(OBJCOPY) -j .text -j .data -O ihex $(ELF_FILE) $(FLASH_FILE)
	$(info Generating Flash hex)
	@$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex $(ELF_FILE) $(EEPROM_FILE)
	$(info Generating EEPROM hex)
	@$(OBJCOPY) -j .fuse --set-section-flags=.fuse="alloc,load" --change-section-lma .fuse=0 -O ihex $(ELF_FILE) $(FUSES_FILE) 
	@srec_cat $(FUSES_FILE) -Intel -crop 0x00 0x01 -offset  0x00 -O $(LFUSE_FILE) -Intel
	@srec_cat $(FUSES_FILE) -Intel -crop 0x01 0x02 -offset -0x01 -O $(HFUSE_FILE) -Intel
	@srec_cat $(FUSES_FILE) -Intel -crop 0x02 0x03 -offset -0x02 -O $(EFUSE_FILE) -Intel
	$(info Generating FUSES hex)

bin: elf
	$(info Generating bin files...)
	@$(OBJCOPY) -j .text -j .data -O binary $(ELF_FILE) $(BFLASH_FILE)
	$(info Generating Flash hex)
	@$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O binary $(ELF_FILE) $(BEEPROM_FILE)
	$(info Generating EEPROM hex)
	@$(OBJCOPY) -j .fuse --set-section-flags=.fuse="alloc,load" --change-section-lma .fuse=0 -O binary $(ELF_FILE) $(BFUSES_FILE) 
	@srec_cat $(BFUSES_FILE) -Binary -crop 0x00 0x01 -offset  0x00 -O $(BLFUSE_FILE) -Binary
	@srec_cat $(BFUSES_FILE) -Binary -crop 0x01 0x02 -offset -0x01 -O $(BHFUSE_FILE) -Binary
	@srec_cat $(BFUSES_FILE) -Binary -crop 0x02 0x03 -offset -0x02 -O $(BEFUSE_FILE) -Binary
	$(info Generating FUSES hex)

srec: elf
	$(info Generating srec files...)
	@$(OBJCOPY) -j .text -j .data -O srec $(ELF_FILE) $(SFLASH_FILE)
	$(info Generating Flash hex)
	@$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O srec $(ELF_FILE) $(SEEPROM_FILE)
	$(info Generating EEPROM hex)
	@$(OBJCOPY) -j .fuse --set-section-flags=.fuse="alloc,load" --change-section-lma .fuse=0 -O srec $(ELF_FILE) $(SFUSES_FILE) 
	@srec_cat $(SFUSES_FILE) -Motorola -crop 0x00 0x01 -offset  0x00 -O $(SLFUSE_FILE) -Motorola
	@srec_cat $(SFUSES_FILE) -Motorola -crop 0x01 0x02 -offset -0x01 -O $(SHFUSE_FILE) -Motorola
	@srec_cat $(SFUSES_FILE) -Motorola -crop 0x02 0x03 -offset -0x02 -O $(SEFUSE_FILE) -Motorola	
	$(info Generating FUSES hex)

	
###############################################################################
#
# Listings targets
#
# Creates listing files
#
###############################################################################
listings: elf
	$(info Generating lst file...)
	@$(OBJDUMP) -h -S -D $(ELF_FILE) > $(LST_FILE)

###############################################################################	
#
# Programming targets
# 
# program_all:						Programs flash,eeprom and fuses. Basically everything
# program_flash:					Programs flash only
# program_eeprom:					Programs eeprom only
# program_fuses:					Program fuses only
#
###############################################################################
program_all: program_flash program_eeprom program_fuses

program_flash: hex
	@"$(AVRDUDE_GENERAL_COMMAND)" -U flash:w:$(FLASH_FILE)

program_eeprom: hex
	@"$(AVRDUDE_GENERAL_COMMAND)" -U eeprom:w:$(EEPROM_FILE)

program_fuses: hex
	@"$(AVRDUDE_GENERAL_COMMAND)" -U lfuse:w:$(LFUSE_FILE)
	@"$(AVRDUDE_GENERAL_COMMAND)" -U hfuse:w:$(HFUSE_FILE)
	@"$(AVRDUDE_GENERAL_COMMAND)" -U efuse:w:$(EFUSE_FILE)


###############################################################################
#
# Cleanup targets
#
# Cleans all those files :c
#
###############################################################################
clean:
	$(info Cleaning up :3)
	@rm -rf $(ELF_FILE) $(MAP_FILE) $(LST_FILE) $(OBJ_FILES) $(FLASH_FILE) $(EEPROM_FILE) $(FUSES_FILE) $(LFUSE_FILE) $(HFUSE_FILE) $(EFUSE_FILE) $(BFLASH_FILE) $(BEEPROM_FILE) $(BFUSES_FILE) $(BLFUSE_FILE) $(BHFUSE_FILE) $(BEFUSE_FILE) $(SFLASH_FILE) $(SEEPROM_FILE) $(SFUSES_FILE) $(SLFUSE_FILE) $(SHFUSE_FILE) $(SEFUSE_FILE)
###############################################################################
