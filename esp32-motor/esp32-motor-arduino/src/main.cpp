#include <Arduino.h>

const int motor1Pin1 = 18;
const int motor1Pin2 = 5;
const int motor1enable = 19;

const int motor2Pin1 = 12;
const int motor2Pin2 = 14;
const int motor2enable = 13;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel1 = 0;
const int pwmChannel2 = 2;
const int resolution = 8;
int dutyCycle = 200;

void setup() {
    Serial.begin(115200);

    // Sets the pins as outputs
    pinMode(motor1enable, OUTPUT);
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);

    pinMode(motor2enable, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);

    ledcSetup(pwmChannel1, freq, resolution);
    ledcAttachPin(motor1enable, pwmChannel1);

    ledcSetup(pwmChannel2, freq, resolution);
    ledcAttachPin(motor2enable, pwmChannel2);
}

void loop() {
    ledcWrite(pwmChannel1, dutyCycle);
    ledcWrite(pwmChannel2, dutyCycle);

    // Move motors forward at max speed
    Serial.println("Moving Forward");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    delay(3000);

    //Stop motors
    Serial.println("Motor Stopped");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    delay(3000);

    // Move motors backward at max speed
    Serial.println("Moving Backward");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    delay(3000);

    // Move motors right at max speed
    Serial.println("Moving Right");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    delay(3000);

    // Move motors left at max speed
    Serial.println("Moving Left");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    delay(3000);

    // Move Forward with increasing duty Cycle
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    while (dutyCycle <= 255) {
        ledcWrite(pwmChannel1, dutyCycle);
        ledcWrite(pwmChannel2, dutyCycle);
        Serial.print("Forward with duty cycle: ");
        Serial.println(dutyCycle);

        dutyCycle += 5;
        delay(500);
    }
    dutyCycle = 255;

    // Move Forward with decreasing duty Cycle
    while(dutyCycle > 200) {
        ledcWrite(pwmChannel1, dutyCycle);
        ledcWrite(pwmChannel2, dutyCycle);
        Serial.print("Forward with duty cycle: ");
        Serial.println(dutyCycle);

        dutyCycle -= 5;
        delay(500);
    }
    dutyCycle = 200;
}