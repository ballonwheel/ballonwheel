

tapasztalatok: megy de hibazik, foleg cat-tal, de szkriptbol ismetelgetve jo lesz...
a python pigpio daemon-os megoldas biztosabbn megy, mert az DMAzik, a kernel-es meg interruptozik
a dht egyvezetekes i/o erzekeny az idozitesre...

kerneles megoldast hasznalom
lasd ../MESH/safranyhope/temp

elegendo a /boot/config-txt-be
dtoverlay=dht11,gpiopin=22


**********

Igen — a DHT11-hez van **Linux kernel IIO** driver, így teljesen megoldható, hogy **Bash-ból** olvasd az értékeket a **/sys** fájlrendszeren. Az alábbiakban megmutatom a gyors beállítást Raspberry Pi Zero 2 W-n (GPIO **BCM22**) és adok egy pici Bash olvasó szkriptet.

# 1) Hardver

* DHT11 **VCC → 3V3**, **GND → GND**, **DATA → BCM22 (fizikai pin 15)**
* **10 kΩ felhúzó** ellenállás a DATA és 3V3 közé (kötelező).

# 2) Kernel modul / Device Tree overlay

A hivatalos kernel driver neve **dht11** (IIO alrendszer). A legegyszerűbb a Raspberry Pi-s DT overlay.

Nyisd meg a boot konfigot, és add hozzá a sort (Bookwormon tipikusan: **/boot/firmware/config.txt**; régebbi RPi OS-en: **/boot/config.txt**):

```
dtoverlay=dht11,gpiopin=22
```

Mentsd és indítsd újra:

```bash
sudo reboot
```

## Megjegyzések

* Ha saját RT kernelt fordítasz: legyen bekapcsolva **CONFIG\_IIO** és **CONFIG\_DHT11** (=m javasolt).
* Újraindítás után a driver létrehoz egy IIO eszközt:
  `/sys/bus/iio/devices/iio:deviceX/`

Tipikus fájlok:

* `name` → „dht11”
* `in_humidityrelative_input` → relatív páratartalom **ezred százalékban** (‰%), azaz %RH × 1000
* `in_temp_input` → hőmérséklet **ezred fokban** (milli-°C), azaz °C × 1000

# 3) Gyors teszt Bash-ból

Találd meg az eszközt és olvass:

```bash
cd /sys/bus/iio/devices
grep -l dht11 iio:device*/name
# pl. iio:device0

cat iio:device0/in_humidityrelative_input
cat iio:device0/in_temp_input
```

# 4) Egysoros, emberbarát kiírás

```bash
IIO=/sys/bus/iio/devices/$(grep -l dht11 /sys/bus/iio/devices/iio:device*/name | xargs dirname | xargs basename)
HRAW=$(cat /sys/bus/iio/devices/$IIO/in_humidityrelative_input)
TRAW=$(cat /sys/bus/iio/devices/$IIO/in_temp_input)
printf "RH=%.1f%%  T=%.1f°C\n" "$(bc <<< "scale=1;$HRAW/1000")" "$(bc <<< "scale=1;$TRAW/1000")"
```

# 5) Folyamatos konzolos mérő (Bash script)

Mentsd `dht11_iio_read.sh` néven, tedd futtathatóvá (`chmod +x dht11_iio_read.sh`):

```bash
#!/usr/bin/env bash
set -euo pipefail

# Keressük meg a dht11 nevű IIO eszközt
find_dht11() {
  for d in /sys/bus/iio/devices/iio:device*; do
    [[ -f "$d/name" ]] || continue
    if grep -q "^dht11$" "$d/name"; then
      echo "$d"
      return 0
    fi
  done
  return 1
}

DEV="$(find_dht11 || true)"
if [[ -z "${DEV:-}" ]]; then
  echo "DHT11 IIO eszköz nem található. Ellenőrizd az overlay-t: dtoverlay=dht11,gpiopin=22"
  exit 1
fi

echo "Olvasás innen: $DEV"
# A DHT11 specifikáció szerint legalább ~1 s-ot várj két mintavétel között.
# A kernel driver is ennek megfelelően szokott viselkedni; túl sűrű olvasás hibát adhat.

while :; do
  if HRAW=$(cat "$DEV/in_humidityrelative_input" 2>/dev/null) \
     && TRAW=$(cat "$DEV/in_temp_input" 2>/dev/null); then
    RH=$(awk -v v="$HRAW" 'BEGIN{printf("%.1f", v/1000)}')
    TC=$(awk -v v="$TRAW" 'BEGIN{printf("%.1f", v/1000)}')
    printf "RH=%s%%  T=%s°C\n" "$RH" "$TC"
  else
    echo "Olvasási hiba (túl gyakori lekérdezés vagy jelhiba)."
  fi
  sleep 2
done
```

# 6) Jogosultságok

Általában az IIO bemenetek **olvashatók mindenki számára** (0444), tehát nem kell root. Ha mégsem, készíthetsz egy udev szabályt, hogy a `iio` csoport olvashassa.

# 7) Mi történik a motorháztető alatt?

* Ez **kernel-módú** (IIO) driver, amely az időzítéskritikus DHT11 protokollt kezeli.
* Olvasáskor a driver intézi a start jelet és a 40 bit időzítéses mintavételét, majd a kapott értékeket a standard IIO interfészen publikálja (milli-egységekben).
* A DHT11 lassú érzékelő: specifikáció szerint **\~1 s** minimális mintavételi periódus, alacsony felbontás (tipikusan egész °C és egész %RH).

---

Ha szeretnéd, adok egy **systemd service + timer** párost is, ami óránként egyet mér és CSV-be loggolja, vagy készítek egy **egylövéses** (`--once`) Bash változatot.

