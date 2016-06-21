#include <Arduino.h>

int i_quadrant = 0; //to determine quadrant number
float f_currentAzimut = 0; //the actual azimuth angle the tracker is pointing at currently
float f_currentElevation = 0;

//flying object coordinates
float f_object_lat = 48.170639;
float f_object_lon = 11.570251;
float f_object_alt = 15;

//pointing angles and flying object information
float f_azimut = 0; //[°]
float f_elevation = 0; //[°]
float f_altitude_to_ground = 0; //[m]
float f_distance_ground = 0; //[m]
float f_total_distance = 0; //calculation not implemented (line of sight distance to object)

//helping variables
int positionValid = 0; //only if GPS could be decoded successfully, the angle calculation starts
byte gpsString;
int i_gps_counter = 0;
unsigned long time_last_decode = 0, time_current_decode = 0;

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

void angleCalculation() {
  float f_delta_lat = f_object_lat - f_home_lat;
  float f_delta_lon = f_object_lon - f_home_lon;

  //first need to determine quadrant with comparing variable (multiplication with 1000 to include 4 comma values of the floating point variable, before rounding into an integer (which can be compared)
  //make two int compare values (one for f_delta_lat and one for f_delta_lon), because floating points can't be compared. The compare value is necessary to determine the quadrant, in which the object is in
  long l_comparevalue_lat = round(f_delta_lat*1000000);
  long l_comparevalue_lon = round(f_delta_lon*1000000);

  //now the compare values help to determine which quadrant the object is in

  //argument for quadrant 1
  if(l_comparevalue_lat >= 0 && l_comparevalue_lon >=0){
    i_quadrant = 1;
  }

  //argument for quadrant 2
  if(l_comparevalue_lat <= 0 && l_comparevalue_lon >= 0){
    i_quadrant = 2;
  }

  //argument for quadrant 3
  if(l_comparevalue_lat <= 0 && l_comparevalue_lon <= 0){
    i_quadrant = 3;
  }

  //argrument for quadrant 4
  if(l_comparevalue_lat >= 0 && l_comparevalue_lon <= 0){
    i_quadrant = 4;
  }


  //Azimut and elevation calculations

  if(i_quadrant == 1){

    //distance difference in km between home position and object position, separately for longitude and latitude
    float f_distance_lat = f_delta_lat*111.3; //111.3km distance between two circles of latitudes
    float f_distance_lon = f_delta_lon*111.3*cos(((f_home_lat+f_object_lat)/2)*DEG_TO_RAD); //middle value of latitude determined for estimation

    //total distance on ground to home position calculation
    f_distance_ground = sqrt(sq(f_distance_lat)+sq(f_distance_lon)); //pythagoras
    f_distance_ground = f_distance_ground*1000; //converting total distance from km into m (for elevation calculation)

    //azimut calculation
    f_azimut = atan(f_distance_lon/f_distance_lat); //azimut in RAD
    f_azimut = f_azimut*RAD_TO_DEG;

    //elevation calculation
    f_altitude_to_ground = f_object_alt - f_home_altitude; //calculating the object altitude in respect to home position
    f_elevation = atan(f_altitude_to_ground / f_distance_ground);  //elevation in RAD
    f_elevation = f_elevation*RAD_TO_DEG;
  }

  if(i_quadrant == 2){

    float f_distance_lat = f_delta_lat*111.3*(-1);
    float f_distance_lon = f_delta_lon*111.3*cos(((f_home_lat+f_object_lat)/2)*DEG_TO_RAD);

    f_distance_ground = sqrt(sq(f_distance_lat)+sq(f_distance_lon));
    f_distance_ground = f_distance_ground*1000;

    f_azimut = atan(f_distance_lat/f_distance_lon);
    f_azimut = f_azimut*RAD_TO_DEG + 90;

    f_altitude_to_ground = f_object_alt - f_home_altitude;
    f_elevation = atan(f_altitude_to_ground / f_distance_ground);
    f_elevation = f_elevation*RAD_TO_DEG;

  }

    if(i_quadrant == 3){

    float f_distance_lat = f_delta_lat*111.3*(-1);
    float f_distance_lon = f_delta_lon*111.3*(-1)*cos(((f_home_lat+f_object_lat)/2)*DEG_TO_RAD);

    f_distance_ground = sqrt(sq(f_distance_lat)+sq(f_distance_lon));
    f_distance_ground = f_distance_ground*1000;

    f_azimut = atan(f_distance_lon/f_distance_lat);
    f_azimut = f_azimut*RAD_TO_DEG + 180;

    f_altitude_to_ground = f_object_alt - f_home_altitude;
    f_elevation = atan(f_altitude_to_ground / f_distance_ground);
    f_elevation = f_elevation*RAD_TO_DEG;

  }

      if(i_quadrant == 4){

    float f_distance_lat = f_delta_lat*111.3;
    float f_distance_lon = f_delta_lon*111.3*(-1)*cos(((f_home_lat+f_object_lat)/2)*DEG_TO_RAD);

    f_distance_ground = sqrt(sq(f_distance_lat)+sq(f_distance_lon));
    f_distance_ground = f_distance_ground*1000;

    f_azimut = atan(f_distance_lat/f_distance_lon);
    f_azimut = f_azimut*RAD_TO_DEG + 270;

    f_altitude_to_ground = f_object_alt - f_home_altitude;
    f_elevation = atan(f_altitude_to_ground / f_distance_ground);
    f_elevation = f_elevation*RAD_TO_DEG;

  }



    /*
    Serial.print("Total distance: ");
    Serial.println(f_distance_ground);
    Serial.print("Altitude to ground: ");
    Serial.println(f_altitude_to_ground);
    Serial.print("Azimut: ");
    Serial.println(f_azimut);
    */

}
