#include <Arduino.h>

int i_quadrant = 0; //to determine quadrant number

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




    //Serial1.print("Total distance: ");
    //Serial1.println(f_distance_ground);
    //Serial1.print("Altitude to ground: ");
    //Serial1.println(f_altitude_to_ground);
    //Serial1.print("Elevation: ");
    //Serial1.println(f_elevation);

}
