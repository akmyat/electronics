#include <Arduino.h>

#define lm35_pin 34

double readTempC(){
    int lm35_value = analogRead(lm35_pin);
    double voltage = (lm35_value / 4096.0) * 3300;
    double tempC = voltage * 0.1;
    return tempC;
}

double readTempF(){
    int lm35_value = analogRead(lm35_pin);
    double voltage = (lm35_value / 4096.0) * 3300;
    double tempC = voltage * 0.1;
    double tempF = (tempC * 1.8) + 32;
    return tempF;
}

void setup() {
    Serial.begin(115200);
    pinMode(lm35_pin, INPUT);
}

void loop() {
    double avg_c = 0.0;
    double avg_f = 0.0;

    for(int i = 0; i < 100; i++){
        double c = readTempC();
        avg_c += c;

        double f = readTempF();
        avg_f += f;

        delay(200);
    }

    avg_c /= 100;
    avg_f /= 100;
    Serial.print("Temperature in C = ");
    Serial.println(avg_c);
    Serial.print("Temperature in F = ");
    Serial.println(avg_f);
    Serial.println();
}