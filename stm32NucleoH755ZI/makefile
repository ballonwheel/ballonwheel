# Compiler
CC = arm-none-eabi-gcc

# Source files
SRCS = main.c

# Include directories
INC_DIRS = -IInc -IFreeRTOS/Source/include -IFreeRTOS/Source/portable/GCC/ARM_CM7/r0p1

# Compiler flags
CFLAGS = -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16
CFLAGS += -Wall -Wextra -std=c11
CFLAGS += $(INC_DIRS)

# Linker script
LDSCRIPT = STM32H755ZITx_FLASH.ld

# Linker flags
LDFLAGS = -T$(LDSCRIPT) -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16
LDFLAGS += -Wl,--gc-sections -Wl,-Map=output.map

# Object files directory
BUILD_DIR = build

# Object files
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))

# Executable name
TARGET = firmware.elf

# OpenOCD settings
OPENOCD_INTERFACE = /path/to/interface.cfg
OPENOCD_TARGET = stm32h7x

# FreeRTOS settings
FREERTOS_VERSION = V10.4.4
FREERTOS_URL = https://github.com/FreeRTOS/FreeRTOS-Kernel/archive/$(FREERTOS_VERSION).zip
FREERTOS_DIR = FreeRTOS
FREERTOS_ZIP = $(FREERTOS_VERSION).zip

.PHONY: all clean flash install

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET) output.map

flash: $(TARGET)
	openocd -f $(OPENOCD_INTERFACE) -c "program $(TARGET) verify reset exit" -c "reset run"

install:
	wget $(FREERTOS_URL)
	unzip $(FREERTOS_ZIP)
	mv FreeRTOS-Kernel-$(FREERTOS_VERSION) $(FREERTOS_DIR)
	rm -f $(FREERTOS_ZIP)
