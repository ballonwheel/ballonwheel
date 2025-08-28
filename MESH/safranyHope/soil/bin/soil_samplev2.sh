#!/usr/bin/env bash
set -euo pipefail

D="/sys/bus/iio/devices/iio:device0"
OUTDIR="/var/log/soil"
mkdir -p "$OUTDIR"
CSV="$OUTDIR/$(date +%F).csv"

# ha még nincs fejléc:
if [ ! -s "$CSV" ]; then
  echo "timestamp,volt0,volt1,volt2,volt3" >> "$CSV"
fi

read_v() {
  local ch="$1"
  local raw scale
  raw=$(cat "$D/in_voltage${ch}_raw")
  scale=$(cat "$D/in_voltage${ch}_scale")
  >&2 echo "[DEBUG] ch=$ch raw=$raw scale=$scale"
  python3 - <<PY
r=$raw
s=$scale
print(f"{r*s:.6f}")
PY
}

avg_channel() {
  local ch="$1" n=10
  >&2 echo "[DEBUG] Measuring channel $ch ($n samples)..."
  local sum=0.0
  for i in $(seq 1 $n); do
    v=$(read_v "$ch")
    >&2 echo "[DEBUG] Sample $i for ch$ch = $v"
    sum=$(python3 - <<PY
a=$sum
b=$v
print(a+b)
PY
)
    sleep 1
  done
  avg=$(python3 - <<PY
print(f"{ $sum / $n :.6f}")
PY
)
  >&2 echo "[DEBUG] Avg for ch$ch = $avg"
  echo "$avg"
}

ts="$(date -Is)"
v0=$(avg_channel 0)
v1=$(avg_channel 1)
v2=$(avg_channel 2)
v3=$(avg_channel 3)

>&2 echo "[DEBUG] Writing line: $ts,$v0,$v1,$v2,$v3"
echo "$ts,$v0,$v1,$v2,$v3" >> "$CSV"
