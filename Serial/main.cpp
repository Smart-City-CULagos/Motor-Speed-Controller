#include <Arduino.h>
#include "constants.h"

volatile byte pulses = 0;
byte motorOutput = 0;
bool direction = 0;

unsigned long timeold = 0;
unsigned long rpm = 0;
unsigned long vel = 0;

const int wheelDiameter = 0.064;
const int pulsesperTurn = 20;

unsigned long setpoint = 0;
int error = 0;
int previous_error = 0;
int integral = 0;
int derivative = 0;

void encoderISR()
{
  pulses++;
}

byte PID(int rpm, int dt)
{
  error = setpoint - rpm;
  integral = integral + error * dt;
  derivative = (error - previous_error) / dt;
  previous_error = error;
  return Kp * error + Ki * integral + Kd * derivative;
}

void setup() {
  #if DEBUG
    Serial.begin(115200);
  #endif
  attachInterrupt(0, encoderISR, RISING);
}

void loop() {
  if(Serial.available())
  {
    setpoint = Serial.parseInt();
    error = 0;
    integral = 0;
    derivative = 0;
    previous_error = 0;
  }
  if(millis() - timeold >= 100)
  {
    noInterrupts();
    rpm = (pulses * 1000 * 60) / ((millis()-timeold)*pulsesperTurn);
    vel = wheelDiameter/2 * rpm * 0.104719;
    pulses = 0;
    motorOutput = PID(rpm, millis()-timeold);
    analogWrite(ENABLE, 0);
    if(direction)
    {
      digitalWrite(INA, LOW);
      digitalWrite(INB, LOW);
      delayMicroseconds(20);
      digitalWrite(INA, HIGH);
      digitalWrite(INB, LOW);
    }
    else
    {
      digitalWrite(INA, LOW);
      digitalWrite(INB, LOW);
      delayMicroseconds(20);
      digitalWrite(INA, LOW);
      digitalWrite(INB, HIGH);
    }
    analogWrite(ENABLE, motorOutput);
    #if DEBUG
      Serial.print(setpoint);
      Serial.print("/t");
      Serial.print(rpm);
      Serial.print("/t");
      Serial.print(motorOutput);
      Serial.print("/t");
      Serial.print(previous_error);
    #endif
    timeold = millis();
    interrupts();
  }
}
