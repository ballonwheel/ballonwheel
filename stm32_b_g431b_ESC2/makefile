CC = arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m4 -mthumb -g -O2 -Wall \
	-I/usr/lib/gcc/arm-none-eabi/8.3.1/include

LDFLAGS = -Tstm32_b_g431b_ESC2.ld -specs=nosys.specs
#-specs=nosys.specs? This flag ensures:  System calls like _exit, _write, and _read are not required.

all: main.elf

main.elf: main.o startup.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o *.elf

