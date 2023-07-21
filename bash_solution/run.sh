#!/bin/bash


date &> log.run
#echo on
#set -x &>> log.run

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


#ket fajl van
#bow_pos --> UART
#bow_motor --> UART
#frame: [frame id][char1 char2 char3]
#ahol minden adat uint8_t
date &> log.startup
./startup.sh &>> log.startup

#octave ciklus:
#var amig nincs bow_pos
#felovassa a bow_pos-t
#torli a bow_pos-t
 #frame check(not iplemented yet): ha nem a kikuldottet kapja vissza 200szor(20db*10msec=200msec), akkor
   #eldobja az adatot es uj dummy frame-t kuld 100db*10ms=1sec-ig
   #egyebkent szamol (ket parameter SET (ts=10ms es ts=20ms, es adaptalodik, ha hiba van?)
#szamol control alg (min-max 1msec??)
#letrehozza a bow_motort
#ciklus vege
sleep 1
date &> log.octave
#(nice --20 octave bow_octave.m &>> log.octave) &
nice --20 octave bow_octave.m &
pid_octave=$!

sleep 1
date &> log.a
(nice --20 ./a.out &>> log.a)&
pid_a=$!

#get PID
echo "$pid_octave" > stop_pid_octave
echo "$pid_a" > stop_pid_a
#echo "PID of this script: $$"
#echo "PPID of this script: $PPID"
#echo "UID of this script: $UID"


#RT improve, tesztelni kell
#octave egyik proc, a.out masik proc
taskset -cp 3 $pid_a &>> log.run
#sudo chrt -v -f -p 50 $pid_a &>> log.run # milyen prio legyen??
#ezt inkabb ne: sudo sh -c "echo -1 > /proc/sys/kernel/sched_rt_runtime_us"

#RT kernel
#http://comfilewiki.co.kr/en/doku.php?id=comfilepi:compile_a_real-time_kernel:index



















