void setup() {
  // put your setup code here, to run once:
  pinMode (P1_6, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly: 
  digitalWrite(P1_6, HIGH);
  delay(500);
  digitalWrite(P1_6, LOW);
  delay(500);
  
}
