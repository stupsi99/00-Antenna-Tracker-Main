#include <Arduino.h>

//this subprogram moves the antenna tracker towards the calculated angles

void azimutControl(){

  int i_currentAzimut = round(currentAzimut); //rounding the current azimuth for comparison
  int i_newAzimut = round(newAzimut); //rounding for comparison with each other

  //Serial1.print("Current Azimut: ");
  //Serial1.println(currentAzimut);

  int i_deltaAzimut = sqrt(sq(i_newAzimut-i_currentAzimut)); //rounded difference of desired and actual azimut

  if(i_deltaAzimut >= 4){

    digitalWrite(aziDriverMs1, LOW); //aziDriverMs1,2,3 LOW for full step mode
    digitalWrite(aziDriverMs2, LOW);
    digitalWrite(aziDriverMs3, LOW);
    int i_stepModeAzi = 1;

    //### ROTATION CLOCKWISE ###
    if(i_newAzimut >= i_currentAzimut){

      digitalWrite(aziDriverEnable, LOW);
      digitalWrite(aziDriverDirection, HIGH); //clockwise
      int increasedSteps = round((newAzimut-currentAzimut)*aziGearRatio/(aziStepAngle*i_stepModeAzi));

      for(int i_aziLoop = 0; i_aziLoop <increasedSteps; i_aziLoop++){

        digitalWrite(aziDriverStep, HIGH);
        delay(1);
        digitalWrite(aziDriverStep, LOW);
        delay(1);

      }

      currentAzimut = currentAzimut + ((increasedSteps*aziStepAngle)/(aziGearRatio*i_stepModeAzi));

      disableMotors();

    }

  //### ROTATION COUNTERCLOCKWISE ###
    if(i_newAzimut < i_currentAzimut){

    digitalWrite(aziDriverEnable, LOW);
    digitalWrite(aziDriverDirection, LOW); //counterclockwise

    int increasedSteps = round((currentAzimut-newAzimut)*aziGearRatio/(aziStepAngle*i_stepModeAzi));

      for(int i_aziLoop = 0; i_aziLoop <increasedSteps; i_aziLoop++){ //move steps counterclockwise

        digitalWrite(aziDriverStep, HIGH); //trigger one step
        delay(1);
        digitalWrite(aziDriverStep, LOW); //pull step pin low so it can be triggered again
        delay(1);

      }

    currentAzimut = currentAzimut - ((increasedSteps*aziStepAngle)/(aziGearRatio*i_stepModeAzi)); //subtract the azimuth change from the current azimuth

    disableMotors();

  }



  }
}


void elevationControl(){
  //Serial1.print("Current Elevation: ");
  //Serial1.println(CurrentElevation);
  //Serial1.print("Calculated Elevation: ");
  //Serial1.println(NewElevation);

  //this code adapts the current elevation level to be as close as possible to the desired one

  int i_currentElevation = round(CurrentElevation); //comparevalue for elevation
  int i_elevation = round(NewElevation); //comparevalue for elevation

  //first make sure the maximum and minimum elevation angle are not exceeded:
  if(i_currentElevation > 90){ //failsafe in case calculations go over 90 degrees elevation
    CurrentElevation = 90;
  }

  if(i_currentElevation < 0){ //failsafe in case calculations go below 0 degrees elevation
    CurrentElevation = 0;
  }

  int i_deltaElevation = sqrt(sq(i_currentElevation-i_elevation)); //for comparisons


  //### only start movement if gap larger than 1 degree: ###

  if(i_deltaElevation >= 1){ //if gap larger than 1 degree

    digitalWrite(eleDriverMs1, LOW); //default quarter step speed
    digitalWrite(eleDriverMs2, LOW);
    digitalWrite(eleDriverMs3, LOW);
    int i_stepModeEle = 1; //parameter used for calculations later on



  //### check which direction to move: ###

  //### move elevation platform upwards: ###

    if(i_currentElevation < i_elevation){

    //set up motor parameters
    digitalWrite(eleDriverEnable, LOW); //Set enable low to allow motor control
    digitalWrite(eleDriverDirection, HIGH); //upwards elevation rotation

    int i_stepsIncreaseEle = round(((NewElevation-CurrentElevation)*eleGearRatio*i_stepModeEle)/eleStepAngle);

    for(int i_eleLoop = 0; i_eleLoop < i_stepsIncreaseEle; i_eleLoop++){

      digitalWrite(eleDriverStep, HIGH);
      delay(1);
      digitalWrite(eleDriverStep,LOW);
      delay(1);
    }

    CurrentElevation = CurrentElevation + ((i_stepsIncreaseEle*eleStepAngle)/eleGearRatio);
    digitalWrite(eleDriverEnable, HIGH); //disable current flow
  }

  //### move elevation platform downwards: ###

  if(i_currentElevation > i_elevation){

    //set up motor parameters
    digitalWrite(eleDriverEnable, LOW); //Set enable low to allow motor control
    digitalWrite(eleDriverDirection, LOW); //downwards elevation rotation

    int i_stepsIncreaseEle = round(((CurrentElevation-NewElevation)*eleGearRatio*i_stepModeEle)/eleStepAngle);

    for(int i_eleLoop = 0; i_eleLoop < i_stepsIncreaseEle; i_eleLoop++){

      digitalWrite(eleDriverStep, HIGH);
      delay(1);
      digitalWrite(eleDriverStep, LOW);
      delay(1);
    }

    CurrentElevation = CurrentElevation - ((i_stepsIncreaseEle*eleStepAngle)/eleGearRatio);
    digitalWrite(eleDriverEnable, HIGH); //disable current flow
  }

}

}
