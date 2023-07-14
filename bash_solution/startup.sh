

#meg kell keresni, hogy hol van az arduino
#[  405.999319] usb 1-1.1: FTDI USB Serial Device converter now attached to ttyUSB1

#UARTNUM = eval$(dmesg | grep FTDI USB Serial | grep tty)

#UART=/dev/ttyUSB+$UARTNUM

UART="/dev/ttyUSB1"

eval "$(chmod 777 $UART)"
eval "$(setserial $UART low_latency)"
eval "$(stty 115200 < $UART)"

mount -t tmpfs -o size=512M,mode=1777 bowtmp ./tmp/

echo "000" >  ./tmp/bow_pos
echo "000" >  ./tmp/bow_motor

chown -R imre ./tmp
chgrp -R imre ./tmp
chmod 666 ./tmp/bow_pos
chmod 666 ./tmp/bow_motor

