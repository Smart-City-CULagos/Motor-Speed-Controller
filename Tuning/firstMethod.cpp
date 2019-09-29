#include <Arduino.h>

#define ENABLE 0 //Enable Pin of the H bridge
#define INA 1 //Input A of the H bridge
#define INB 2 //Input B of the H bridge

volatile byte pulses;

unsigned long timeold = 0;
const int pulsesperTurn = 20;

void encoderISR()
{
  pulses++;
}

void setup() {
  Serial.begin(115200);
  attachInterrupt(0, encoderISR, RISING);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  digitalWrite(INA, LOW);
  digitalWrite(INB, LOW);
  delay(100);
  digitalWrite(INA, HIGH);
  analogWrite(ENABLE, 255);
}

void loop() {
  if(millis() - timeold >= 100)
  {
    noInterrupts();
    Serial.println((pulses * 1000 * 60) / ((millis()-timeold)*pulsesperTurn));
    pulses = 0;
    timeold = millis();
    interrupts();
  }
}
