





# sudo cp * /etc/systemd/system
# sudo systemctl disable --now safrany-gpio.service 2>/dev/null || true
# sudo rm -f /run/safranyGpio/* 2>/dev/null || true
# sudo systemctl start safrany-gpio.service
# tail -n5 /var/log/safranyGpio/gpio.csv


sudo systemctl daemon-reload
sudo systemctl enable --now safrany-gpio.timer
sudo systemctl restart safrany-gpio.service
sudo systemctl status safrany-gpio.service
journalctl -u safrany-gpio.service -b -n 100 --no-pager


sudo systemctl daemon-reload
sudo systemctl enable --now safrany-gpio-pullups.service
sudo systemctl status safrany-gpio-pullups.service
journalctl -u safrany-gpio-pullups.service -b -n 100 --no-pager


systemctl is-enabled safrany-gpio-pullups.service
systemctl is-enabled safrany-gpio.timer
systemctl list-timers | grep safrany
systemctl status safrany-gpio.timer
journalctl -b -u safrany-gpio.service --no-pager

