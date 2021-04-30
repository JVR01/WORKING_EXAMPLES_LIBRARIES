import time
import board
import busio
import adafruit_ccs811
from digitalio import DigitalInOut, Direction
#https://pi-buch.info/co2-ampel-mit-dem-raspberry-pico-und-circuitpython/ 
# LED setup.

ledgreen = DigitalInOut(board.GP2)
ledyellow = DigitalInOut(board.GP3)
ledred = DigitalInOut(board.GP4)

ledgreen.direction = Direction.OUTPUT
ledyellow.direction = Direction.OUTPUT
ledred.direction = Direction.OUTPUT
 
#I2C setup 
i2c = busio.I2C(scl=board.GP17, sda=board.GP16)
ccs811 = adafruit_ccs811.CCS811(i2c)
 
# Wait for the sensor to be ready
while not ccs811.data_ready:
    pass
 
while True:
    print("CO2: {} PPM, TVOC: {} PPM".format(ccs811.eco2, ccs811.tvoc))
    
    if ccs811.eco2 < 1000: 
        ledgreen.value = True 
        ledyellow.value = False 
        ledred.value = False 
    elif ccs811.eco2 > 999 and ccs811.eco2 < 1999:
        ledgreen.value = False
        ledyellow.value = True
        ledred.value = False
    else:
        ledgreen.value = False
        ledyellow.value = False
        ledred.value = True
    time.sleep(1)
