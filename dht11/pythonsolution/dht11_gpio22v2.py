#!/usr/bin/env python3
import pigpio, time

# sudo apt-get update
# sudo apt-get install -y python3-pip python3-dev pigpio
# sudo pip3 install pigpio
# pigpio démon indítása (precíz időzítéshez kell)
# sudo systemctl enable --now pigpio
# sudo systemctl disable --now pigpiod
# sudo apt-get purge pigpio
# systemctl status pigpiod
# sudo python3 dht11_gpio22v2.py

PIN = 22        # BCM22 (physical 15)
THRESH_US = 50  # ~0 bit ~26-28 µs HIGH, 1 bit ~70 µs HIGH → küszöb ~50 µs

class DHT11:
    def __init__(self, pi, pin=PIN):
        self.pi = pi
        self.pin = pin
        self.cb = None
        self.highs = []
        self._last_tick = None
        self._last_level = 1
        self._done = False

    def _cb(self, gpio, level, tick):
        # level: 0=LOW, 1=HIGH, 2=WATCHDOG timeout
        if level == 2:
            self._done = True
            return

        if self._last_tick is None:
            self._last_tick = tick
            self._last_level = level
            return

        dt = pigpio.tickDiff(self._last_tick, tick)  # µs
        # Ha most LEFELÉ ment (1->0), akkor a megelőző HIGH hossza dt
        if self._last_level == 1 and level == 0:
            self.highs.append(dt)

        self._last_tick = tick
        self._last_level = level

    def read(self, retries=5):
        for _ in range(retries):
            self.highs.clear()
            self._last_tick = None
            self._last_level = 1
            self._done = False

            # Start jel: 18 ms LOW, majd gyors HIGH, és INPUT mód
            self.pi.set_mode(self.pin, pigpio.OUTPUT)
            self.pi.write(self.pin, 1)
            time.sleep(0.05)
            self.pi.write(self.pin, 0)
            time.sleep(0.018)
            self.pi.write(self.pin, 1)
            self.pi.set_mode(self.pin, pigpio.INPUT)

            # Ha nincs külső felhúzó a modulon, használj belső felhúzást:
            # self.pi.set_pull_up_down(self.pin, pigpio.PUD_UP)
            # Ha a modulon VAN felhúzó, maradhat kikapcsolva:
            self.pi.set_pull_up_down(self.pin, pigpio.PUD_OFF)

            # Callback + watchdog (100 ms)
            if self.cb is not None:
                self.cb.cancel()
            self.cb = self.pi.callback(self.pin, pigpio.EITHER_EDGE, self._cb)
            self.pi.set_watchdog(self.pin, 100)

            # Várunk, amíg vége a keretnek (watchdog zárja le), vagy összejön 42+ HIGH
            t0 = time.time()
            while not self._done and (time.time() - t0) < 0.2:
                # tipikusan 40 HIGH jön, de a kezdeti kézfogás miatt több is lehet
                if len(self.highs) >= 45:
                    # lesz elég adat, a watchdog úgyis leállítja rögtön
                    pass
                time.sleep(0.001)

            # Lezárás
            self.pi.set_watchdog(self.pin, 0)
            if self.cb is not None:
                self.cb.cancel()
                self.cb = None

            if len(self.highs) >= 40:
                bits = []
                # A legvégén lévő 40 HIGH idő az érdemi adat
                for us in self.highs[-40:]:
                    bits.append(1 if us > THRESH_US else 0)

                # 5 byte: RH_int, RH_dec, T_int, T_dec, checksum
                by = []
                for i in range(5):
                    v = 0
                    for b in bits[i*8:(i+1)*8]:
                        v = (v << 1) | b
                    by.append(v)

                rh_i, rh_d, t_i, t_d, chk = by
                if ((rh_i + rh_d + t_i + t_d) & 0xFF) == chk:
                    # DHT11-nél rh_d és t_d többnyire 0
                    return float(rh_i), float(t_i)

            # kis pihi és újrapróba
            time.sleep(0.2)

        raise RuntimeError("DHT11 read failed (pigpio)")

if __name__ == "__main__":
    pi = pigpio.pi()
    if not pi.connected:
        raise RuntimeError("pigpio daemon not running (sudo systemctl enable --now pigpiod)")

    sensor = DHT11(pi, PIN)
    try:
        while True:
            rh, t = sensor.read()
            print(f"RH={rh:.0f}%  T={t:.0f}°C")
            time.sleep(1.0)
    except KeyboardInterrupt:
        pass
    finally:
        pi.stop()

