#include <Arduino.h>

void disableMotors(){

  //resetting all driver pins to default states (important to disable current flow)
  digitalWrite(eleDriverStep, LOW);
  digitalWrite(eleDriverDirection, LOW);
  digitalWrite(eleDriverMs1, LOW);
  digitalWrite(eleDriverMs2, LOW);
  digitalWrite(eleDriverMs3, LOW);
  digitalWrite(eleDriverEnable, HIGH);

  digitalWrite(aziDriverStep, LOW);
  digitalWrite(aziDriverDirection, LOW);
  digitalWrite(aziDriverMs1, LOW);
  digitalWrite(aziDriverMs2, LOW);
  digitalWrite(aziDriverMs3, LOW);
  digitalWrite(aziDriverEnable, HIGH);

}

void ledIntro(){

  for(int i_fade=0; i_fade<255; i_fade++){
    analogWrite(LedRing, i_fade);
    delay(5);
  }

}

void setupBluetooth(){

  //has to be done every bootup (to change baudrate to 9600)
  Serial1.print("$"); //configuration commands
  Serial1.print("$");
  Serial1.print("$");
  delay(1000);
  Serial1.println("U,9600,N"); //configuration for 9600 baudrate
  delay(1000);
  Serial1.begin(9600);

}

void setupMagnetometer(){

  delay(500);
  Magnetometer.init();
  /*if (!Magnetometer.init()){

    Serial.println("Sensor Initialization Failed\n\r");

  }
  */
}

void updateAngles(){

  const long gpsUpdateInterval = 1000;
  int positionValid = 0;

  while(Serial.available()){

    gpsString = Serial.read();
    Serial1.write(gpsString); //GPS string to Bluetooth Module
    if(gpsSensor.encode(gpsString)){

      gpsSensor.f_get_position(&UAVLatitude,&UAVLongitude);
      UAVAltitude = gpsSensor.f_altitude();
      //tone(buzzerPin,1000,50);
      positionValid = 1;

    }

    currentGpsTimestamp = millis();

    if(positionValid == 1 && currentGpsTimestamp - previousGpsTimestamp >= gpsUpdateInterval){

      previousGpsTimestamp = currentGpsTimestamp;
      angleCalculation();
      positionValid = 0;
      tone(buzzerPin,2000,50);

    }
  }
}

void setHomePosition(){

  while(Serial.available()){

    gpsString = Serial.read();
    if(gpsSensor.encode(gpsString)){

      gpsSensor.f_get_position(&HomeLatitude,&HomeLongitude);
      HomeAltitude = gpsSensor.f_altitude();

    }
  }
}

void checkBattery(){

  int batteryValue = analogRead(batteryPin);
  float pinVoltage = batteryValue * (5.0 / 1023.0);
  float batteryVoltage = (pinVoltage * 13.15) / 4.7; //13.15kOhm and 4.7kOhm resistor

  int iBatteryVoltage = round(batteryVoltage * 100); //times hundred for more accuracy

  if(iBatteryVoltage < 1140){

    batteryAlarm();

  }

}

void batteryAlarm(){

  int batteryAlarmInterval = 5000;

  if(currentBatteryAlarmTimestamp - previousBatteryAlarmTimestamp > batteryAlarmInterval){

    tone(buzzerPin,2000,200);
    tone(buzzerPin,1500,200);
    tone(buzzerPin,1000,200);

  }

}
