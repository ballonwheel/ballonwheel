
[pic of Ballonwheel system](https://drive.google.com/file/d/1SbojtoAPsziPh_wPwiDQSBB1EkkLj9f4/view?usp=drivesdk) 



[video of Ballonwheel performance](https://drive.google.com/file/d/1aYekoKIV0XY-9lRHnn9m6GJVegpmdNTa/view?usp=drivesdk) 


# 1 quickstart (iqhomesetup)

$ git clone https://github.com/ballonwheel/ballonwheel

$ cd ballonwheel/bash_solution_only_octave

$ sudo ./run.sh

$ sudo octave bow_octave_disp.m

$ sudo ./stop.sh

 

# 2 install for run and development

$ uname -a

Linux raspberrypi 5.15.36-rt41-v8+ #1 SMP PREEMPT Fri May 6 21:32:06 CEST 2022 aarch64 GNU/Linux

$ lsb_release -a

No LSB modules are available.

Distributor ID:	Debian

Description:	Debian GNU/Linux 11 (bullseye)

Release:	11

Codename:	bullseye

## Git remote settings

$ eval "$(ssh-agent -s)"

Agent pid 251373

$ ssh-add .ssh/id_ed25519.rsa

Enter passphrase for .ssh/id_ed25519.rsa: 

Identity added: .ssh/id_ed25519.rsa (hello@gmail.com)

$ git remote set-url origin git@github.com:ballonwheel/ballonwheel.git


 

## 2.1 octave

$ sudo apt-get install octave octave-signal octave-control octave-instrument-control

$ octave

octave:1> GNU Octave, version 6.2.0

https://octave.sourceforge.io/symbolic/

octave:2> pkg install -forge symbolic



## 2.2 Scliab

$ sudo apt-get install scilab

### 2.2.1 wgserialxcosio

https://atoms.scilab.org/toolboxes/wgserialxcosio/16.04.2

in Scilab Install command--> atomsInstall("wgserialxcosio")

start xcos and you can use the working files in the folder of wgserialxcosio.


## 2.2, maxima

$ sudo apt-get install maxima

$ maxima

(%i2) version: ?\*autoconf\-version\*;

(%o2)                               5.44.0

 
## 2.3, sympy

$ sudo apt-get install python

$ imre@raspberrypi:~/ballonwheel/sympy $ python

$ Python 3.9.2 (default, Feb 28 2021, 17:03:44)

$ [GCC 10.2.1 20210110] on linux


$ sudo apt-get install python3-pip

$ git clone https://github.com/sympy/sympy.git

$ git pull origin master

$ python -m pip install -e .


$ python .\bowSymbol.py



## 2.4, Arduino nano CLI

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

