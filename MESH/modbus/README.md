

sudo apt-get update
sudo apt-get install mbpoll


🔌 Példa: slave 3, Modbus RTU, /dev/ttyUSB0, 9600 baud
Input regiszter olvasás (pl. 30001..30004, de Modbus címben 0-tól indexel):
mbpoll -m rtu -a 3 -b 9600 -P none -s 1 -t 4:int -r 0 -c 4 /dev/ttyUSB0


Magyarázat:
-m rtu → RTU mód
-a 3 → slave cím 3
-b 9600 → baud rate
-P none → parity none (8N1)
-s 1 → stop bit (1)
-t 4:int → Input Register (function code 4), signed int
-r 0 → kezdő regiszter cím (30001 → 0 offset)
-c 4 → 4 db regiszter
/dev/ttyUSB0 → RS485 adapter eszköz


mbpoll -m rtu -a 3 -b 9600 -P none -s 1 -t 3:int -r 0 1 /dev/ttyUSB0

Itt:
-t 3:int → Holding Register (FC 3)
-r 0 → 40001
1 → érték
Így simán tudsz bash scripttel olvasni/írni minden slave-hez, Python nélkül.







