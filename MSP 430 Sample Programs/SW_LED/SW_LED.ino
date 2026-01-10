#define LED P2_1
#define SW  P2_3

unsigned char Status;

void setup() {
  // put your setup code here, to run once:
  pinMode (LED, OUTPUT);
  pinMode(SW, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly: 
  Status = digitalRead(SW);
  if (Status == 0)
  {
    digitalWrite(LED, HIGH);
  }
  else
  {
    digitalWrite(LED, LOW);
  }
  
  
}
