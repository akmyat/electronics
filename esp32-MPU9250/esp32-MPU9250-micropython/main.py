import utime
from machine import I2C, Pin
from mpu9250 import MPU9250
from ak8963 import AK8963

i2c = I2C(scl=Pin(22), sda=Pin(21))

dummy = MPU9250(i2c) # this opens the bybass to access to the AK8963
ak8963 = AK8963(i2c)
offset, scale = ak8963.calibrate(count=256, delay=200)

sensor = MPU9250(i2c, ak8963=ak8963)

while True:
    print(sensor.acceleration)

    utime.sleep_ms(1000)