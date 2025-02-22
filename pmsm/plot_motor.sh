

#!/bin/bash

# Create a temporary file for real-time plotting
DATA_FILE=$(mktemp)

# Cleanup function to remove temp file on exit
cleanup() {
    rm -f "$DATA_FILE"
}
trap cleanup EXIT

# Start the motor simulation and save output to temp file
./dc_motor_sim | while IFS=$'\t' read -r time voltage current speed; do
    echo "$time $voltage $current $speed" >> "$DATA_FILE"
    
    # Use Gnuplot to plot the data in ASCII mode
    gnuplot <<EOF
set terminal dumb 100 30
set title "DC Motor Real-Time Plot"
set xlabel "Time (s)"
set ylabel "Values"
set grid
set key outside
set xrange [*:*]
set yrange [-15:15]
plot "$DATA_FILE" using 1:2 with lines title "Voltage", \
     "$DATA_FILE" using 1:3 with lines title "Current", \
     "$DATA_FILE" using 1:4 with lines title "Speed"
EOF
    sleep 0.5  # Refresh interval
    clear  # Clear terminal for real-time effect
done
