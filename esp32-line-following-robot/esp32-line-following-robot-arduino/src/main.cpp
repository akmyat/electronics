#include <Arduino.h>

int DEBUG = LOW;

// Black Line Follower
const int IR1 = 2; // Right Sensor
const int IR2 = 4; // Left Sensor

// Motor Right
const int enA = 19;
const int motorApin1 = 18;
const int motorApin2 = 5;

// Motor Left
const int enB = 13;
const int motorBpin1 = 12;
const int motorBpin2 = 14;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel1 = 0;
const int pwmChannel2 = 2;
const int resolution = 8;

void setup() {
    pinMode(enA, OUTPUT);
    pinMode(motorApin1, OUTPUT);
    pinMode(motorApin2, OUTPUT);

    pinMode(enB, OUTPUT);
    pinMode(motorBpin1, OUTPUT);
    pinMode(motorBpin2, OUTPUT);

    ledcSetup(pwmChannel1, freq, resolution);
    ledcAttachPin(enA, pwmChannel1);

    ledcSetup(pwmChannel2, freq, resolution);
    ledcAttachPin(enB, pwmChannel2);

    pinMode(IR1, INPUT);
    pinMode(IR2, INPUT);

    if(DEBUG == HIGH){
        Serial.begin(115200);
    }
}

void loop() {
    int ir1_value = digitalRead(IR1);
    int ir2_value = digitalRead(IR2);

    if(DEBUG == HIGH){
        Serial.println(ir1_value);
        Serial.println(ir2_value);
        Serial.println();
    }

    // IR not on Black Line
    if(ir1_value == LOW && ir2_value == LOW){
        // Stop both motors
        digitalWrite(motorApin1, LOW);
        digitalWrite(motorApin2, LOW);
        ledcWrite(pwmChannel1, 0);

        digitalWrite(motorBpin1, LOW);
        digitalWrite(motorBpin2, LOW);
        ledcWrite(pwmChannel2, 0);
    }
    // IR on Black Line
    else if(ir1_value == HIGH && ir2_value == HIGH) {
        // Move the motors forward
        if(DEBUG == HIGH){
            Serial.println("Forward");
        }

        digitalWrite(motorApin1, HIGH);
        digitalWrite(motorApin2, LOW);
        ledcWrite(pwmChannel1, 40);

        digitalWrite(motorBpin1, HIGH);
        digitalWrite(motorBpin2, LOW);
        ledcWrite(pwmChannel2, 40);
    }
    // Left IR is not on Black
    else if(ir1_value == LOW && ir2_value == HIGH){
        // Turn Right
        if(DEBUG == HIGH){
            Serial.println("Right");
        }
        digitalWrite(motorApin1, HIGH);
        digitalWrite(motorApin2, LOW);
        ledcWrite(pwmChannel1, 40);

        digitalWrite(motorBpin1, LOW);
        digitalWrite(motorBpin2, HIGH);
        ledcWrite(pwmChannel2, 20);
    }
    // Right IR is not on Black
    else if(ir1_value == HIGH && ir2_value == LOW){
        // Turn left
        if(DEBUG == HIGH){
            Serial.println("Left");
        }
        digitalWrite(motorApin1, LOW);
        digitalWrite(motorApin2, HIGH);
        ledcWrite(pwmChannel1, 20);

        digitalWrite(motorBpin1, HIGH);
        digitalWrite(motorBpin2, LOW);
        ledcWrite(pwmChannel2, 40);
    }
    // Stop both the motors
    else {
        // Stop both motors
        digitalWrite(motorApin1, LOW);
        digitalWrite(motorApin2, LOW);
        ledcWrite(pwmChannel1, 0);

        digitalWrite(motorBpin1, LOW);
        digitalWrite(motorBpin2, LOW);
        ledcWrite(pwmChannel2, 0);
    }
}