#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_MAX31855.h"

#define MAXDO 12
#define MAXCS 13
#define MAXCLK 14

Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

#define REDLED 5
#define GREENLED 16
#define YELLOWLED 4

#define RELAY 2

void setup() {
    Serial.begin(115200);

    Serial.println("MAX31855 test");
    thermocouple.begin();
    delay(500);

    pinMode(REDLED, OUTPUT);
    pinMode(GREENLED, OUTPUT);
    pinMode(YELLOWLED, OUTPUT);

    pinMode(RELAY, OUTPUT);
}

void loop() {
    double i = thermocouple.readInternal();
    double c = thermocouple.readCelsius();
    double f = thermocouple.readFahrenheit();

    if(isnan(i) || isnan(c) || isnan(f)){
        Serial.println(F("Failed to read from MAX31855 Sensor!"));
        return;
    }

    Serial.print(F("Temperature in C = "));
    Serial.println(c);
    Serial.print(F("Temperature in F = "));
    Serial.println(f);
    Serial.println();
    delay(1000);

    if (c >= 343) {
        digitalWrite(REDLED, HIGH);
        digitalWrite(GREENLED, LOW);
        digitalWrite(YELLOWLED, LOW);

        digitalWrite(RELAY, HIGH);

        Serial.print("Temperature in C: ");
        Serial.println(c);
        Serial.println("Temperature is High");
        Serial.println("Turn OFF Soldering Iron");
    } else if(c <= 316) {
        digitalWrite(REDLED, LOW);
        digitalWrite(GREENLED, LOW);
        digitalWrite(YELLOWLED, HIGH);

        digitalWrite(RELAY, LOW);

        Serial.print("Temperature in C: ");
        Serial.println(c);
        Serial.println("Temperature is LOW");
        Serial.println("Heating Soldering Iron");
    } else {
        digitalWrite(REDLED, LOW);
        digitalWrite(GREENLED, HIGH);
        digitalWrite(YELLOWLED, LOW);

        digitalWrite(RELAY, LOW);

        Serial.print("Temperature in C: ");
        Serial.println(c);
        Serial.println("Temperature is Normal");
    }
}