#!/bin/bash


#Setserial /dev/ttyUSB0 low_latency
#stty 19200 < /dev/ttyS1
#stty 19200 -F /dev/ttyS1

#od -x < /dev/ttyUSB0

# sudo mount -t tmpfs -o size=512M,mode=1777 bowtmp ./tmp/


UART="/dev/ttyUSB1"

((num=0))
while ((i++)); read -r fromarduino
do
  ((num++))
  read -r toarduino < ./tmp/bow_motor

  if [ "$num" -gt "100" ]; then
    ((num=0))
    echo "$i. data: $fromarduino < $UART"
    echo "$i. data: $toarduino > $UART"

  fi
  echo $fromarduino > ./tmp/bow_pos
  echo $toarduino > $UART
done < $UART



