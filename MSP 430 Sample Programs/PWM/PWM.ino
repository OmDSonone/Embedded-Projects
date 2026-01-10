#define PWMOUT P1_6

void setup() {
  // put your setup code here, to run once:
  pinMode(P1_6, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly: 
  for (int i=0; i<255; )
  {
    analogWrite(PWMOUT, i);
    delay(500);
    i += 10;
  }
  
}
