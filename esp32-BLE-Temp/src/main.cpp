#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEService.h>
#include <BLEUtils.h>
#include <BLE2902.h>

/* DHT 22 */
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

/* BLE Server */
#define bleServerName "AKM DHT22 Sensor"

BLECharacteristic temperatureCharacteristic(BLEUUID((uint16_t)0x2A6E), BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor temperatureDescriptor(BLEUUID((uint16_t) 0x2901));
BLEDescriptor temperatureValueDescriptor(BLEUUID((uint16_t) 0x2902));

BLECharacteristic humidityCharacteristic(BLEUUID((uint16_t)0x2A6F), BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor humidityDescriptor(BLEUUID((uint16_t)0x2901));
BLEDescriptor humidityValueDescriptor(BLEUUID((uint16_t)0x2902));

bool deviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer *bleServer){
        deviceConnected = true;
    }
    void onDisconnect(BLEServer *bleServer){
        deviceConnected = false;
    }
};

/* Timer */
unsigned int currMillis = 0;
unsigned int previousMillis = 0;
const int UPDATE_INTERVAL = 2000;

void setup() {
    Serial.begin(115200);

    Serial.println(F("DHT22 test!"));
    dht.begin();

    BLEDevice::init(bleServerName);

    BLEServer *bleServer = BLEDevice::createServer();
    bleServer->setCallbacks(new MyServerCallbacks());

    BLEService *environmentService = bleServer->createService(BLEUUID((uint16_t)0x181A));

    temperatureDescriptor.setValue("Range from -40 to +250 degree Celsius");
    temperatureCharacteristic.addDescriptor(&temperatureDescriptor);
    temperatureCharacteristic.addDescriptor(&temperatureValueDescriptor);
    environmentService->addCharacteristic(&temperatureCharacteristic);

    humidityDescriptor.setValue("Range from 0% to 100%");
    humidityCharacteristic.addDescriptor(&humidityDescriptor);
    humidityCharacteristic.addDescriptor(&humidityValueDescriptor);
    environmentService->addCharacteristic(&humidityCharacteristic);

    environmentService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(BLEUUID((uint16_t)0x181A));
    bleServer->getAdvertising()->start();
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");
}

void loop() {
    if(deviceConnected) {
        currMillis = millis();
        if(currMillis - previousMillis > UPDATE_INTERVAL){
            // Read Humidity
            float humidity = dht.readHumidity();

            // Read Temperature
            float temperature = dht.readTemperature();

            // Check if any reads failed
            if(isnan(humidity) || isnan(temperature)){
                Serial.println(F("Failed to read from DHT sensor!"));
                return;
            }

            Serial.print("Temperature in C: ");
            Serial.print(temperature);
            Serial.print(" Humidity: ");
            Serial.println(humidity);

//            static char temperatureTemp[6];
//            dtostrf(temperature, 6, 2, temperatureTemp);
            uint8_t temperatureData[2];
            uint16_t temperatureValue;
            temperatureValue = (uint16_t)(temperature * 100);
            temperatureData[0] = temperatureValue;
            temperatureData[1] = temperatureValue>>8;
            temperatureCharacteristic.setValue(temperatureData,2);
            temperatureCharacteristic.notify();

            uint8_t humidityData[2];
            uint16_t humidityValue;
            humidityValue = (uint16_t) (humidity * 100);
            humidityData[0] = humidityValue;
            humidityData[1] = humidityValue>>8;
            humidityCharacteristic.setValue(humidityData,2);
            humidityCharacteristic.notify();

            previousMillis = currMillis;
        }
    }
}