

# imre@raspberrypiZ2w3:~/ballonwheel/dht11/pythonsolution $ sudo pip3 install RPi.GPIO
# sudo apt-get install python3-pip -y
# imre@raspberrypiZ2w3:~/ballonwheel/dht11/pythonsolution $ sudo pip3 install dht11


import RPi.GPIO as GPIO
import dht11

# initialize GPIO
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.cleanup()

# read data using pin 14
instance = dht11.DHT11(pin = 22)
result = instance.read()

if result.is_valid():
    print("Temperature: %-3.1f C" % result.temperature)
    print("Humidity: %-3.1f %%" % result.humidity)
else:
    print("Error: %d" % result.error_code)
