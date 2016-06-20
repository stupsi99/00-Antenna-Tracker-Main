#include <Arduino.h>
#include <TinyGPS.h>
#include <Wire.h>
#include "SFE_HMC6343.h" //Magnetometer library


//Objects
SFE_HMC6343 Magnetometer;
TinyGPS gpsSensor;

//Pin mapping and definitions
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
#define aziGearRatio 24

float f_currentAzimut = 0; //the actual azimuth angle the tracker is pointing at currently
float f_currentElevation = 0;

unsigned long previousMillis = 0; //variable for restricting the angle calculation interval
const long interval = 1000; //interval for angle calculation in ms (to prevent buffer overflow on the serial port)

//test home coordinates (middle Luitpoldpark)
float f_home_lat = 48.168926;
float f_home_lon = 11.568848;
float f_home_altitude = 0; //[m]

//flying object coordinates
float f_object_lat = 0;
float f_object_lon = 0;
float f_object_alt = 0;

//pointing angles and flying object information
float f_azimut = 0; //[°]
float f_elevation = 0; //[°]
float f_altitude_to_ground = 0; //[m]
float f_distance_ground = 0; //[m]
float f_total_distance = 0; //calculation not implemented (line of sight distance to object)

//helping variables
int positionValid = 0; //only if GPS could be decoded successfully, the angle calculation starts
byte aa; //used as GPS string buffer for bluetooth
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
  //ele_calibration();
  //azi_calibration();

  Serial1.println("Setup Procedure Complete.");
  tone(buzzerPin,2000,300);

}

void loop() {


  //checking serial port for GPS input
  while (Serial2.available()){
    aa = Serial2.read();
    //Serial.write(aa); //passing the string on to bluetooth module

    if(gpsSensor.encode(aa)){ //checking if GPS input is valid
      gpsSensor.f_get_position(&f_object_lat,&f_object_lon); //command to get latitude and longitude from GPS
      f_object_alt = gpsSensor.f_altitude(); //command to get altitude from GPS

      /*
      i_gps_counter ++; //counts the amount of decoded gps strings
      Serial1.print("Decoded GPS strings: ");
      Serial1.print(i_gps_counter);
      time_current_decode = millis();
      time_current_decode = time_current_decode/1000;
      Serial1.print(" at ");
      Serial1.print(time_current_decode);
      Serial1.println(" seconds.");
      */
      tone(buzzerPin,1000,50);

      positionValid = 1; //allow next loops

    }

    }

  unsigned long currentMillis = millis(); //variable necessary to compare the milliseconds

  if(currentMillis - previousMillis >= interval){ //checks if its time to update calculations
    previousMillis = currentMillis; //updates the millisecond count

  if(positionValid == 1){ //executes only when GPS position is valid
    angleCalculation(); //executes angle calculation sub program

    //Serial1.println(f_object_lat, 6); //output Latitude, 6 decimal points
    //Serial1.print("Azimut angle is: ");
    //Serial1.println(f_azimut);
    //Serial.print("Distance on ground in meter is: ");
    //Serial.println(f_distance_ground);


    positionValid = 0; //blocks the angle calculation until it has a new gps string
    //tone(buzzerPin,2000,50);
    }

  }

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



}
