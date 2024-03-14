
![bow setup](https://drive.google.com/file/d/1SbojtoAPsziPh_wPwiDQSBB1EkkLj9f4/view?usp=drivesdk) 



1, quickstart (iqhomesetup)

$ git clone https://github.com/ballonwheel/ballonwheel

$ cd ballonwheel/bash_solution_only_octave

$ sudo ./run.sh

$ sudo octave bow_octave_disp.m

$ sudo ./stop.sh

 

2, install for run and development
$ uname -a

Linux raspberrypi 5.15.36-rt41-v8+ #1 SMP PREEMPT Fri May 6 21:32:06 CEST 2022 aarch64 GNU/Linux

 

2.1, octave install

$ sudo apt-get install octave

$ sudo apt-get install octave-control

$ sudo apt-get install octave-signal

$ sudo apt-get install octave-instrument-control

$ sudo apt-get install octave-symbolic

 

2.2, maxima install, 

$ sudo apt-get install maxima

$ maxima

(%i2) version: ?\*autoconf\-version\*;

(%o2)                               5.44.0

 


2.3, Arduino nano CLI

https://github.com/arduino/arduino-cli

https://arduino.github.io/arduino-cli/0.21/commands/arduino-cli/

$ mkdir bow

$ cd bow

https://github.com/arduino/arduino-cli/releases

$ wget https://github.com/arduino/arduino-cli/releases/download/0.22.0/arduino-cli_0.22.0_Linux_64bit.tar.gz

unpack here 

$ ./arduino-cli

$ ./arduino-cli config init

$ ./arduino-cli sketch new blink

$ nano blink/blink.ino

$ ./arduino-cli core update-index

$ ./arduino-cli board list 

$ ./arduino-cli board listall

$ ./arduino-cli core search nano

$ ./arduino-cli core install arduino:avr   

$ ./arduino-cli core update-index

$ ./arduino-cli board list   

$ ./arduino-cli board listall

$ sudo chmod 777 /dev/ttyUSB0    

$ ./arduino-cli board list   

$ ./arduino-cli compile --verbose --fqbn arduino:avr:nano blink   

$ ./arduino-cli upload -v -p /dev/ttyUSB0 --fqbn arduino:avr:nano blink 

$ cd ..

$ cd ./ballonwheel/bash_solution_only_octave/arduino

$ ../../../bow/arduino-cli compile --verbose --fqbn arduino:avr:nano arduino.ino

$ ../../../bow/arduino-cli upload -v -p /dev/ttyUSB0 --fqbn arduino:avr:nano arduino.ino






