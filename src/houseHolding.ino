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
