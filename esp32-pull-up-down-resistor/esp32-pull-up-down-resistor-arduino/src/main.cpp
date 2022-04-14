#include <Arduino.h>

const int push_btn1_pin = 15;   // Pull-up button
int push_btn1_state = 0;

const int push_btn2_pin = 4;    // Pull-down button
int push_btn2_state = 0;

void setup() {
    Serial.begin(115200);
    pinMode(push_btn1_pin, INPUT);
    pinMode(push_btn2_pin, INPUT);
}

void loop() {
    push_btn1_state = digitalRead(push_btn1_pin);

    if(push_btn1_state == HIGH){
        Serial.println("Btn1: High");
    }

    if(push_btn1_state == LOW){
        Serial.println("Btn1: Low");
    }

    push_btn2_state = digitalRead(push_btn2_pin);

    if(push_btn2_state == HIGH){
        Serial.println("Btn2: High");
    }

    if(push_btn2_state == LOW){
        Serial.println("Btn2: Low");
    }
}