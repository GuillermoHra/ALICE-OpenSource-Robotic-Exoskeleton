//Switch test

void setup() {
  Serial.begin(9600);
  pinMode(21, INPUT); //Knee back
  pinMode(20, INPUT); //Knee fron
}

void loop() {
  //Contact switches
  //Change state when digital read is '0' (switch pressed)
  //Connection: Left(digital pind and Vcc w/10 kohm resistor-pull up), middle(GND)

  Serial.print("State Front: ");
  Serial.println(digitalRead(20));
  delay(500);
  
  Serial.print("State Back: ");
  Serial.println(digitalRead(21));
  delay(500);

}
