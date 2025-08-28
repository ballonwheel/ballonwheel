#!/usr/bin/env bash
set -euo pipefail
D="/sys/bus/iio/devices/iio:device0"
# OUTDIR="/var/log/soil"
OUTDIR="."
mkdir -p "$OUTDIR"
CSV="$OUTDIR/$(date +%F).csv"

# ha még nincs fejléc:
[ -s "$CSV" ] || echo "timestamp,volt0,volt1,volt2,volt3" >> "$CSV"

read_v() {
  local ch="$1"
  local raw scale
  raw=$(cat "$D/in_voltage${ch}_raw")
  scale=$(cat "$D/in_voltage${ch}_scale")
  python3 - <<PY
r=$raw
s=$scale
print(f"{r*s:.6f}")
PY
}

avg_channel() {
  local ch="$1" n=10 sum=0
  for _ in $(seq 1 $n); do
    v=$(read_v "$ch")
    # bc helyett python:
    sum=$(python3 - <<PY
a=$sum
b=$v
print(a+b)
PY
)
    sleep 2
  done
  python3 - <<PY
print(f"{ $sum / 10 :.6f}")
PY
}

ts="$(date -Is)"
v0=$(avg_channel 0)
v1=$(avg_channel 1)
v2=$(avg_channel 2)
v3=$(avg_channel 3)

echo "$ts,$v0,$v1,$v2,$v3" >> "$CSV"

