#include <Arduino.h>

int MapQuadrant = 0; //to determine quadrant number
float CurrentAzimut; //the actual azimuth angle the tracker is pointing at currently
float CurrentElevation;

float UAVLatitude = 48.170639;
float UAVLongitude = 11.570251;
float UAVAltitude = 15;

//pointing angles and flying object information
float NewAzimut = 0; //[°]
float NewElevation = 0; //[°]
float AltitudeToGround = 0; //[m]
float DistanceOnGround = 0; //[m]

//helping variables
int positionValid = 0; //only if GPS could be decoded successfully, the angle calculation starts
byte gpsString;
int i_gps_counter = 0;
unsigned long time_last_decode = 0, time_current_decode = 0;

//test home coordinates (middle Luitpoldpark)
float HomeLatitude = 48.168926;
float HomeLongitude = 11.568848;
float HomeAltitude = 0; //[m]

/*Test Luitpold Park
Home Positon: 48.168926,11.568848
Quadrant 1: 48.170639,11.570251, Azimut: 28,61°, 217,32m Distance on ground;
Quadrant 2: 48.167487,11.570463, Azimut: 143,19°, 199,97m Distance on ground;
Quadrant 3: 48.167821,11.566685, Azimut: 232,52°, 202,98m Distance on ground
Quadrant 4: 48.170246,11.567995, Azimut: 336,67°, 160,00m Distance on ground
*/

void angleCalculation() {
  float DeltaLatitude = UAVLatitude - HomeLatitude;
  float DeltaLongitude = UAVLongitude - HomeLongitude;

  //first need to determine quadrant with comparing variable (multiplication with 1000 to include 4 comma values of the floating point variable, before rounding into an integer (which can be compared)
  long CompareLatitude = round(DeltaLatitude*1000000);
  long CompareLongitude = round(DeltaLongitude*1000000);

  //argument for quadrant 1
  if(CompareLatitude >= 0 && CompareLongitude >=0){
    MapQuadrant = 1;
  }

  //argument for quadrant 2
  if(CompareLatitude <= 0 && CompareLongitude >= 0){
    MapQuadrant = 2;
  }

  //argument for quadrant 3
  if(CompareLatitude <= 0 && CompareLongitude <= 0){
    MapQuadrant = 3;
  }

  //argrument for quadrant 4
  if(CompareLatitude >= 0 && CompareLongitude <= 0){
    MapQuadrant = 4;
  }

  //Azimut and elevation calculations

  if(MapQuadrant == 1){

    //distance difference in km between home position and object position, separately for longitude and latitude
    float DistanceLatitudeKm = DeltaLatitude*111.3; //111.3km distance between two circles of latitudes
    float DistanceLongitudeKm = DeltaLongitude*111.3*cos(((HomeLatitude+UAVLatitude)/2)*DEG_TO_RAD); //middle value of latitude determined for estimation

    //total distance on ground to home position calculation
    DistanceOnGround = sqrt(sq(DistanceLatitudeKm)+sq(DistanceLongitudeKm)); //pythagoras
    DistanceOnGround = DistanceOnGround*1000; //converting total distance from km into m (for elevation calculation)

    //azimut calculation
    NewAzimut = atan(DistanceLongitudeKm/DistanceLatitudeKm); //azimut in RAD
    NewAzimut = NewAzimut*RAD_TO_DEG;

    //elevation calculation
    AltitudeToGround = UAVAltitude - HomeAltitude; //calculating the object altitude in respect to home position
    NewElevation = atan(AltitudeToGround / DistanceOnGround);  //elevation in RAD
    NewElevation = NewElevation*RAD_TO_DEG;
  }

  if(MapQuadrant == 2){

    float DistanceLatitudeKm = DeltaLatitude*111.3*(-1);
    float DistanceLongitudeKm = DeltaLongitude*111.3*cos(((HomeLatitude+UAVLatitude)/2)*DEG_TO_RAD);

    DistanceOnGround = sqrt(sq(DistanceLatitudeKm)+sq(DistanceLongitudeKm));
    DistanceOnGround = DistanceOnGround*1000;

    NewAzimut = atan(DistanceLatitudeKm/DistanceLongitudeKm);
    NewAzimut = NewAzimut*RAD_TO_DEG + 90;

    AltitudeToGround = UAVAltitude - HomeAltitude;
    NewElevation = atan(AltitudeToGround / DistanceOnGround);
    NewElevation = NewElevation*RAD_TO_DEG;

  }

  if(MapQuadrant == 3){

    float DistanceLatitudeKm = DeltaLatitude*111.3*(-1);
    float DistanceLongitudeKm = DeltaLongitude*111.3*(-1)*cos(((HomeLatitude+UAVLatitude)/2)*DEG_TO_RAD);

    DistanceOnGround = sqrt(sq(DistanceLatitudeKm)+sq(DistanceLongitudeKm));
    DistanceOnGround = DistanceOnGround*1000;

    NewAzimut = atan(DistanceLongitudeKm/DistanceLatitudeKm);
    NewAzimut = NewAzimut*RAD_TO_DEG + 180;

    AltitudeToGround = UAVAltitude - HomeAltitude;
    NewElevation = atan(AltitudeToGround / DistanceOnGround);
    NewElevation = NewElevation*RAD_TO_DEG;

  }

  if(MapQuadrant == 4){

    float DistanceLatitudeKm = DeltaLatitude*111.3;
    float DistanceLongitudeKm = DeltaLongitude*111.3*(-1)*cos(((HomeLatitude+UAVLatitude)/2)*DEG_TO_RAD);

    DistanceOnGround = sqrt(sq(DistanceLatitudeKm)+sq(DistanceLongitudeKm));
    DistanceOnGround = DistanceOnGround*1000;

    NewAzimut = atan(DistanceLatitudeKm/DistanceLongitudeKm);
    NewAzimut = NewAzimut*RAD_TO_DEG + 270;

    AltitudeToGround = UAVAltitude - HomeAltitude;
    NewElevation = atan(AltitudeToGround / DistanceOnGround);
    NewElevation = NewElevation*RAD_TO_DEG;

  }

    Serial.print("Total distance: ");
    Serial.println(DistanceOnGround);
    Serial.print("Altitude to ground: ");
    Serial.println(AltitudeToGround);
    Serial.print("Azimut: ");
    Serial.println(NewAzimut);

}
