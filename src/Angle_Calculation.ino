#include <Arduino.h>

//This sub program calculates the 

int i_quadrant = 0; //to determine quadrant number




void angleCalculation() {
  //Serial.println("Calculating");
  //Serial.println(f_object_lat,6);  
  //calculation of delta_lat & delta_lon, sign (- or +) only important for determining the quadrant
  float f_delta_lat = f_object_lat - f_home_lat;
  float f_delta_lon = f_object_lon - f_home_lon;
  //Serial.print("Delta Latitude: ");
  //Serial.println(f_delta_lat,10);
  //Serial.print("Delta Longitude: ");
  //Serial.println(f_delta_lon,10);

  //first need to determine quadrant with comparing variable (multiplication with 1000 to include 4 comma values of the floating point variable, before rounding into an integer (which can be compared)
  //make two int compare values (one for f_delta_lat and one for f_delta_lon), because floating points can't be compared. The compare value is necessary to determine the quadrant, in which the object is in
  long l_comparevalue_lat = round(f_delta_lat*1000000);
  long l_comparevalue_lon = round(f_delta_lon*1000000);
  //Serial.print("Comparevaluelon: ");
  //Serial.println(l_comparevalue_lon);
  //Serial.print("Comparevaluelat: ");
  //Serial.println(l_comparevalue_lat);

  //now the compare values help to determine which quadrant the object is in
  
  //argument for quadrant 1
  if(l_comparevalue_lat >= 0 && l_comparevalue_lon >=0){
    i_quadrant = 1; //will be used later for determining the 4 variations of azimut calculation
    //Serial.println("Quadrant 1");    
  }

  //argument for quadrant 2
  if(l_comparevalue_lat <= 0 && l_comparevalue_lon >= 0){
    i_quadrant = 2;
    //Serial.println("Quadrant 2");    
  }

  //argument for quadrant 3
  if(l_comparevalue_lat <= 0 && l_comparevalue_lon <= 0){
    i_quadrant = 3;
    //Serial.println("Quadrant 3");
  }

  //argrument for quadrant 4
  if(l_comparevalue_lat >= 0 && l_comparevalue_lon <= 0){
    i_quadrant = 4;
    //Serial.println("Quadrant 4");
  }

  
  //Serial.print("delta_lat is: ");
  //Serial.println(f_delta_lat);

  //Serial.print("delta_lon is: ");
  //Serial.println(f_delta_lon);

  //###azimut, distance on ground and elevation calculations based on the different quadrants:###
  
  if(i_quadrant == 1){
    
    //distance difference in km between home position and object position, separately for longitude and latitude
    float f_distance_lat = f_delta_lat*111.3; //111.3km distance between two circles of latitudes
    float f_distance_lon = f_delta_lon*111.3*cos(((f_home_lat+f_object_lat)/2)*DEG_TO_RAD); //middle value of latitude determined for estimation
    
    //total distance on ground to home position calculation
    f_distance_ground = sqrt(sq(f_distance_lat)+sq(f_distance_lon)); //pythagoras
    f_distance_ground = f_distance_ground*1000; //converting total distance from km into m (for elevation calculation)
    //Serial.print("Total distance: ");
    //Serial.println(f_distance_ground);
    
    //azimut calculation
    f_azimut = atan(f_distance_lon/f_distance_lat); //azimut in RAD
    f_azimut = f_azimut*RAD_TO_DEG;
    //Serial.print("Azimut: ");
    //Serial.println(f_azimut);

    //elevation calculation
    f_altitude_to_ground = f_object_alt - f_home_altitude; //calculating the object altitude in respect to home position
    //Serial.print("Altitude: ");
    //Serial.println(f_altitude_to_ground);
    f_elevation = atan(f_altitude_to_ground / f_distance_ground);  //elevation in RAD
    f_elevation = f_elevation*RAD_TO_DEG;
    //Serial.print("Elevation: ");
    //Serial.println(f_elevation);
  }

  if(i_quadrant == 2){
    
    //distance difference in km between home position and object position, separately for longitude and latitude
    float f_distance_lat = f_delta_lat*111.3*(-1); //111.3km distance between two circles of latitudes, multiplied with -1 to correct the negative f_delta_lat value in quadrant 2
    float f_distance_lon = f_delta_lon*111.3*cos(((f_home_lat+f_object_lat)/2)*DEG_TO_RAD); //middle value of latitude determined for estimation
    
    //total distance on ground to home position calculation
    f_distance_ground = sqrt(sq(f_distance_lat)+sq(f_distance_lon)); //pythagoras
    f_distance_ground = f_distance_ground*1000; //converting total distance from km into m (for elevation calculation)
    //Serial.print("Distance on ground: ");
    //Serial.println(f_distance_ground);
    
    //azimut calculation
    f_azimut = atan(f_distance_lat/f_distance_lon); //azimut in RAD
    f_azimut = f_azimut*RAD_TO_DEG + 90;  //azimut in degree and including the 90° correction value in quadrant 2
    //Serial.print("Azimut: ");
    //Serial.println(f_azimut);

    //elevation calculation
    f_altitude_to_ground = f_object_alt - f_home_altitude; //calculating the object altitude in respect to home position
    //Serial.print("Altitude: ");
    //Serial.println(f_altitude_to_ground);
    f_elevation = atan(f_altitude_to_ground / f_distance_ground);  //elevation in RAD
    f_elevation = f_elevation*RAD_TO_DEG;
    //Serial.print("Elevation: ");
    //Serial.println(f_elevation);
  }

    if(i_quadrant == 3){
    
    //distance difference in km between home position and object position, separately for longitude and latitude
    float f_distance_lat = f_delta_lat*111.3*(-1); //111.3km distance between two circles of latitudes, multiplied with -1 to correct the negative f_delta_lat value in quadrant 3
    float f_distance_lon = f_delta_lon*111.3*(-1)*cos(((f_home_lat+f_object_lat)/2)*DEG_TO_RAD); //middle value of latitude determined for estimation, all multiplied with - 1 to correct the negative f_delta_lon value in quadrant 3
    
    //total distance on ground to home position calculation
    f_distance_ground = sqrt(sq(f_distance_lat)+sq(f_distance_lon)); //pythagoras
    f_distance_ground = f_distance_ground*1000; //converting total distance from km into m (for elevation calculation)
    //Serial.print("Total distance: ");
    //Serial.println(f_distance_ground);
    
    //azimut calculation
    f_azimut = atan(f_distance_lon/f_distance_lat); //azimut in RAD
    f_azimut = f_azimut*RAD_TO_DEG + 180;  //azimut in degree and including the 180° correction value in quadrant 3
    //Serial.print("Azimut: ");
    //Serial.println(f_azimut);

    //elevation calculation
    f_altitude_to_ground = f_object_alt - f_home_altitude; //calculating the object altitude in respect to home position
    //Serial.print("Altitude: ");
    //Serial.println(f_altitude_to_ground);
    f_elevation = atan(f_altitude_to_ground / f_distance_ground);  //elevation in RAD
    f_elevation = f_elevation*RAD_TO_DEG;
    //Serial.print("Elevation: ");
    //Serial.println(f_elevation);
  }

      if(i_quadrant == 4){
    
    //distance difference in km between home position and object position, separately for longitude and latitude
    float f_distance_lat = f_delta_lat*111.3; //111.3km distance between two circles of latitudes
    float f_distance_lon = f_delta_lon*111.3*(-1)*cos(((f_home_lat+f_object_lat)/2)*DEG_TO_RAD); //middle value of latitude determined for estimation, all multiplied with - 1 to correct the negative f_delta_lon value in quadrant 4

    //total distance on ground to home position calculation
    f_distance_ground = sqrt(sq(f_distance_lat)+sq(f_distance_lon)); //pythagoras
    f_distance_ground = f_distance_ground*1000; //converting total distance from km into m (for elevation calculation)
    //Serial.print("Total distance: ");
    //Serial.println(f_distance_ground);
    
    //azimut calculation
    f_azimut = atan(f_distance_lat/f_distance_lon); //azimut in RAD
    f_azimut = f_azimut*RAD_TO_DEG + 270;  //azimut in degree and including the 270° correction value in quadrant 4
    //Serial.print("Azimut: ");
    //Serial.println(f_azimut);

    //elevation calculation
    f_altitude_to_ground = f_object_alt - f_home_altitude; //calculating the object altitude in respect to home position
    //Serial.print("Altitude: ");
    //Serial.println(f_altitude_to_ground);
    f_elevation = atan(f_altitude_to_ground / f_distance_ground);  //elevation in RAD
    f_elevation = f_elevation*RAD_TO_DEG;
    //Serial.print("Elevation: ");
    //Serial.println(f_elevation);
  }



    
    //Serial1.print("Total distance: ");
    //Serial1.println(f_distance_ground);
    //Serial1.print("Altitude to ground: ");
    //Serial1.println(f_altitude_to_ground);
    //Serial1.print("Elevation: ");
    //Serial1.println(f_elevation);

}
