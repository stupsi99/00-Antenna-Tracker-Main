#include <Arduino.h>

void runHeadings(){

  Magnetometer.readHeading();
  float magnetometerHeading = Magnetometer.heading/10.0;

  //Move to zero degrees
  while (magnetometerHeading >= 10) {

    digitalWrite(eleDriverEnable, LOW); //Low = enabled
    digitalWrite(eleDriverDirection, LOW);
    digitalWrite(eleDriverMs1, LOW);
    digitalWrite(eleDriverMs2, LOW);
    digitalWrite(eleDriverMs3, LOW);

    digitalWrite(eleDriverStep, HIGH);
    delay(1);
    digitalWrite(eleDriverStep, LOW);
    delay(1);

    disableMotors();

    Magnetometer.readHeading();
    magnetometerHeading = Magnetometer.heading/10.0;
    Serial.println(magnetometerHeading);
  }
}
