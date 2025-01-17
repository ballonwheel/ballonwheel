CC = arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m4 -mthumb -Wall -Og \
	-I/usr/lib/gcc/arm-none-eabi/8.3.1/include

LDFLAGS = -Tstm32_b_g431b_ESC2.ld -specs=nosys.specs
#-specs=nosys.specs? This flag ensures:  System calls like _exit, _write, and _read are not required.
CFLAGS += -DSTM32G431xx
SRCS = main.c
OBJS = $(SRCS:.c=.o)
TARGET = main.elf
BIN = main.bin
STARTUP_SCRS = startup.c

.PHONY: all clean

all: $(BIN)

$(BIN): $(TARGET)
	arm-none-eabi-objcopy -O binary $(TARGET) $(BIN)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(STARTUP_SCRS) -o $@
# $@ The TARGET(ami a : elott van) file name (e.g., main.elf).
# $^ List of prerequisites(ami a : utan van)  (e.g., main.o startup.o).
# $< The first prerequisite.
# $? Prerequisites that are newer than the target.

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(BIN)


flash: main.bin
	st-flash write main.bin 0x08000000

