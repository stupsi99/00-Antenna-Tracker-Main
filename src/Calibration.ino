#include <Arduino.h>

void CalibrateElevation(){
  //This code calibrates the elevation axis to be at zero degree after switching antenna tracker on

  //first, the elevation plate has to move upwards a certain amount:
  digitalWrite(eleDriverEnable, LOW); //Set enable low to allow motor control
  digitalWrite(eleDriverDirection, HIGH); //translates into counterclockwise elevation rotation (or upwards rotation) to make sure that calibration lever is not on calibration switch anymore (in case that the elevation plate was mechanically forced below zero degree during transport (means cal_switch is pressed, but level below zero degrees), this counterclockwise move makes sure that it is definitely above zero degree)
  digitalWrite(eleDriverMs1, HIGH); //eleDriverMs1,2,3 HIGH for 1/16th microstepping mode
  digitalWrite(eleDriverMs2, LOW);
  digitalWrite(eleDriverMs3, LOW);
  int i_cal=0; //counting variable for the "for loop"
  i_cal= (10*eleGearRatio*2)/eleStepAngle; //determines how many steps have to be made to move elevation plate 10 degrees upwards

  //for loop to move upwards the calculated microsteps
  for(int x_cal=0; x_cal<i_cal; x_cal++){
    digitalWrite(eleDriverStep, HIGH); //trigger one step
    delay(1);
    digitalWrite(eleDriverStep, LOW); //pull step pin low so it can be triggered again
    delay(1);
  }

  delay(500);
  //next, the elevation plate moves downwards until the calibration switch is touched
  digitalWrite(eleDriverDirection, LOW); //translates into motor rotatio that lowers elevation plate
  //turn on half step mode:
  digitalWrite(eleDriverMs1, HIGH);
  digitalWrite(eleDriverMs2, LOW);
  digitalWrite(eleDriverMs3, LOW);

  //while loop until the button is pressed
  while(digitalRead(eleCalibrationBtn) == LOW){
    digitalWrite(eleDriverStep, HIGH); //trigger one step
    delay(1);
    digitalWrite(eleDriverStep, LOW); //pull step pin low so it can be triggered again
    delay(1);
  }


  disableMotors();


}

void CalibrateAzimut(){
  //This code calibrates the azimuth axis. It sets the Magnetometer into calibration mode and then turns the antenna tracker 360 degrees

  //Magnetometer command to go into calibration mode:
  //###Magnetometer calibration command###
  //set up motor driver:
  digitalWrite(aziDriverEnable, LOW); //Set enable low to allow motor control
  digitalWrite(aziDriverDirection, HIGH); //direction does not matter for calibration
  digitalWrite(aziDriverMs1, LOW); //eleDriverMs1,2,3 LOW for full step mode
  digitalWrite(aziDriverMs2, LOW);
  digitalWrite(aziDriverMs3, LOW);

  int i_calAzi=0; //counting variable for the "for loop"
  i_calAzi= (360*aziGearRatio*1)/aziStepAngle; //determines how many steps at full step (*1) are required to rotate the given calibration rotation angle

  //for loop to rotate tracker 360 degrees
  for(int y_cal=0; y_cal<i_calAzi; y_cal++){
    digitalWrite(aziDriverStep, HIGH); //trigger one step
    delay(1);
    digitalWrite(aziDriverStep, LOW); //pull step pin low so it can be triggered again
    delay(1);
  }

  disableMotors();
}
