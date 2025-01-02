# STM32G4 Makefile

# Compiler
CC = arm-none-eabi-gcc

# Compiler flags
CFLAGS = -mcpu=cortex-m4 -mthumb -Wall -Og \
	-I./Inc \
	-I/home/imre/stm32/STM32CubeG4/Drivers/STM32G4xx_HAL_Driver/Inc \
	-I/home/imre/stm32/STM32CubeG4/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy \
	-I/home/imre/stm32/STM32CubeG4/Drivers/CMSIS/Device/ST/STM32G4xx/Include \
	-I/home/imre/stm32/STM32CubeG4/Drivers/CMSIS/Include \
	-I/usr/lib/gcc/arm-none-eabi/8.3.1/include

LDFLAGS = -TSTM32G431RBTX_FLASH.ld -specs=nosys.specs

CFLAGS += -DSTM32G431xx


# Source files
SRCS = main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Output files
TARGET = main.elf
BIN = main.bin

# Path to STM32CubeG4 root directory
STM32_CUBE_G4_PATH = /home/imre/stm32/STM32CubeG4

# HAL driver files
HAL_SRCS = $(STM32_CUBE_G4_PATH)/Drivers/STM32G4xx_HAL_Driver/Src/*.c
CMSIS_SRCS = $(STM32_CUBE_G4_PATH)/Drivers/CMSIS/Device/ST/STM32G4xx/Source/Templates/system_stm32g4xx.c
STARTUP_SCRS = $(STM32_CUBE_G4_PATH)/Drivers/CMSIS/Device/ST/STM32G4xx/Source/Templates/gcc/startup_stm32g431xx.s

.PHONY: all clean

all: $(BIN)

$(BIN): $(TARGET)
	arm-none-eabi-objcopy -O binary $(TARGET) $(BIN)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(HAL_SRCS) $(CMSIS_SRCS) $(STARTUP_SCRS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(BIN)

flash: main.bin
	st-flash write main.bin 0x08000000

