######################################
# target
######################################
TARGET = motor_driver

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
src/main.c \
src/system_stm32f1xx.c \
src/spi.c \
src/uart.c \
src/syscalls.c \
src/gpio.c \
src/tim.c \
src/adc.c \
src/dma.c \
src/delay.c \
src/drv8323.c \
src/iwdg.c \
src/hw_init.c \
src/irq.c

# ASM sources
ASM_SOURCES =  \
startup_stm32f103xb.s


######################################
# firmware library
######################################
PERIFLIB_SOURCES =


#######################################
# binaries
#######################################
BINPATH = /opt/gcc-arm-none-eabi-7-2017-q4-major/bin/
PREFIX = $(BINPATH)arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump
AR = $(PREFIX)ar
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-D STM32F103xB


# AS includes
AS_INCLUDES =

# C includes
C_INCLUDES =  \
-I inc \
-I Drivers/CMSIS/Device \
-I Drivers/CMSIS/Include

C_WARNINGS = \
-Wall


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(C_WARNINGS)

ifeq ($(DEBUG), 1)
CFLAGS += -g3 -gdwarf-2
endif


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F103RBTx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: bin

# Prettify output
V = 0
ifeq ($V, 0)
	Q := @
endif

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo "[CC]	$(notdir $<)"
	$Q$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo "[AS]	$(notdir $<)"
	$Q$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@echo "[LD]	$(TARGET).elf"
	$Q$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@echo "[OBJDUMP]	$(TARGET).lst"
	$(OBJDUMP) -St $(BUILD_DIR)/$(TARGET).elf >$(BUILD_DIR)/$(TARGET).lst
	@echo "[SZ]	$(TARGET).elf:"
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo "[OBJCOPY]	$(TARGET).hex"
	$Q$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo "[OBJCOPY]	$(TARGET).bin"
	$Q$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR)

bin: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

flash: bin
	@echo "[FLASH] flashing $(BUILD_DIR)/$(TARGET).bin to 0x8000000"
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000

openocd:
	openocd -f board/st_nucleo_f103rb.cfg
#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
