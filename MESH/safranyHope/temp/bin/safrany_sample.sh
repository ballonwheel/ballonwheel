#!/usr/bin/env bash
set -eu
LC_ALL=C

BUF_DIR="/run/safranyTemp"
BUF_FILE="${BUF_DIR}/buffer.csv"

# RAM-beli mappa (tmpfs) biztosan létrejön
sudo mkdir -p "$BUF_DIR"

# DHT11 IIO eszköz megkeresése
IIO_DIR=""
for d in /sys/bus/iio/devices/iio:device*; do
  [ -d "$d" ] || continue
  if [ -f "$d/name" ] && grep -q '^dht11@' "$d/name"; then
    IIO_DIR="$d"; break
  fi
done

# ha nincs IIO DHT, most nincs mit írni
[ -z "$IIO_DIR" ] && exit 0

# 1 mérés, kis retry-val (kernel DHT11 néha nyűgös)
RETRIES=3
RETRY_SLEEP=0.5
DHT_T=""
DHT_RH=""

for _ in $(seq 1 "$RETRIES"); do
  T_RAW=$(cat "${IIO_DIR}/in_temp_input" 2>/dev/null || true)
  H_RAW=$(cat "${IIO_DIR}/in_humidityrelative_input" 2>/dev/null || true)
  if [[ "$T_RAW" =~ ^[0-9]+$ ]] && [[ "$H_RAW" =~ ^[0-9]+$ ]]; then
    DHT_T=$(awk -v x="$T_RAW" 'BEGIN{printf "%.3f", x/1000.0}')
    DHT_RH=$(awk -v x="$H_RAW" 'BEGIN{printf "%.3f", x/1000.0}')
    break
  fi
  sleep "$RETRY_SLEEP"
done

# ha nincs érvényes DHT adat, dobjuk ezt a mintát
[ -z "$DHT_T" ] && exit 0
[ -z "$DHT_RH" ] && exit 0

# CPU hőmérséklet
CPU_RAW=$(cat /sys/class/thermal/thermal_zone0/temp 2>/dev/null || echo "")
CPU_T=""
if [[ "$CPU_RAW" =~ ^[0-9]+$ ]]; then
  CPU_T=$(awk -v x="$CPU_RAW" 'BEGIN{printf "%.3f", x/1000.0}')
fi

TS=$(date +%s)  # epoch, másodperc
# formátum: epoch,dht_temp_c,dht_rh_pc,cpu_temp_c
LINE="${TS},${DHT_T},${DHT_RH},${CPU_T}"

# írjuk RAM bufferbe; kicsi sor → append atomikus gyakorlatilag
echo "$LINE" | sudo tee -a "$BUF_FILE" >/dev/null

