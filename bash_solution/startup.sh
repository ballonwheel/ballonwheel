


set -x
#echo on
echo startup
echo $1
echo $2

#meg kell keresni, hogy hol van az arduino
#[  405.999319] usb 1-1.1: FTDI USB Serial Device converter now attached to ttyUSB1

#UARTNUM = eval$(dmesg | grep FTDI USB Serial | grep tty)

#UART=/dev/ttyUSB+$UARTNUM

#UART="/dev/ttyUSB0"
UART="/dev/ttyUSB$2"

chmod 777 $UART
if [ "$1" == "ftdi" ]
  then
    setserial -v $UART low_latency
    stty 115200 < $UART
    echo "ftdi setting"
  else
    echo "ch340 do nothing"
fi



#ket fajl van a RAMban
#kezdetben mindkettoben 001
#bow_pos   <-- UART
#bow_motor --> UART
umount ./tmp
mount -t tmpfs -o size=512M,mode=1777 bowtmp ./tmp/

echo "001" >  ./tmp/bow_pos
echo "001" >  ./tmp/bow_motor

chown -R imre ./tmp
chgrp -R imre ./tmp
chmod 666 ./tmp/bow_pos
chmod 666 ./tmp/bow_motor


