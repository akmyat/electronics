from machine import Pin
from time import sleep

source_led_pin = 12
source_led = Pin(source_led_pin, Pin.OUT)
sink_led_pin = 13
sink_led = Pin(sink_led_pin, Pin.OUT)

while True:
    source_led.off()
    source_led_state = source_led.value()
    print("Source LED: {}".format(source_led_state))
    sleep(1)
    source_led.on()
    source_led_state = source_led.value()
    print("Source LED: {}".format(source_led_state))

    sink_led.on()
    sink_led_state = sink_led.value()
    print("Sink LED: {}".format(sink_led_state))
    sleep(1)
    sink_led.off()
    sink_led_state = sink_led.value()
    print("Sink LED: {}".format(sink_led_state))

    print("\n")
