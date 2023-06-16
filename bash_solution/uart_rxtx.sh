#!/bin/bash


((num=0))
while ((i++)); read -r fromarduino
do
  ((num++))
  read -r toarduino < /home/imre/bow_motor

  if [ "$num" -gt "100" ]; then
    ((num=0))
    echo "$i. data: $fromarduino < /dev/ttyUSB0"
    echo "$i. data: $toarduino > /dev/ttyUSB0"

  fi
  echo $fromarduino > /home/imre/bow_pos
  echo $toarduino > /dev/ttyUSB0
done < /dev/ttyUSB0


