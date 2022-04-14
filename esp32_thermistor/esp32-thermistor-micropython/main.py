import math
from machine import Pin, ADC
from time import sleep

thermistor_pin = 34
r1 = 1000.0


def steinhart_temperature_c(r, r0=1000, t0=25.0, beta=3950.0):
    steinhart = math.log(r / r0) / beta
    steinhart += 1.0 / (t0 + 273.15)
    steinhart = (1.0 / steinhart) - 273.15
    return steinhart


thermistor = ADC(Pin(thermistor_pin))
thermistor.atten(ADC.ATTN_11DB)

while True:
    v0 = thermistor.read()
    r2 = r1 * (4096.0 / v0 - 1.0)
    print('Thermistor resistance: {} ohms'.format(r2))

    temperature_c = steinhart_temperature_c(r2)
    print('Temperature in C: {}'.format(temperature_c))

    print("\n")
    sleep(1)
