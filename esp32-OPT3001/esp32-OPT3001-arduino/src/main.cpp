#include <Arduino.h>
#include <Wire.h>
#include <PID_v1.h>

const int ledPin = 2;

/* PWM */
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 12;

/* OPT3001 */
uint16_t iData;
uint8_t iBuff[2];
float flux = 0.0;
uint16_t iExponent;
uint16_t iMantissa;

/* PID */
// Define variables we'll be connecting to
double Setpoint, Input, Output;

// Specify the links and initial tuning parameters
double Kp = 1.182, Ki=0.01, Kd=0;

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

/* Timer */
unsigned int lastTime = 0;
unsigned int currTime = 0;

void setup() {
    ledcSetup(ledChannel, freq, resolution);
    ledcAttachPin(ledPin, ledChannel);

    Serial.begin(115200);

    Wire.begin();
    Wire.beginTransmission(0x44);
    Wire.write(0x01);
    Wire.write(0xC6);
    Wire.write(0x10);
    Wire.endTransmission();

    Wire.beginTransmission(0x44);
    Wire.write(0x00);
    Wire.endTransmission();
    delay(100);

    Setpoint = 700;

    // Turn the PID on
    myPID.SetMode(AUTOMATIC);
    myPID.SetSampleTime(100);
    myPID.SetOutputLimits(0, 4096);
}

// Read the OPT3001 sensor value
float readOPT3001(){
    Wire.requestFrom(0x44, 2);
    Wire.readBytes(iBuff, 2);
    iData = (iBuff[0] << 8 | iBuff[1]);
    iMantissa = iData & 0x0FFF;
    iExponent = (iData & 0xF000) >> 12;
    return iMantissa * (0.01 * pow(2, iExponent));
}
void loop() {
//    bool DEBUG = true;
//    if (DEBUG == true){
//        for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
//            ledcWrite(ledChannel, dutyCycle);
//            flux = readOPT3001();
//            //Serial.print("PWM: ");
//            Serial.print(dutyCycle);
//            Serial.print(",");
//            //Serial.print(" Lux: ");
//            Serial.println(flux);
//            delay(100);
//        }
//        ledcWrite(ledChannel, 0);
//        delay(200);
//    }
//    else {
//        int dutyCycle = 10;
//        ledcWrite(ledChannel, dutyCycle);
//        flux = readOPT3001();
//        //Serial.print("PWM: ");
//        Serial.print(dutyCycle);
//        Serial.print(",");
//        //Serial.print(" Lux: ");
//        Serial.println(flux);
//        delay(100);
//    }
    currTime = millis();
    if(currTime - lastTime < 20000){
        Setpoint = 100;
    } else {
        Setpoint = 700;
    }
    Input = readOPT3001();
    myPID.Compute();

    ledcWrite(ledChannel, Output);
//    Serial.print(Output);
//    Serial.print(",");
    Serial.print(int(Setpoint));
    Serial.print(",");
    Serial.println(int(readOPT3001()));
    delay(100);
}