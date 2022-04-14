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
"BQAwgaMxCzAJBgNVBAYTAlRIMRQwEgYDVQQIDAtQYXRodW10aGFuaTEVMBMGA1UE\n" \
"BwwMS2hsb25nIEx1YW5nMSYwJAYDVQQKDB1Bc2lhbiBJbnN0aXR1dGUgb2YgVGVj\n" \
"aG5vbG9neTEMMAoGA1UECwwDSUNUMQwwCgYDVQQDDANJb1QxIzAhBgkqhkiG9w0B\n" \
"CQEWFGFrbS5ha215YXRAZ21haWwuY29tMB4XDTIyMDQwODA1MTY1MVoXDTMyMDQw\n" \
"NTA1MTY1MVowgaMxCzAJBgNVBAYTAlRIMRQwEgYDVQQIDAtQYXRodW10aGFuaTEV\n" \
"MBMGA1UEBwwMS2hsb25nIEx1YW5nMSYwJAYDVQQKDB1Bc2lhbiBJbnN0aXR1dGUg\n" \
"b2YgVGVjaG5vbG9neTEMMAoGA1UECwwDSUNUMQwwCgYDVQQDDANJb1QxIzAhBgkq\n" \
"hkiG9w0BCQEWFGFrbS5ha215YXRAZ21haWwuY29tMIIBIjANBgkqhkiG9w0BAQEF\n" \
"AAOCAQ8AMIIBCgKCAQEAq4vNc13ob0b1IWqCEQy3ZlBjo+1Iq4waeDPYXgkZb1Gp\n" \
"EE3Y/6Od5za6/8NaT4Luln4GBrsSpIv/PalMZ0Ebc77j33EpjhwF4q56eGunCNn4\n" \
"f6oFhfSZDBb64DLM4DxFXQkpZEGEf7P7OG0tG7kXJh0a66HfXU1krZ+gZL5IjQCL\n" \
"7eU3Sa3AQz4QqRrZ2Y907j3Apr5Fv1Ea29P0cbKYaSJwd85zzd+p93kmFWpk12Kb\n" \
"SPStNCLitoWHgXeAWDq+hpiPfKDFC0jFSyJqPhwPt45etwLhwJtx9duuKojrymp3\n" \
"1ZLNJZ7ZigRwmGUd0iQXTP+U7acOK2kZ6WkSJSF7tQIDAQABo1MwUTAdBgNVHQ4E\n" \
"FgQUBQnM5T8i0SxxBRglry4cFIBeOmUwHwYDVR0jBBgwFoAUBQnM5T8i0SxxBRgl\n" \
"ry4cFIBeOmUwDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAEJ+9\n" \
"c9XkktUtkersGlxzKF9M9NzjCY2+OJNiaWf1+s7WQuLPjY2VjdpzfqiA7KHlJzBK\n" \
"IDV7IB0giiPEJXHjVkq9Yd+cJsuW3WPWj4ZhjzGTv1XRNXbDbz1xwXdX9RQEH2SK\n" \
"WgZ/6cpishEqtc8qQxvEXJTKxpKA5AusqugqONUrKf+bKbC1HzrnvtdUqmenBQ9F\n" \
"7WaFMWyDPJ5VfrVKmErPOESJXFBrBTD3GwS0mU/5iXEby4yhYDmkynOIENpSdszO\n" \
"cYcI9rB76fYwR/v0TGBfqvg/MZP+TaJxSrEXjI8wXFy7Rdb50dh9H9xhDOh8U/jN\n" \
"sbh8yDh2fkbAt+SuHw==\n" \
"-----END CERTIFICATE-----\n";

