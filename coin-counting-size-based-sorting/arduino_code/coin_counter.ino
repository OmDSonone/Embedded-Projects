int irSensor = 2;
int coinCount = 0;
int lastState = HIGH;

void setup() {
  pinMode(irSensor, INPUT);
  Serial.begin(9600);
  Serial.println("Coin Counting System Started");
}

void loop() {
  int currentState = digitalRead(irSensor);

  if (currentState == LOW && lastState == HIGH) {
    coinCount++;
    Serial.print("Coin Count: ");
    Serial.println(coinCount);
    delay(300);  // debounce
  }

  lastState = currentState;
}

