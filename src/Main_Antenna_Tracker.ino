#include <Arduino.h>
#include <TinyGPS.h>
#include <Wire.h>
#include "SFE_HMC6343.h" //Magnetometer library

//Objects
SFE_HMC6343 Magnetometer;
TinyGPS gpsSensor;

#define LedRing 44
#define LogicConverterLow 46 //Low voltage reference pin for logic level converter (Magnetometer)
#define LogicConverterHigh 52 //High voltage reference
#define buzzerPin 33
#define userSwitch 27
#define batteryPin 0

#define eleDriverStep 2
#define eleDriverDirection 3
#define eleDriverMs1 4
#define eleDriverMs2 5
#define eleDriverMs3 6
#define eleDriverEnable 7
#define eleStepAngle 1.8 //[degrees]
#define eleCalibrationBtn 23 //Button used to find zero elevation
#define eleGearRatio 33.075 //Mechanical gear ratio

#define aziDriverStep 8
#define aziDriverDirection 9
#define aziDriverMs1 10
#define aziDriverMs2 11
#define aziDriverMs3 12
#define aziDriverEnable 13
#define aziStepAngle 0.9
#define aziGearRatio 24 //Mechanical gear ratio

unsigned long previousGpsTimestamp = 0;
unsigned long currentGpsTimestamp;

float UAVLatitude = 48.170639;
float UAVLongitude = 11.570251;
float UAVAltitude = 15;

//helping variables
int positionValid = 0; //only if GPS could be decoded successfully, the angle calculation starts
int i_gps_counter = 0;
unsigned long time_last_decode = 0, time_current_decode = 0;


void setup() {

  Serial.begin(9600); //to PC
  Serial1.begin(115200); //Bluetooth
  Serial2.begin(9600); //GPS via XBEE

  pinMode(eleCalibrationBtn, INPUT);
  pinMode(LedRing, OUTPUT);
  pinMode(userSwitch, INPUT);
  pinMode(LogicConverterHigh, OUTPUT);

  pinMode(eleDriverStep, OUTPUT);
  pinMode(eleDriverDirection, OUTPUT);
  pinMode(eleDriverMs1, OUTPUT);
  pinMode(eleDriverMs2, OUTPUT);
  pinMode(eleDriverMs3, OUTPUT);
  pinMode(eleDriverEnable, OUTPUT);

  pinMode(aziDriverStep, OUTPUT);
  pinMode(aziDriverDirection, OUTPUT);
  pinMode(aziDriverMs1, OUTPUT);
  pinMode(aziDriverMs2, OUTPUT);
  pinMode(aziDriverMs3, OUTPUT);
  pinMode(aziDriverEnable, OUTPUT);

  disableMotors();
  ledIntro();
  setupBluetooth();
  setupMagnetometer();
  //calibrateAzimut();

}

void loop() {

  getNewGps();









    //azimutControl();
    //elevationControl();
    //motorControl();
    //accelControl();
    /*
    batt_voltage = analogRead(batt_pin);
    float pin_voltage = batt_voltage * (5.0 / 1023.0);
    float battery_voltage = (pin_voltage*13.15)/4.7;
    Serial.println(battery_voltage);
    */
    //angleCalculation();
    //runHeadings();
    //getNorthHeading();
    //Serial1.println("Bluetooth Sending");

}
