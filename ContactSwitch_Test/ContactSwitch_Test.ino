//Switch test

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
}

void loop() {
  //Contact switches
  //Change state when digital read is '0' (switch pressed)
  //Connection: Left(digital pind and Vcc w/10 kohm resistor-pull up), middle(GND)
  Serial.print("State: ");
  Serial.println(digitalRead(2));
  delay(50);
}
