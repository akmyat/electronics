from machine import Pin

push_btn1_pin = 15   # Pull-up button
push_btn1 = Pin(push_btn1_pin, Pin.IN)

push_btn2_pin = 4   # Pull-down button
push_btn2 = Pin(push_btn2_pin, Pin.IN)

while True:
    push_btn1_state = push_btn1.value()
    if push_btn1_state == 1:
        print("Btn1: High")

    if push_btn1_state == 0:
        print("Btn2: Low")

    push_btn2_state = push_btn2.value()
    if push_btn2_state == 1:
        print("Btn2: High")

    if push_btn2_state == 0:
        print("Btn2: Low")
