#include <Arduino.h>

//this sub program calibrates the azimuth and elevation axes

void ele_calibration(){
  //This code calibrates the elevation axis to be at zero degree after switching antenna tracker on
  
  //first, the elevation plate has to move upwards a certain amount:
  digitalWrite(ele_EN, LOW); //Set enable low to allow motor control
  digitalWrite(ele_dir, HIGH); //translates into counterclockwise elevation rotation (or upwards rotation) to make sure that calibration lever is not on calibration switch anymore (in case that the elevation plate was mechanically forced below zero degree during transport (means cal_switch is pressed, but level below zero degrees), this counterclockwise move makes sure that it is definitely above zero degree)
  digitalWrite(ele_MS1, HIGH); //ele_MS1,2,3 HIGH for 1/16th microstepping mode
  digitalWrite(ele_MS2, LOW);
  digitalWrite(ele_MS3, LOW);
  int i_cal=0; //counting variable for the "for loop"  
  i_cal= (ele_calRevAngle*ele_gearRatio*2)/ele_stepAngle; //determines how many steps have to be made to move elevation plate 10 degrees upwards

  //for loop to move upwards the calculated microsteps
  for(int x_cal=0; x_cal<i_cal; x_cal++){
    digitalWrite(ele_stp, HIGH); //trigger one step
    delay(1);
    digitalWrite(ele_stp, LOW); //pull step pin low so it can be triggered again
    delay(1);
  }

  delay(500);
  //next, the elevation plate moves downwards until the calibration switch is touched
  digitalWrite(ele_dir, LOW); //translates into motor rotatio that lowers elevation plate
  //turn on half step mode:
  digitalWrite(ele_MS1, HIGH);
  digitalWrite(ele_MS2, LOW);
  digitalWrite(ele_MS3, LOW);
  
  //while loop until the button is pressed
  while(digitalRead(ele_calButton) == LOW){
    digitalWrite(ele_stp, HIGH); //trigger one step
    delay(1);
    digitalWrite(ele_stp, LOW); //pull step pin low so it can be triggered again
    delay(1);
  }


  resetBEDPins();


}

void azi_calibration(){
  //This code calibrates the azimuth axis. It sets the compass into calibration mode and then turns the antenna tracker 360 degrees

  //compass command to go into calibration mode:
  //###compass calibration command###
  //set up motor driver:
  digitalWrite(azi_EN, LOW); //Set enable low to allow motor control
  digitalWrite(azi_dir, HIGH); //direction does not matter for calibration
  digitalWrite(azi_MS1, LOW); //ele_MS1,2,3 LOW for full step mode
  digitalWrite(azi_MS2, LOW);
  digitalWrite(azi_MS3, LOW);

  int i_calAzi=0; //counting variable for the "for loop"
  i_calAzi= (azi_calRevAngle*azi_gearRatio*1)/azi_stepAngle; //determines how many steps at full step (*1) are required to rotate the given calibration rotation angle

  //for loop to rotate tracker 360 degrees
  for(int y_cal=0; y_cal<i_calAzi; y_cal++){
    digitalWrite(azi_stp, HIGH); //trigger one step
    delay(1);
    digitalWrite(azi_stp, LOW); //pull step pin low so it can be triggered again
    delay(1);     
  }

  resetBEDPins();
}

void resetBEDPins(){

  //resetting all easy driver pins to default states
  digitalWrite(ele_stp, LOW);
  digitalWrite(ele_dir, LOW);
  digitalWrite(ele_MS1, LOW);
  digitalWrite(ele_MS2, LOW);
  digitalWrite(ele_MS3, LOW);
  digitalWrite(ele_EN, HIGH);

  digitalWrite(azi_stp, LOW);
  digitalWrite(azi_dir, LOW);
  digitalWrite(azi_MS1, LOW);
  digitalWrite(azi_MS2, LOW);
  digitalWrite(azi_MS3, LOW);
  digitalWrite(azi_EN, HIGH);
  
}

