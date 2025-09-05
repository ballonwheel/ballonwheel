

heartbeat
    copy service to server(rpiz2w3)
				--> scp /var/log/
				
temp service
    tempambient, temp cpu, cpuload, diskfree
	lasd ~ballonwheel/dht11
	/boot/config

gpio service
    pullup, input service
    output service
    gpin: hordolow, hordohigh;  --> cat /var/log/safranyGpio/gpio.csv
    gpout: szelep               <-- cat /var/log/

soil service
    adc0123
				--> cat / var/log/soil/??
	lasd ~ballonwheel/ads1115
	/boot/config.txt
	

| Funkció                 |   BCM GPIO |  Fejléc tű | Megjegyzés                                                 |
| ----------------------- | ---------: | ---------: | ---------------------------------------------------------- |
| ADS1115 SDA             |  **GPIO2** |  pin **3** | I²C1 (meglévő) – ne használd másra                         |
| ADS1115 SCL             |  **GPIO3** |  pin **5** | I²C1 (meglévő) – ne használd másra                         |
| **Relé kimenet**        | **GPIO27** | pin **13** | Kimenet (LOW/HIGH a modulod szerint)                       |
| **Kapcsoló #1 bemenet** | **GPIO17** | pin **11** | Bemenet, belső pull-up, GND-re húzott kapcsoló             |
| **Kapcsoló #2 bemenet** | **GPIO24** | pin **18** | Bemenet, belső pull-up, GND-re húzott kapcsoló             |
| **DHT11 adat**          | **GPIO22** | pin **15** | Egyvezetékes adat, **4.7–10 kΩ** felhúzó 3.3 V-ra ajánlott |


Íme egy egyszerű ASCII-pinout a Raspberry Pi Zero 40 tűs fejlécéről (felülnézet, 1-es a saroknál).
Bal oldal: páratlan pinek (1…39) • Jobb oldal: páros pinek (2…40)

```
Raspberry Pi Zero – GPIO header (top view)

 3V3         (1) o o (2)     5V
 SDA1  GPIO2 (3) o o (4)     5V
 SCL1  GPIO3 (5) o o (6)     GND
 GPCLK0 GPIO4(7) o o (8)     GPIO14 TXD
 GND         (9) o o (10)    GPIO15 RXD
 GPIO17     (11) o o (12)    GPIO18 PCM_CLK
 GPIO27     (13) o o (14)    GND
 GPIO22     (15) o o (16)    GPIO23
 3V3        (17) o o (18)    GPIO24
 MOSI  GPIO10(19) o o (20)   GND
 MISO  GPIO9 (21) o o (22)   GPIO25
 SCLK  GPIO11(23) o o (24)   GPIO8  CE0
 GND        (25) o o (26)    GPIO7  CE1
 ID_SD GPIO0(27) o o (28)    ID_SC GPIO1
 GPIO5      (29) o o (30)    GND
 GPIO6      (31) o o (32)    GPIO12 PWM0
 GPIO13 PWM1(33) o o (34)    GND
 GPIO19 PCM_FS(35) o o (36)  GPIO16
 GPIO26     (37) o o (38)    GPIO20 PCM_DIN
 GND        (39) o o (40)    GPIO21 PCM_DOUT
```

Gyors legenda:
I²C: SDA1=GPIO2, SCL1=GPIO3 • UART: TXD=GPIO14, RXD=GPIO15 •
SPI: MOSI=GPIO10, MISO=GPIO9, SCLK=GPIO11, CE0=GPIO8, CE1=GPIO7 •
Táp: 3V3 a (1,17), 5V a (2,4), GND: (6,9,14,20,25,30,34,39).



Megoldjuk kétféleképp — válaszd, ami szimpatikusabb.

## 1) Tartós (bootkor beállítja a firmware) – **ajánlott**

Szerkeszd a **/boot/firmware/config.txt**-t (ha nincs ilyen, akkor **/boot/config.txt**):

```
# Kapcsolók: belső felhúzó bekapcsolása
gpio=17=ip,pu
gpio=24=ip,pu
```

* `ip` = input, `pu` = pull-up.
* Mentés után **reboot**:

  ```bash
  sudo reboot
  ```
* Ellenőrzés:

  ```bash
  sudo apt-get update && sudo apt-get install -y raspi-gpio
  raspi-gpio get 17
  raspi-gpio get 24
  # ... a kimenetben "pu" (pull up) kell szerepeljen
  ```

> Debian Bookwormon a Pi-n jellemzően **/boot/firmware/config.txt** az aktív; ha a másik van meg, azt használd.

---

## 2) Azonnali (futás közben) – reboot nélkül

```bash
sudo apt-get update && sudo apt-get install -y raspi-gpio
sudo raspi-gpio set 17 ip pu
sudo raspi-gpio set 24 ip pu
sudp raspi-gpio get 17
sudo raspi-gpio get 24
```

Ha szeretnéd, hogy ez minden bootkor automatikusan lefusson (anélkül, hogy a config.txt-t módosítanád), adj egy kis oneshot service-t:

**/etc/systemd/system/safrany-gpio-pullups.service**

```ini
[Unit]
Description=Enable pull-ups on GPIO17 and GPIO24

[Service]
Type=oneshot
ExecStart=/usr/bin/raspi-gpio set 17 ip pu
ExecStart=/usr/bin/raspi-gpio set 24 ip pu

[Install]
WantedBy=multi-user.target
```

Aktiválás:

```bash
sudo systemctl daemon-reload
sudo systemctl enable --now safrany-gpio-pullups.service
```

---

### Megjegyzés

* A sysfs (`/sys/class/gpio/...`) **nem** tud pull-upot kapcsolni, ezért kell **config.txt** vagy **raspi-gpio**/DT pinctrl.
* A mi `safrany_relay_apply.sh` szkriptünk nyugodtan maradhat: a pull-up egyszer beáll, és úgy marad, amíg nem írod át.
