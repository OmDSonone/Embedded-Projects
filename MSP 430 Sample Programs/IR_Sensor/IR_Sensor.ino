#define IR_OUT  P2_6
#define BUZZER P2_0

void setup() {
  // put your setup code here, to run once:
  pinMode(IR_OUT, INPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);
  Serial.println("IR Sensor Interfacing");

}

void loop() {
  // put your main code here, to run repeatedly: 
  if (digitalRead(IR_OUT) == 1)
  {
    digitalWrite(BUZZER, HIGH);
    Serial.println("Object Detected");
  }
  else
  {
    digitalWrite(BUZZER, LOW);
  }
  
}
