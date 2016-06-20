#include <Arduino.h>

//this subprogram moves the antenna tracker towards the calculated angles

void azimutControl(){
  //this code adapts only the current azimuth angle to be as close as possible to the desired one

  //following code section decides whether or not it is better to move clockwise or counterclockwise:
  
  //Following situations can occur which determine the rotational direction (simplified version, actually there are more situations, but not very likely, if the flying object does not fly overhead the tracker):
  //Situation 1: desired azimuth is bigger than current azimuth -> clockwise rotation of tracker
  //Situation 2: desired azimuth is smaller than current azimuth -> counterclockwise rotation of tracker
  //The crossing of the zero degree azimut line is not taken into account yet, so the path can not be too close to zero degree!
  
  int i_currentAzimut = round(f_currentAzimut); //rounding the current azimuth for comparison
  int i_azimut = round(f_azimut); //rounding for comparison with each other
  
  //Serial1.print("Current Azimut: ");
  //Serial1.println(f_currentAzimut);

  int i_deltaAzimut = sqrt(sq(i_azimut-i_currentAzimut)); //rounded difference of desired and actual azimut 
  
  if(i_deltaAzimut >= 2){ //only adjust angle, if azimut difference is greater than 2 degrees

    //default speed for small gaps:
    digitalWrite(aziDriverMs1, LOW); //quarter step mode
    digitalWrite(aziDriverMs2, HIGH);
    digitalWrite(aziDriverMs3, LOW);
    int i_stepModeAzi = 4; //parameter for new angle calculation
    
    if(i_deltaAzimut >= 8){ //if gap is larger than 10 degrees
    
    //higher speed for large gaps  
    digitalWrite(aziDriverMs1, LOW); //aziDriverMs1,2,3 LOW for full step mode
    digitalWrite(aziDriverMs2, LOW);
    digitalWrite(aziDriverMs3, LOW);
    i_stepModeAzi = 1;      
    }


    //### ROTATION CLOCKWISE ###
    if(i_azimut >= i_currentAzimut){ //if desired azimuth is higher than current actual azimuth
    
    //then set up azimut motor driver to move azimuth clockwise
    digitalWrite(aziDriverEnable, LOW); //Set enable low to allow motor control
    digitalWrite(aziDriverDirection, HIGH); //clockwise azimuth rotation direction
    
    int i_stepsIncrease = round((f_azimut-f_currentAzimut)*aziGearRatio/(aziStepAngle*i_stepModeAzi)); //calculation of how many steps to increase (rounded to integer)
    
    for(int i_aziLoop = 0; i_aziLoop <i_stepsIncrease; i_aziLoop++){

    digitalWrite(aziDriverStep, HIGH); //trigger one step
    delay(1);
    digitalWrite(aziDriverStep, LOW); //pull step pin low so it can be triggered again
    delay(1);    
      
    }
    
    f_currentAzimut = f_currentAzimut + ((i_stepsIncrease*aziStepAngle)/(aziGearRatio*i_stepModeAzi)); //add the azimuth change to the current azimuth

    digitalWrite(aziDriverEnable, HIGH); //disable current flow
    
  }

  //### ROTATION COUNTERCLOCKWISE ###
  if(i_azimut < i_currentAzimut){ //if desired azimuth is smaller than current actual azimuth

    //then set up azimut motor driver to move azimuth counterclockwise
    digitalWrite(aziDriverEnable, LOW); //Set enable low to allow motor control
    digitalWrite(aziDriverDirection, LOW); //counterclockwise azimuth rotation direction

    int i_stepsIncrease = round((f_currentAzimut-f_azimut)*aziGearRatio/(aziStepAngle*i_stepModeAzi));
    
    for(int i_aziLoop = 0; i_aziLoop <i_stepsIncrease; i_aziLoop++){ //move steps counterclockwise

    digitalWrite(aziDriverStep, HIGH); //trigger one step
    delay(1);
    digitalWrite(aziDriverStep, LOW); //pull step pin low so it can be triggered again
    delay(1);    
      
    }
    
    f_currentAzimut = f_currentAzimut - ((i_stepsIncrease*aziStepAngle)/(aziGearRatio*i_stepModeAzi)); //subtract the azimuth change from the current azimuth
    
    digitalWrite(aziDriverEnable, HIGH); //disable current flow
      
  }



  }
}


void elevationControl(){
  //Serial1.print("Current Elevation: ");
  //Serial1.println(f_currentElevation);
  //Serial1.print("Calculated Elevation: ");
  //Serial1.println(f_elevation);

  //this code adapts the current elevation level to be as close as possible to the desired one

  int i_currentElevation = round(f_currentElevation); //comparevalue for elevation
  int i_elevation = round(f_elevation); //comparevalue for elevation

  //first make sure the maximum and minimum elevation angle are not exceeded:
  if(i_currentElevation > 90){ //failsafe in case calculations go over 90 degrees elevation
    f_currentElevation = 90;
  }

  if(i_currentElevation < 0){ //failsafe in case calculations go below 0 degrees elevation
    f_currentElevation = 0;
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

    int i_stepsIncreaseEle = round(((f_elevation-f_currentElevation)*eleGearRatio*i_stepModeEle)/eleStepAngle);

    for(int i_eleLoop = 0; i_eleLoop < i_stepsIncreaseEle; i_eleLoop++){

      digitalWrite(eleDriverStep, HIGH);
      delay(1);
      digitalWrite(eleDriverStep,LOW);
      delay(1);
    }

    f_currentElevation = f_currentElevation + ((i_stepsIncreaseEle*eleStepAngle)/eleGearRatio);
    digitalWrite(eleDriverEnable, HIGH); //disable current flow
  }

  //### move elevation platform downwards: ###
  
  if(i_currentElevation > i_elevation){

    //set up motor parameters
    digitalWrite(eleDriverEnable, LOW); //Set enable low to allow motor control
    digitalWrite(eleDriverDirection, LOW); //downwards elevation rotation

    int i_stepsIncreaseEle = round(((f_currentElevation-f_elevation)*eleGearRatio*i_stepModeEle)/eleStepAngle);

    for(int i_eleLoop = 0; i_eleLoop < i_stepsIncreaseEle; i_eleLoop++){

      digitalWrite(eleDriverStep, HIGH);
      delay(1);
      digitalWrite(eleDriverStep, LOW);
      delay(1);
    }

    f_currentElevation = f_currentElevation - ((i_stepsIncreaseEle*eleStepAngle)/eleGearRatio);
    digitalWrite(eleDriverEnable, HIGH); //disable current flow    
  }
  
}

}
  