const char* device001_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIID3DCCAsQCFCJP86+Lpr1/kUsiExIIsOX665/bMA0GCSqGSIb3DQEBCwUAMIGj\n" \
"MQswCQYDVQQGEwJUSDEUMBIGA1UECAwLUGF0aHVtdGhhbmkxFTATBgNVBAcMDEto\n" \
"bG9uZyBMdWFuZzEmMCQGA1UECgwdQXNpYW4gSW5zdGl0dXRlIG9mIFRlY2hub2xv\n" \
"Z3kxDDAKBgNVBAsMA0lDVDEMMAoGA1UEAwwDSW9UMSMwIQYJKoZIhvcNAQkBFhRh\n" \
"a20uYWtteWF0QGdtYWlsLmNvbTAeFw0yMjA0MDgwNTM3NTdaFw0zMjA0MDUwNTM3\n" \
"NTdaMIGwMQswCQYDVQQGEwJUSDEUMBIGA1UECAwLUGF0aHVtVGhhbmkxFTATBgNV\n" \
"BAcMDEtobG9uZyBMdWFuZzEnMCUGA1UECgweQXNpYW4gSW5zdGl0dXRlIG9mIFRl\n" \
"Y2hub2xvZ3lnMQwwCgYDVQQLDANJb1QxGDAWBgNVBAMMD01RVFQgQ0xJRU5UIDAw\n" \
"MTEjMCEGCSqGSIb3DQEJARYUYWttLmFrbXlhdEBnbWFpbC5jb20wggEiMA0GCSqG\n" \
"SIb3DQEBAQUAA4IBDwAwggEKAoIBAQC4tgoj2SelKWRJUR+Cb5gVpfu2mVQ+xDEB\n" \
"/+zKBh7Ly2AYEi3VA7foft1uPWgHdXMdNNdzgANWosQfacRe5mYDZcZKgnuelLEH\n" \
"vqio+aixpo+8WbgngUZoiYYNj1XkLkCwR1q20uaN1VUso/AyCTbCeyflELMoFzp/\n" \
"8tJw3Qr19UFYWNbNfcbrT9Vq0r+kLJe8C8M3tGbInPGpF8W16Xb4ioZEUOghrhL2\n" \
"PVuPjhFIhVWY9a0E8XU9UVe42beC+VWLXFG16r1gPs6jxShHf7L9RdLns+7C7rUR\n" \
"RSV77mXfv15lTbQzbSd8lZImGDwAaU+SxbA7liUM0BUuSl+p0flvAgMBAAEwDQYJ\n" \
"KoZIhvcNAQELBQADggEBAFMDh+dXkurjtA4IVjVeRNJVs4Gs1u2WdYfVseSjC6is\n" \
"e26axfQO5qkNqWeeziSvu9CQP/0aJ7pJhHvPudUxqUY4c4XK4boh3W4ZjrgHdvfk\n" \
"N/rAPzUOICGccqKw2u+UTud4XTngKm/mDc5hmN8CYDTVAlCyvRkCozST3a+RyX1X\n" \
"0P0fjL64gO0vAwcvvpnYjHkOVhaBMnlNqC/c8C7qQaNgi/SS1jKUTnD7k25WEYhK\n" \
"HnTrRr91EKFz2w3TV0fh5z8pjpeoW9TbFg+IACHIF7KCMYGyMYZuSSNTC3jDzD4V\n" \
"I+3a7cikV7Fg6T55bp+7PrnthFD/edKkgF8LXhEQO5YwDDAKBggrBgEFBQcDAg==\n" \
"-----END CERTIFICATE-----\n";

