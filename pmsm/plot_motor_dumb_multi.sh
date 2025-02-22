

#!/bin/bash

# Create temporary files for real-time plotting
DATA_FILE=$(mktemp)

# Cleanup function to remove temp file on exit
cleanup() {
    rm -f "$DATA_FILE"
}
trap cleanup EXIT

# Start the motor simulation and save output to temp file
./dc_motor_sim | while IFS=$'\t' read -r time voltage current speed; do
    echo "$time $voltage $current $speed" >> "$DATA_FILE"

    clear  # Clear the screen before updating all plots

    # Plot Voltage
    gnuplot <<EOF
set terminal dumb 50 15
set title "Voltage"
set xlabel "Time (s)"
set ylabel "Voltage (V)"
set xrange [*:*]
set yrange [-15:15]
plot "$DATA_FILE" using 1:2 with lines title "V"
EOF

    # Plot Current
    gnuplot <<EOF
set terminal dumb 50 15
set title "Current"
set xlabel "Time (s)"
set ylabel "Current (A)"
set xrange [*:*]
set yrange [-5:5]
plot "$DATA_FILE" using 1:3 with lines title "I"
EOF

    # Plot Speed
    gnuplot <<EOF
set terminal dumb 50 15
set title "Speed"
set xlabel "Time (s)"
set ylabel "Speed (rad/s)"
set xrange [*:*]
set yrange [-10:10]
plot "$DATA_FILE" using 1:4 with lines title "Speed"
EOF

    sleep 0.1  # Refresh interval
done
