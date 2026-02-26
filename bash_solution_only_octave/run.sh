#!/bin/bash

echo "run"
date &> log.run
echo on
set -x &>> log.run

#http://comfilewiki.co.kr/en/doku.php?id=comfilepi:improving_real-time_performance:index
#sudo apt install cpufrequtils
cpufreq-set -g performance &>> log.run
#pi@raspberrypi:~ $ sudo cat /sys/devices/system/cpu/cpu[0-3]/cpufreq/cpuinfo_cur_freq
#1500000
#1500000
#1500000
#1500000
#pi@raspberrypi:~ $ vcgencmd measure_clock arm
#frequency(48)=1500398464

#First, add line below:
#isolcpus=nohz,domain,managed_irq,3 irqaffinity=0-2 nohz_full=3 rcu_nocbs=3
#to the /boot/cmdline.txt file.
#This will prevent Linux from allocating the 4th core to most process, interrupt handlers, and kernel threads. Reboot for the change to take effect.



umount ./tmp
mount -t tmpfs -o size=512M,mode=1777 bowtmp ./tmp/
#touch ./tmp/bow_motor
chown -R imre ./tmp
chgrp -R imre ./tmp
chmod 666 ./tmp/bow_motor


#octave ciklus:
date &> log.octave
#(nice --20 octave bow_octave.m &>> log.octave) &
nice --20 octave bow_octave_uart.m &
pid_octave=$!


#get PID
echo "$pid_octave" > stop_pid_octave
#echo "PID of this script: $$"
#echo "PPID of this script: $PPID"
#echo "UID of this script: $UID"


#RT improve, tesztelni kell
#octave egyik proc, a.out masik proc
taskset -cp 3 $pid_octave &>> log.run
#sudo chrt -v -f -p 50 $pid_a &>> log.run # milyen prio legyen??
#ezt inkabb ne: sudo sh -c "echo -1 > /proc/sys/kernel/sched_rt_runtime_us"

#RT kernel
#http://comfilewiki.co.kr/en/doku.php?id=comfilepi:compile_a_real-time_kernel:index



















