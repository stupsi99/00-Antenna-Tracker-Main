#include <Arduino.h>

#include <TinyGPS.h> //Tiny GPS library for extracting GPS information
#include <Wire.h>
#include "SFE_HMC6343.h" //Compass library
#include <AccelStepper.h> //Smooth stepper library

SFE_HMC6343 compass;
TinyGPS locosys; //setup a Tiny GPS object

//######fixed definitions:#####

#define ledPin 44 //LED is on pin D44
#define compassLV 46 //compass 3.3V or LV side from Logic Level Converter is on PWM Pin 46
#define compassHV 52 //compass 5V or HV side from Logic Level Converter is on Pin 52

#define buzzerPin 33 //buzzer is on pin D33

#define userSwitch 27

//###Setup parameters for elevation axis only:###

//Elevation motor pin connections
#define ele_stp 2
#define ele_dir 3
#define ele_MS1 4
#define ele_MS2 5
#define ele_MS3 6
#define ele_EN 7

//Motor properties:
#define ele_stepAngle 1.8 //elevation motor step angle of 1.8 degrees

#define ele_calButton 23 //elevation calibration button pin D23

//Gear ratios:
#define ele_gearRatio 33.075  //type here elevation gear ratio (ratio >1)

//Calibration default values:
#define ele_calRevAngle 10 //this means that the elevation plate moves upwards for 10 degrees before moving back down to hit the calibration switch

//###Setup parameters for azimuth axis only:###

//Azimuth motor pin connections
#define azi_stp 8
#define azi_dir 9
#define azi_MS1 10
#define azi_MS2 11
#define azi_MS3 12
#define azi_EN 13

//Motor properties:
#define azi_stepAngle 0.9

//Gear ratio:
#define azi_gearRatio 24

//Calibration default values:
#define azi_calRevAngle 360 //this means that for compass calibration, the antenna tracker will turn 360 degrees.

int i_stepsAzimutRight = 0; //used for calculating required clockwise steps in motor control section

//global variables:

float f_currentAzimut = 0; //the actual azimuth angle the tracker is pointing at currently
float f_currentElevation = 0;

unsigned long previousMillis = 0; //variable for restricting the angle calculation interval
const long interval = 1000; //interval for angle calculation in ms (to prevent buffer overflow on the serial port)

//test home coordinates (middle Luitpoldpark)
float f_home_lat = 48.168926;
float f_home_lon = 11.568848;
float f_home_altitude = 0; //[m]

/*Test Luitpold Park
Home Positon: 48.168926,11.568848
Quadrant 1: 48.170639,11.570251, Azimut: 28,61°, 217,32m Distance on ground;
Quadrant 2: 48.167487,11.570463, Azimut: 143,19°, 199,97m Distance on ground;
Quadrant 3: 48.167821,11.566685, Azimut: 232,52°, 202,98m Distance on ground
Quadrant 4: 48.170246,11.567995, Azimut: 336,67°, 160,00m Distance on ground
*/

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

//###AccelStepper###

AccelStepper aziStepper(1, 8, 9); //Azimut stepper

//###Battery Voltage Measurement###

int batt_voltage = 0;
#define batt_pin 0 //Battery voltage connected to Analog 0

void setup() {
  Serial.begin(9600); //output to PC
  Serial2.begin(9600); //GPS attached

  //#####pin mode setup:#####
  pinMode(ele_calButton, INPUT); //Elevation calibration button setup
  pinMode(ledPin, OUTPUT); //ledPin setup
  pinMode(userSwitch, INPUT); //User switch
  
  /*
  pinMode(compassLV, OUTPUT);
  pinMode(compassHV, OUTPUT);

  analogWrite(compassLV, 168); //turns on 3.3V on the LV pin of the compass
  digitalWrite(compassHV, HIGH); //turns on 5V on the HV pin of the compass

  delay(500); //required delay for compass to wake up

  if(!compass.init()){
    Serial.println("Compass initialization failed"); //reporting error in compass sensor
  }
  */
  
  //elevation stepper pinMode setup:
  pinMode(ele_stp, OUTPUT);
  pinMode(ele_dir, OUTPUT);
  pinMode(ele_MS1, OUTPUT);
  pinMode(ele_MS2, OUTPUT);
  pinMode(ele_MS3, OUTPUT);
  pinMode(ele_EN, OUTPUT);

  //azimuth stepper pinMode setup:
  pinMode(azi_stp, OUTPUT);
  pinMode(azi_dir, OUTPUT);
  pinMode(azi_MS1, OUTPUT);
  pinMode(azi_MS2, OUTPUT);
  pinMode(azi_MS3, OUTPUT);
  pinMode(azi_EN, OUTPUT);

  resetBEDPins(); //make sure that the stepper motor driver pins are turned off (otherwise might cause overheating)
  
  //short & superflashy intro sequence (LED and tone):
  //tone(buzzerPin,2000,300);
  delay(100);
  tone(buzzerPin,2300,300);
  delay(100);
  tone(buzzerPin,2600,300);
  delay(100);
  tone(buzzerPin,2900,300);
  delay(100);
  
  for(int i_fade=0; i_fade<255; i_fade++){ //LED fading loop
    analogWrite(ledPin, i_fade);
    delay(5);
  }

  

  //###Bluetooth (attached to Serial1) configuration procedure:###
  //has to be done every bootup (to change baudrate to 9600)
  Serial1.begin(115200); //beginning baudrate
  Serial1.print("$"); //configuration commands
  Serial1.print("$");
  Serial1.print("$");
  delay(1000);
  Serial1.println("U,9600,N"); //configuration for 9600 baudrate
  delay(1000);
  Serial1.begin(9600);
  Serial1.println("Bluetooth Module set to 9600bps.");
  Serial1.println("Bluetooth Module Transmitting test.");

  
  //###Axes calibration procedure:###
  Serial1.println("Starting Elevation Calibration.");
  //ele_calibration();
  Serial1.println("Elevation Calibration Complete.");

  
  Serial1.println("Starting Azimuth Calibration.");
  //azi_calibration();
  Serial1.println("Azimuth Calibration Complete.");
  
  Serial1.println("Setup Procedure Complete.");
  tone(buzzerPin,2000,300);


  //###AccelStepper Setup###

  aziStepper.setMaxSpeed(9600);
  aziStepper.setSpeed(9600);

  aziStepper.setAcceleration(1000);
  aziStepper.moveTo(32000);
  
}

void loop() {


  //checking serial port for GPS input
  while (Serial2.available()){
    aa = Serial2.read();
    //Serial.write(aa); //passing the string on to bluetooth module

    if(locosys.encode(aa)){ //checking if GPS input is valid
      locosys.f_get_position(&f_object_lat,&f_object_lon); //command to get latitude and longitude from GPS 
      f_object_alt = locosys.f_altitude(); //command to get altitude from GPS     
      
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
