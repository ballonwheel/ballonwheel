#!/bin/bash

# od -x < /dev/ttyUSB0

# sudo mount -t tmpfs -o size=512M,mode=1777 bowtmp ./tmp/

# scilab-cli
# exec('/home/imre/ballonwheel/bash_solution/uart.sci')


toarduino="UUUU"
UART="/dev/ttyUSB1"

chmod 777 $UART
setserial $UART low_latency
stty 115200 < $UART
#stty 19200 -F /dev/ttyS1



echo "000" > ./tmp/bow_pos

((num=0))
while ((i++)); read -r fromarduino # a read /n ig olvas!!
#while ((1));
do
  ((num++))
  #read -r toarduino < ./tmp/bow_motor

  #if [ "$num" -gt "100" ]; then
    ((num=0))
    echo "$i. data: $fromarduino < "
    echo "$i. data: $toarduino > "
    #echo "$i data. UUU >\n"
  #fi
  echo "$fromarduino" > ./tmp/bow_pos
  #sleep 0.01
  echo "$toarduino" > $UART
  #echo "UUU" > $UART
#sleep 1
done < $UART
#done


