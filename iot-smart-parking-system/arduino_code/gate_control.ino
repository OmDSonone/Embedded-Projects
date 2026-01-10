#include <Servo.h>

Servo entryGate;
Servo exitGate;

void setup() {
  entryGate.attach(3);
  exitGate.attach(5);
  Serial.begin(9600);

  entryGate.write(0);
  exitGate.write(0);
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();

    if (command == 'E') {        // Entry gate open
      entryGate.write(90);
      delay(2000);
      entryGate.write(0);
    }

    if (command == 'X') {        // Exit gate open
      exitGate.write(90);
      delay(2000);
      exitGate.write(0);
    }
  }
}
