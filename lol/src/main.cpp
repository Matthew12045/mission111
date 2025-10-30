#include <Arduino.h>
#include <Servo.h>
Servo bocchi;
void setup() {
  // put your setup code here, to run once:
  bocchi.attach(5);
  
  pinMode(4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(4, HIGH);
  bocchi.write(0);
}
