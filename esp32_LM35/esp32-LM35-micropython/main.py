from machine import Pin, ADC
from time import sleep

lm35_pin = 34
lm35 = ADC(Pin(lm35_pin))
lm35.width(ADC.WIDTH_12BIT)
lm35.atten(ADC.ATTN_11DB)

def read_tempc():
    lm35_value = lm35.read()
    voltage = (lm35_value / 4096.0) * 3300
    tempc = voltage * 0.1
    return tempc


def read_tempf():
    lm35_value = lm35.read()
    voltage = (lm35_value / 4096.0) * 3300
    tempc = voltage * 0.1
    tempf = (tempc * 1.8) + 32
    return tempf


while True:
    avg_c = 0.0
    avg_f = 0.0
    for i in range(100):
        c = read_tempc()
        avg_c += c

        f = read_tempf()
        avg_f += f

        sleep(0.2)

    avg_c /= 100
    avg_f /= 100
    print('Temperature in C = {}'.format(avg_c))
    print('Temperature in F = {}'.format(avg_f))
    print("\n")
