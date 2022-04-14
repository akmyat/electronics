#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ThingSpeak.h>
#include <WiFi.h>

/* DHT22 */
#define DHTPIN 4
#define DHTTYPE DHT22 // DHT22 data pin
DHT dht(DHTPIN, DHTTYPE);

float readTemperature(){
    float temperature = dht.readTemperature();

    if(isnan(temperature)){
        Serial.println(F("Failed to read from DHT Sensor!"));
        return -50;
    }

    Serial.print(F("Temperature: "));
    Serial.print(temperature);
    Serial.println(F("°C"));

    return temperature;
}

float readHumidity(){
    float humidity = dht.readHumidity();

    if(isnan(humidity)){
        Serial.println(F("Failed to read from DHT Sensor!"));
        return -1;
    }

    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.println("%");

    return humidity;
}

/* WiFi */
const char* ssid = "ST7-14";
const char* password = "MayThazin11";
WiFiClient client;

void connectToWiFi(){
    if(WiFi.status() != WL_CONNECTED){
        Serial.print("Attempting to connect");
        while(WiFi.status() != WL_CONNECTED){
            WiFi.begin(ssid, password);
            delay(5000);
        }
        Serial.println("\nConnected.");
    }
}

/* Thing Speak */
unsigned long myChannelNumber = 1;
unsigned long temperatureFieldNumber = 1;
unsigned long humidityFieldNumber = 2;
const char* myWriteAPIKey = "5VIUK1UP923R8NG6";

void writeToThingSpeak(unsigned long fieldNumber, float value){
    int response = ThingSpeak.writeField(myChannelNumber, fieldNumber, value, myWriteAPIKey);
    if (response == 200){
        Serial.println("Channel update successful.");
    } else {
        Serial.println("Problem updating channel. HTTP error code"+ String(response));
    }
}

/* Deep Sleep */
#define uS_TO_S_FACTOR 1000000ULL
#define TIME_TO_SLEEP 600

void print_wakeup_reason(){
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason) {
        case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
        case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
        case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
        case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
        default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
    }
}

/* Timer */
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void setup() {
    Serial.begin(115200);

    Serial.println("DHT22 Started.");
    dht.begin();

    WiFi.mode(WIFI_STA);
    connectToWiFi();

    ThingSpeak.begin(client);

    float temperature = readTemperature();
    writeToThingSpeak(temperatureFieldNumber, temperature);

    delay(60000);

    float humidity = readHumidity();
    writeToThingSpeak(humidityFieldNumber, humidity);

    print_wakeup_reason();
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    Serial.println("Setup ESP32 to sleep for every" + String(TIME_TO_SLEEP) + "Seconds");

    // Setup to go to deep sleep
    Serial.println("Going to sleep now");
    Serial.flush();
    esp_deep_sleep_start();
}

void loop() {
}