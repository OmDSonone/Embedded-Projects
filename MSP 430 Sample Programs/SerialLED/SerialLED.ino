#define L0 P1_0
#define L1 P1_1
#define L2 P1_2
#define L3 P1_3

void setup() {
  // put your setup code here, to run once:
  pinMode (L0, OUTPUT);
  pinMode (L1, OUTPUT);
  pinMode (L2, OUTPUT);
  pinMode (L3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly: 
  digitalWrite(L0, 1);
  digitalWrite(L1, true);
  digitalWrite(L2, HIGH);
  digitalWrite(L3, HIGH);
  delay(500);
   digitalWrite(L0, 0);
  digitalWrite(L1, false);
  digitalWrite(L2, LOW);
  digitalWrite(L3, LOW);
  delay(500);
  
  
}
