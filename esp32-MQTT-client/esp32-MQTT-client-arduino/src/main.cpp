#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <WiFiClientSecure.h>

/* DHT 22 */
#define DHTTYPE DHT22
#define DHTPIN 4
DHT dht(DHTPIN, DHTTYPE);

/* LED */
const int lamp = 5;

/* SSL/TLS Certificates */
const char* ca_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIEKTCCAxGgAwIBAgIUAsdIQ1UAIHu2hZTarRDf9uCCx1cwDQYJKoZIhvcNAQEL\n" \
"sbh8yDh2fkbAt+SuHw==\n" \
"-----END CERTIFICATE-----\n";

const char* device001_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIID3DCCAsQCFCJP86+Lpr1/kUsiExIIsOX665/bMA0GCSqGSIb3DQEBCwUAMIGj\n" \
"I+3a7cikV7Fg6T55bp+7PrnthFD/edKkgF8LXhEQO5YwDDAKBggrBgEFBQcDAg==\n" \
"-----END CERTIFICATE-----\n";

const char* device001_key = \
"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEowIBAAKCAQEAuLYKI9knpSlkSVEfgm+YFaX7tplUPsQxAf/sygYey8tgGBIt\n" \
"Ui29h6AzMZ/M8bG5uPIqLp0gpir1ulbI+IPEY7AnplBb12kG8G7e\n" \
"-----END RSA PRIVATE KEY-----\n";

/* PubSubClient */
// WiFi
const char* ssid = "SSID";
const char* password = "PASSWORD";

// MQTT Server
const char* mqtt_server = "IP_ADDRESS";

//WiFiClient espClient;
WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup_wifi(){
    /*
     * Set up WiFi
     */
    delay(10);

    Serial.println();
    Serial.print("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("WiFi connected - ESP IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(String topic, byte* message, unsigned int length){
    /*
     * When some deive publishes a message, do something
     */
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for(int i=0; i<length; i++){
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();

    // if message received on the topic room/lamp
    if(topic == "room/lamp"){
        Serial.print("Changing Room lamp to");
        if(messageTemp == "on"){
            digitalWrite(lamp, HIGH);
            Serial.println("ON");
        } else if (messageTemp == "off"){
            digitalWrite(lamp, LOW);
            Serial.println("Off");
        }
    }
    Serial.println();
}

void reconnect(){
    while(!client.connected()){
        Serial.print("Attempting MQTT connection...");

        if(client.connect("ESP32Client")){
            Serial.println("Connected");
            client.subscribe("room/lamp");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println("try again in 5 seconds");
            delay(5000);
        }
    }
}

// Timers
long now = millis();
long lastMeasure = 0;

void setup() {
    Serial.begin(115200);

    Serial.println(F("DHT 22 test"));
    dht.begin();

    pinMode(lamp, OUTPUT);

    setup_wifi();

    /* Set SSL/TLS certificate */
    espClient.setCACert(ca_cert);
    espClient.setCertificate(device001_cert);
    espClient.setPrivateKey(device001_key);

    client.setServer(mqtt_server, 8883);
    client.setCallback(callback);
}

void loop() {
    if(!client.connected()){
        reconnect();
    }
    if(!client.loop()){
        client.connect("ESP32Client");
    }

    now = millis();
    if(now - lastMeasure > 10000){
        lastMeasure = now;

        float h = dht.readHumidity();
        float t = dht.readTemperature();
        float f = dht.readTemperature(true);

        if(isnan(h) || isnan(t) || isnan(f)){
            Serial.println(F("Failed to read from DHT sensor!"));
            return;
        }

        float hic = dht.computeHeatIndex(t, h, false);
        static char temperatureTemp[7];
        dtostrf(hic, 6, 2, temperatureTemp);

        static char humidityTemp[7];
        dtostrf(h, 6, 2, humidityTemp);

        client.publish("room/temperature", temperatureTemp);
        client.publish("room/humidity", humidityTemp);

        Serial.print("Humidity: ");
        Serial.print(h);
        Serial.print("%\t Temperature: ");
        Serial.print(t);
        Serial.print("*C");
        Serial.print(f);
        Serial.print("*F\t Heat Index: ");
        Serial.print(hic);
        Serial.println("*C");
    }
}
