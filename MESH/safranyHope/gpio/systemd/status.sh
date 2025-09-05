


systemctl is-enabled safrany-gpio-pullups.service
systemctl is-enabled safrany-gpio.timer
systemctl list-timers | grep safrany
systemctl status safrany-gpio.timer
journalctl -b -u safrany-gpio.service --no-pager

#0imre@raspberrypiZ2w3:/etc/systemd/system $ cat /run/safranyGpio/relay_
#relay_hw.last     relay_logic.last  
#imre@raspberrypiZ2w3:/etc/systemd/system $ cat /run/safranyGpio/relay_logic.last 
