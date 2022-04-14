from dc_motor import DCMotor
from machine import Pin, PWM
from time import sleep

frequency = 15000

motor1_pin1 = Pin(12, Pin.OUT)
motor1_pin2 = Pin(14, Pin.OUT)
motor1_enable = PWM(Pin(13), frequency)
dc_motor1 = DCMotor(pin1=motor1_pin1, pin2=motor1_pin2, enable_pin=motor1_enable)

motor2_pin1 = Pin(18, Pin.OUT)
motor2_pin2 = Pin(5, Pin.OUT)
motor2_enable = PWM(Pin(19), frequency)
dc_motor2 = DCMotor(pin1=motor2_pin1, pin2=motor2_pin2, enable_pin=motor2_enable)

while True:
    # Move motors to Forward
    print("Moving Forward")
    dc_motor1.forward(100)
    dc_motor2.forward(100)
    sleep(10)

    # Move motors to Backward
    print("Moving Backward")
    dc_motor1.backward(100)
    dc_motor2.backward(100)
    sleep(10)

    # Move motors to Left
    print("Moving Left")
    dc_motor1.forward(100)
    dc_motor2.backward(50)
    sleep(10)

    # Move motors to Right
    print("Moving Right")
    dc_motor1.backward(50)
    dc_motor2.forward(100)
    sleep(10)
