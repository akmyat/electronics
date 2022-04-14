#include <Arduino.h>

const int source_led_pin = 12;
int source_led_state = 0;

const int sink_led_pin = 13;
int sink_led_state = 0;

void setup() {
    Serial.begin(115200);
    pinMode(source_led_pin, OUTPUT);
    pinMode(sink_led_pin, OUTPUT);

    digitalWrite(source_led_pin, LOW);
    digitalWrite(sink_led_pin, HIGH);
}

void loop() {
    digitalWrite(source_led_pin, HIGH);
    source_led_state = 1;
    Serial.print("Source LED: ");
    Serial.println(source_led_state);
    delay(500);
    digitalWrite(source_led_pin, LOW);
    source_led_state = 0;
    Serial.print("Source LED: ");
    Serial.println(source_led_state);
    delay(500);

    digitalWrite(sink_led_pin, LOW);
    sink_led_state = 0;
    Serial.print("Source LED: ");
    Serial.println(sink_led_state);
    delay(500);
    digitalWrite(sink_led_pin, HIGH);
    sink_led_state = 0;
    Serial.print("Source LED: ");
    Serial.println(sink_led_state);
    delay(500);
}