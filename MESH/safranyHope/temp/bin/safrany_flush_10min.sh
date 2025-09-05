#!/usr/bin/env bash
set -eu
LC_ALL=C

BUF_DIR="/run/safranyTemp"
BUF_FILE="${BUF_DIR}/buffer.csv"
SNAP="${BUF_DIR}/buffer.snapshot.csv"

LOG_DIR="/var/log/safranyTemp"
LOG_FILE="${LOG_DIR}/temp.csv"

sudo mkdir -p "$BUF_DIR" "$LOG_DIR"

# header a tartós CSV-ben, ha még nincs
if [ ! -f "$LOG_FILE" ]; then
  echo "timestamp,dht_temp_c_avg,dht_rh_avg,cpu_temp_c_avg,sample_count,load1,load5,load15,root_used_pct,root_avail_gb" \
    | sudo tee -a "$LOG_FILE" >/dev/null
fi

# nincs buffer → nincs mit flush-olni
[ -f "$BUF_FILE" ] || exit 0

# készítsünk pillanatképet és ürítsük a buffert (adatvesztés nélkül, minimal race)
sudo cp -f "$BUF_FILE" "$SNAP" || true
# kinullázás
: | sudo tee "$BUF_FILE" >/dev/null

# dolgozzuk fel a snapshotot
[ -s "$SNAP" ] || exit 0

# csak az utolsó ~10 perc adatait vegyük (±1 perc tolerancia) epoch alapján
NOW=$(date +%s)
CUTOFF=$((NOW - 11*60))

# awk-val szűrés + átlagolás
read -r CNT AVG_T AVG_RH AVG_CPU < <(
  awk -F, -v c="$CUTOFF" '
    $1 ~ /^[0-9]+$/ && $1 >= c {
      t_sum += $2; rh_sum += $3; 
      if ($4 != "") cpu_sum += $4; 
      n_t++; n_rh++; if ($4 != "") n_cpu++;
    }
    END {
      if (n_t>0) at=t_sum/n_t; else at="";
      if (n_rh>0) ar=rh_sum/n_rh; else ar="";
      if (n_cpu>0) ac=cpu_sum/n_cpu; else ac="";
      printf("%d %.3f %.3f %.3f\n", n_t, at, ar, ac);
    }
  ' "$SNAP"
)

# ha nincs minta, lépjünk ki csendben
[ "${CNT:-0}" -gt 0 ] || exit 0

# load average
read -r L1 L5 L15 _ < /proc/loadavg

# diszk root: used% és szabad GB
read -r _ _ _ USED_PCT AVAIL_KB _ < <(df -Pk / | awk 'NR==2{print $1,$2,$3,$5,$4,$6}')
AVAIL_GB=$(awk -v k="$AVAIL_KB" 'BEGIN{printf "%.2f", k/1024/1024}')

TS_HUMAN=$(date +"%Y-%m-%d %H:%M:%S")

LINE="${TS_HUMAN},${AVG_T},${AVG_RH},${AVG_CPU},${CNT},${L1},${L5},${L15},${USED_PCT},${AVAIL_GB}"
echo "$LINE" | sudo tee -a "$LOG_FILE" >/dev/null

# snapshot törlése
sudo rm -f "$SNAP" || true

