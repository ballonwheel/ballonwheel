#!/usr/bin/env bash



#!/usr/bin/env bash
set -euo pipefail

CONF="/home/imre/ballonwheel/MESH/safranyHope/gpio/cfg/gpio-service.conf"

# --- RELÉ (konfigból) ---
RELAY_GPIO=27
RELAY_ACTIVE_HIGH=true
RELAY_STATE=0
LOG_FILE="/var/log/safranyGpio/gpio.csv"

# --- BEMENETEK (fixen, nem konfigból) ---
SW1_GPIO=17
SW2_GPIO=24

# --- konfig betöltés ---
[[ -r "$CONF" ]] && source "$CONF"

# --- segédek ---
gpio_export() {
  local n="$1" dir="$2"
  if [[ ! -e "/sys/class/gpio/gpio${n}" ]]; then
    echo "${n}" > /sys/class/gpio/export
  fi
  echo "${dir}" > "/sys/class/gpio/gpio${n}/direction"
}
gpio_write() { echo "$2" > "/sys/class/gpio/gpio$1/value"; }
gpio_read()  { cat "/sys/class/gpio/gpio$1/value"; }
log(){ local ts; ts="$(date '+%Y-%m-%d %H:%M:%S')"; echo "[$ts] $*" >> "$LOG_FILE"; }

# --- init ---
mkdir -p "$(dirname "$LOG_FILE")"
touch "$LOG_FILE"

# export + irány
gpio_export "$RELAY_GPIO" "out"
gpio_export "$SW1_GPIO"  "in"
gpio_export "$SW2_GPIO"  "in"

# bemenetek beolvasása (csak loghoz)
sw1="$(gpio_read "$SW1_GPIO" 2>/dev/null || echo "?")"
sw2="$(gpio_read "$SW2_GPIO" 2>/dev/null || echo "?")"

# relé kívánt HW-szint a konfig szerint (nincs input alapú döntés!)
hw="$RELAY_STATE"
$RELAY_ACTIVE_HIGH || hw=$(( RELAY_STATE==1 ? 0 : 1 ))

# beállítás
gpio_write "$RELAY_GPIO" "$hw"

# --- csak változáskor logolunk ---
STATE_DIR="/run/safranyGpio"; mkdir -p "$STATE_DIR"

prev_sw1="$(cat "$STATE_DIR/sw1.last" 2>/dev/null || echo "")"
prev_sw2="$(cat "$STATE_DIR/sw2.last" 2>/dev/null || echo "")"
prev_hw="$(cat "$STATE_DIR/relay_hw.last" 2>/dev/null || echo "")"
prev_logic="$(cat "$STATE_DIR/relay_logic.last" 2>/dev/null || echo "")"

changed=false
[[ "$sw1"   != "$prev_sw1"   ]] && changed=true
[[ "$sw2"   != "$prev_sw2"   ]] && changed=true
[[ "$hw"    != "$prev_hw"    ]] && changed=true
[[ "$RELAY_STATE" != "$prev_logic" ]] && changed=true

if $changed; then
  log "CHANGE: SW1=$sw1 SW2=$sw2 RELAY_STATE=$RELAY_STATE (hw=$hw)"
fi

# új baseline mentése
echo -n "$sw1"         > "$STATE_DIR/sw1.last"
echo -n "$sw2"         > "$STATE_DIR/sw2.last"
echo -n "$hw"          > "$STATE_DIR/relay_hw.last"
echo -n "$RELAY_STATE" > "$STATE_DIR/relay_logic.last"