const char* device001_key = \
"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEowIBAAKCAQEAuLYKI9knpSlkSVEfgm+YFaX7tplUPsQxAf/sygYey8tgGBIt\n" \
"1QO36H7dbj1oB3VzHTTXc4ADVqLEH2nEXuZmA2XGSoJ7npSxB76oqPmosaaPvFm4\n" \
"J4FGaImGDY9V5C5AsEdattLmjdVVLKPwMgk2wnsn5RCzKBc6f/LScN0K9fVBWFjW\n" \
"zX3G60/VatK/pCyXvAvDN7RmyJzxqRfFtel2+IqGRFDoIa4S9j1bj44RSIVVmPWt\n" \
"BPF1PVFXuNm3gvlVi1xRteq9YD7Oo8UoR3+y/UXS57Puwu61EUUle+5l379eZU20\n" \
"M20nfJWSJhg8AGlPksWwO5YlDNAVLkpfqdH5bwIDAQABAoIBAF1gkdG7lwI04aD6\n" \
"5M4A8kaHaa/O5brxXcEgCym6XFk0UDj6/7Z/MbrUX8a6Plv+8Rnju8OFBKsIj9M9\n" \
"metL2GiBS/Qg8LOjgbdjmEoMJE7i4Ckcxi8aeGiilWVHf+lhxIwZFm/0QF+VaICv\n" \
"w97Xr5ozvmBbx+ZNj+Jc9CvgYwkhfO0nAV2FomoQp68NDx1OJiDVTRnoKILbWF1A\n" \
"q6CZvzNYM/V8vhZtx9nrXiky85eLAFeA2fEow7gjlQJwuESKVWsXzp5lEsm/G46a\n" \
"+T/4LbqKZgEFpCQHaF3VhhrVjZCL6gpy5Pn2mUAIN+O5/isurcVxCzBhvp2aGTuJ\n" \
"3vOFCbECgYEA9ItY9lCZSUtLGOAMdlOxHOdWGpCAGfKLh8U7JigRGGs5OYPARmiR\n" \
"1zrgXbOsZWn08ujqS9xr2fmvcuLa7JS+4fxEt2jDt/ccw6tylVl/hi+iGzmC2KKx\n" \
"jObDJOLf3JqbqWyRfhEhtDJYCXwQlNNI74/oq5vkNiUdmpIVUiFl34cCgYEAwV0n\n" \
"M+019rNCCTP1AnqjZr+yZA1WJAfCxnYjuCt+ibTAqFdxWUovE7tmcuDjP9AEc8Ke\n" \
"UY6+TLqMDnfiOoU++NJQg3t2TjMRkxDvtZfPvsBNSbM/dQu2ReW1VSaN8ShSeSua\n" \
"Fg+JfugJGYVV81bIizAs4tyXXj8+xRW6OOSsgNkCgYEA2IWFGFqWzoGfFYBTKHXD\n" \
"zQ67ezvUYKUTi/xC+e7h7BgMC/Ouy+iMsOANhomurACcBHPb951L1S0xfmyEYJL3\n" \
"TApcIXTeTkW7LzjfdINQ2VPVmXtbckrjIBm2UTOb+KzgBKv/SnsKt40XzBYQbGWD\n" \
"H7Hw1flqn9JPGuwbeqPdmoMCgYBHQaPLv/Ot+Psx1wLIKheDKst7kWqYFoARkMGz\n" \
"Adw8tO52Wtr7c5oLpHiphvRXtLmfSy36kibQscIhJs6okCfXm/FtspazVKB6WLht\n" \
"bAdznPweV77fzDQcZ/095tpR73ENFCNRU+JwcOPBkuBMr0Nx/4vbAj2Jzra7/DjG\n" \
"9IYZaQKBgDyhdQQnpA+MIzf0fy4bDfKjUyakxnoSITRonQLrpGjiIWL1VOV19SKf\n" \
"D90Wy8QJRWU/8eGN9D8arorMmLKhvi55fe1jr38E3JJzhQzkVOKSTAyEJjU/X0SU\n" \
"Ui29h6AzMZ/M8bG5uPIqLp0gpir1ulbI+IPEY7AnplBb12kG8G7e\n" \
"-----END RSA PRIVATE KEY-----\n";

/* PubSubClient */
// WiFi
const char* ssid = "ST7-14";
const char* password = "MayThazin11";

// MQTT Server
const char* mqtt_server = "172.168.100.150";

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
