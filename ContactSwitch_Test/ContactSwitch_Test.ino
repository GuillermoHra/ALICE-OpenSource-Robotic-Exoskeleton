//Switch test
int potVal;
int angle;

void setup() {
  Serial.begin(9600);
  pinMode(21, INPUT); //Knee back
  pinMode(20, INPUT); //Knee front
  pinMode(A14, INPUT); //Pot
}

void loop() {
  //Contact switches
  //Change state when digital read is '0' (switch pressed)
  //Connection: Left(digital pind and Vcc w/10 kohm resistor-pull up), middle(GND)

  Serial.print("Pot: ");
  Serial.println(analogRead(A14));
  //Serial.print("Angle: ");
  //pot_Val = analogRead(A14));
  //angle = map(pot_Val, 0, 1023, 3, 100);
  //Serial.println(angle);

  
  delay(500);

}
