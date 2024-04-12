# ADS1115
ADC 16bit 860sps i2c 4singleEnded 3.3V

https://www.ti.com/lit/ds/symlink/ads1115.pdf


Understanding Kernel modul dev

https://sysprog21.github.io/lkmpg/



## 1. i2c character device kernel driver

### 1.1 Egy lehetoseg, hogy chaaracter device-t irok es userspace-ben hasznalom az ADC adatot.

https://hubtronics.in/learn/ads1115-char-driver-raspberry-pi


### 1.2 Masik lehetoseg, hogy az i2c driverre irok user space-bol

https://github.com/giobauermeister/ads1115-linux-rpi

https://www.bristolwatch.com/rpi/ads1115.html




## 2. devicetree

https://forums.raspberrypi.com/viewtopic.php?t=275563

$ sudo nano /boot/config.txt

dtoverlay=ads1015,dtparam=cha_enable,cha_cfg=0


$ sudo reboot now

$ lsmod

Module                  Size  Used by

ti_ads1015             28672  0

industrialio_triggered_buffer    16384  1 ti_ads1015

kfifo_buf              16384  1 industrialio_triggered_buffer

industrialio           90112  3 industrialio_triggered_buffer,kfifo_buf,ti_ads1015

regmap_i2c             16384  1 ti_ads1015

brcmfmac              335872  0


$ cd /sys/bus/iio/devices/iio:device0

$ /sys/bus/iio/devices/iio:device0 $ cat name

ads1015


$ /sys/bus/iio/devices/iio:device0 $ ls 

buffer                                   in_voltage0-voltage1_scale               in_voltage2_raw                          of_node

buffer0                                  in_voltage0-voltage3_raw                 in_voltage2_sampling_frequency           power

current_timestamp_clock                  in_voltage0-voltage3_sampling_frequency  in_voltage2_scale                        sampling_frequency_available

dev                                      in_voltage0-voltage3_scale               in_voltage2-voltage3_raw                 scale_available

events                                   in_voltage1_raw                          in_voltage2-voltage3_sampling_frequency  scan_elements

in_voltage0_raw                          in_voltage1_sampling_frequency           in_voltage2-voltage3_scale               subsystem

in_voltage0_sampling_frequency           in_voltage1_scale                        in_voltage3_raw                          trigger

in_voltage0_scale                        in_voltage1-voltage3_raw                 in_voltage3_sampling_frequency           uevent

in_voltage0-voltage1_raw                 in_voltage1-voltage3_sampling_frequency  in_voltage3_scale

in_voltage0-voltage1_sampling_frequency  in_voltage1-voltage3_scale               name


$ /sys/bus/iio/devices/iio:device0 $ cat ./in_voltage1_scale 

3.000000000


$ /sys/bus/iio/devices/iio:device0 $ cat ./in_voltage1_raw 

0


$ /sys/bus/iio/devices/iio:device0 $ cat ./in_voltage1_raw 

54


$ /sys/bus/iio/devices/iio:device0 $ cat ./in_voltage1_raw 

642



