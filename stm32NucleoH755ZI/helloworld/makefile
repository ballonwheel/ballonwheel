# Compiler
CC = arm-none-eabi-gcc

# Source files
SRCS = hello_world.c

# Include directories
INC_DIRS =

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
TARGET = hello_world.elf

.PHONY: all clean

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET) output.map
