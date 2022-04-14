#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_MAX31855.h"

#define MAXDO 12
#define MAXCS 13
#define MAXCLK 14

Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

void setup() {
    Serial.begin(115200);

    Serial.println("MAX31855 test");
    thermocouple.begin();
    delay(500);
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
}