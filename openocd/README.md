


#1. install
https://openocd.org/


https://github.com/xpack-dev-tools/openocd-xpack/releases



https://github.com/xpack-dev-tools/openocd-xpack/releases/download/v0.12.0-3/xpack-openocd-0.12.0-3-linux-arm64.tar.gz


https://sourceforge.net/p/openocd/code/ci/master/tree/


#2. Quickstart for the impatient
Olimex at91sam3sxx board-dal
============================

Windows-on Olimex ftdi driver:
https://zadig.akeo.ie/

Windows-ra gcc, gdb:
https://www.st.com/en/development-tools/stm32cubeclt.html

.\bin\openocd.exe -f .\scripts\interface\ftdi\olimex-arm-usb-ocd.cfg -f .\scripts\target\at91sam3sXX.cfg


After OpenOCD startup, connect GDB with

  (gdb) target extended-remote localhost:3333


