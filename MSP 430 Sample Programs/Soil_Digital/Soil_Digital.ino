#define Sensor P2_6
#define LEDOUT P1_6

void setup() {
  // put your setup code here, to run once:
  pinMode (P2_6, INPUT);
  pinMode (P1_6, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
   if (digitalRead(Sensor) == 0)
   {
     digitalWrite(LEDOUT, HIGH);
   }
   else
   {
      digitalWrite(LEDOUT, LOW);
   }
  
}
