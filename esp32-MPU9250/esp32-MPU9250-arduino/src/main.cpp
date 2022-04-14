#include <Arduino.h>
#include "MPU9250.h"

MPU9250 IMU(Wire, 0x68);
int status;

float vo = 0;
float ao = 0;
unsigned int currtime = 0;
unsigned int lasttime = 0;
const int interval = 1000;

void setup() {
    Serial.begin(115200);

    status = IMU.begin();
    if(status < 0){
        Serial.println("IMU initialization unsuccessful");
        Serial.println("Check IMU wiring or try cycling power");
        Serial.print("Status: ");
        Serial.println(status);
        while(1) {}
    }

    IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G);
    IMU.setGyroRange(MPU9250::GYRO_RANGE_500DPS);
    IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
    IMU.setSrd(19);

    IMU.readSensor();
    ao = IMU.getAccelX_mss();
}

void loop() {

    currtime = millis();
    if (currtime - lasttime > interval){
        IMU.readSensor();

        Serial.print(IMU.getAccelX_mss(),6);
        Serial.print("\t");
        Serial.print(IMU.getAccelY_mss(),6);
        Serial.print("\t");
        Serial.print(IMU.getAccelZ_mss(),6);
        Serial.println("\t");
//        float v = vo + (a - ao) * 1;
//
//        Serial.print("X-axis velocity: ");
//        Serial.print(v);
//        Serial.println("m/s");
//
//        vo = v;
//        ao = a;

        lasttime = currtime;
    }
}