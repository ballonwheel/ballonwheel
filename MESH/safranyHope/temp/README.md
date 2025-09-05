


CPU load average – mi ez, hogyan „jön létre”?

A Linux /proc/loadavg három számot ad: 1, 5, 15 perces exponenciálisan csillapított mozgóátlagot a runnable (futásra kész) és uninterruptible sleep (I/O-ra váró) folyamatok számáról.

Nem „százalék”, hanem átlagos konkurens folyamatszám.

Pl. 1.00 1 percen: átlagban 1 futásra kész feladat volt.

Ha a gép 4 magos, akkor 4.00 körül „telített”, >4.00 már torlódás.

A három ablak (1/5/15) EMA (exponential moving average), így simított, nem pillanatkép. Pont ezért jó trendnek, nem tüskének.

Ha szeretnéd, a loggerbe tehetek normalized load oszlopot is (pl. load1/CPU_count), de a nyers load a szokásos.



imre@raspberrypiZ2w3:~/ballonwheel/MESH/safranyHope/temp/systemd $ sudo cp * /etc/systemd/system/
imre@raspberrypiZ2w3:~/ballonwheel/MESH/safranyHope/temp/systemd $ sudo systemctl daemon-reload
imre@raspberrypiZ2w3:~/ballonwheel/MESH/safranyHope/temp/systemd $ sudo systemctl enable --now safranytemp-sample.timer
Created symlink /etc/systemd/system/timers.target.wants/safranytemp-sample.timer → /etc/systemd/system/safranytemp-sample.timer.
imre@raspberrypiZ2w3:~/ballonwheel/MESH/safranyHope/temp/systemd $ sudo systemctl enable --now safranytemp-flush.timer
Created symlink /etc/systemd/system/timers.target.wants/safranytemp-flush.timer → /etc/systemd/system/safranytemp-flush.timer.
imre@raspberrypiZ2w3:~/ballonwheel/MESH/safranyHope/temp/systemd $ systemctl status safranytemp-sample.timer
● safranytemp-sample.timer - Run sampler every 10 seconds
     Loaded: loaded (/etc/systemd/system/safranytemp-sample.timer; enabled; vendor preset: enabled)
     Active: active (waiting) since Fri 2025-09-05 13:59:19 CEST; 18s ago
    Trigger: Fri 2025-09-05 13:59:39 CEST; 1s left
   Triggers: ● safranytemp-sample.service

Sep 05 13:59:19 raspberrypiZ2w3 systemd[1]: Started Run sampler every 10 seconds.
imre@raspberrypiZ2w3:~/ballonwheel/MESH/safranyHope/temp/systemd $ systemctl status safranytemp-flush.timer
● safranytemp-flush.timer - Run flush every 10 minutes
     Loaded: loaded (/etc/systemd/system/safranytemp-flush.timer; enabled; vendor preset: enabled)
     Active: active (waiting) since Fri 2025-09-05 13:59:29 CEST; 19s ago
    Trigger: Fri 2025-09-05 14:09:29 CEST; 9min left
   Triggers: ● safranytemp-flush.service

Sep 05 13:59:29 raspberrypiZ2w3 systemd[1]: Started Run flush every 10 minutes.
imre@raspberrypiZ2w3:~/ballonwheel/MESH/safranyHope/temp/systemd $ sudo systemctl start safranytemp-sample.service
imre@raspberrypiZ2w3:~/ballonwheel/MESH/safranyHope/temp/systemd $ sudo cat /run/safranyTemp/buffer.csv | tail -n 3
1757073592,27.000,58.000,40.780
1757073604,27.000,58.000,39.704
1757073615,27.000,58.000,39.704
imre@raspberrypiZ2w3:~/ballonwheel/MESH/safranyHope/temp/systemd $ sudo systemctl start safranytemp-flush.service
imre@raspberrypiZ2w3:~/ballonwheel/MESH/safranyHope/temp/systemd $ sudo tail -n 3 /var/log/safranyTemp/temp.csv
timestamp,dht_temp_c_avg,dht_rh_avg,cpu_temp_c_avg,sample_count,load1,load5,load15,root_used_pct,root_avail_gb
2025-09-05 13:59:29,26.000,58.000,40.780,1,1.04,0.42,0.35,12%,96.75
2025-09-05 14:00:37,27.000,58.000,40.421,6,0.52,0.39,0.35,12%,96.75
imre@raspberrypiZ2w3:~/ballonwheel/MESH/safranyHope/temp/systemd $ 

