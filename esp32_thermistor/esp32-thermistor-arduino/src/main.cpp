#include <Arduino.h>

const int thermistor_pin = 34;
const int r1 = 1000.0;

double steinhartTemperatureC(double r, double r0=1000.0, double t0=25.0, double beta=3950.0){
    double steinhart;
    steinhart = log(r / r0) / beta;
    steinhart += 1.0 / (t0 + 273.15);
    steinhart = (1.0 / steinhart) - 273.15;
    return steinhart;
}

void setup() {
    Serial.begin(115200);

    pinMode(thermistor_pin, INPUT);
}

void loop() {
    int v0 = analogRead(thermistor_pin);
    double r2 = r1 * (4096.0 / v0 - 1.0);
    Serial.print("Thermistor resistance: ");
    Serial.print(r2);
    Serial.println("Ohms");

    double temperature_c = steinhartTemperatureC(r2);
    Serial.print("Temperature in C: ");
    Serial.println(temperature_c);

    Serial.println();
    delay(1000);
}
