#define SensorAN A3
#define LEDOUT P1_6

unsigned int ADC_OUT =0;

void setup() {
  // put your setup code here, to run once:
  pinMode (SensorAN, INPUT);
  pinMode (LEDOUT, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
   ADC_OUT = analogRead(A4);
   if (ADC_OUT > 700)
   {
    digitalWrite(LEDOUT, HIGH);
   }
   else
   {
    digitalWrite(LEDOUT, LOW);
   }
  
}
