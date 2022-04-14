from dc_motor import DCMotor
from machine import Pin, PWM

frequency = 15000

# Right Motor
motor1_pin1 = Pin(12, Pin.OUT)
motor1_pin2 = Pin(14, Pin.OUT)
motor1_enable = PWM(Pin(13), frequency)
dc_motor1 = DCMotor(pin1=motor1_pin1, pin2=motor1_pin2, enable_pin=motor1_enable)

# Left Motor
motor2_pin1 = Pin(18, Pin.OUT)
motor2_pin2 = Pin(5, Pin.OUT)
motor2_enable = PWM(Pin(19), frequency)
dc_motor2 = DCMotor(pin1=motor2_pin1, pin2=motor2_pin2, enable_pin=motor2_enable)

# Left IR sensor
left_ir = Pin(2)

# Right IR sensor
right_ir = Pin(4)

while True:
    left_ir_value = left_ir.value()
    right_ir_value = right_ir.value()

    if left_ir_value == 0 and right_ir_value == 0:
        dc_motor1.stop()
        dc_motor2.stop()
    elif left_ir_value == 1 and right_ir_value == 1:
        dc_motor1.forward(100)
        dc_motor2.forward(100)
    elif left_ir_value == 0 and right_ir_value == 1:
        dc_motor1.forward(100)
        dc_motor2.backward(50)
    elif left_ir_value == 1 and right_ir_value == 0:
        dc_motor1.forward(50)
        dc_motor2.backward(100)
    else:
        dc_motor1.stop()
        dc_motor2.stop()
