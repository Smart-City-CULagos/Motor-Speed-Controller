#include <Arduino.h>
#include "constants.h"

volatile byte pulses = 0;
byte motorOutput = 0;
bool direction = 0;

unsigned long timeold = 0;
unsigned long feedback = 0;

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

byte PID(int feedbackValue, int dt)
{
  error = setpoint - feedbackValue;
  integral = integral + error * dt;
  derivative = (error - previous_error) / dt;
  previous_error = error;
  return Kp * error + Ki * integral + Kd * derivative;
}

void setup() {
  Serial.begin(115200);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  attachInterrupt(0, encoderISR, RISING);
}

void loop() {
  if(Serial.available())
  {
    direction = Serial.read();
    setpoint = Serial.parseInt();
    while(Serial.available())
    {
      Serial.read();
    }
    error = 0;
    integral = 0;
    derivative = 0;
    previous_error = 0;
  }
  if(millis() - timeold >= 100)
  {
    noInterrupts();
    feedback = (pulses * 1000 * 60) / ((millis()-timeold)*pulsesperTurn);
    motorOutput = PID(feedback, millis()-timeold);
    pulses = 0;
    analogWrite(ENABLE, 0);
    if(direction=='+')
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
      Serial.print("Target: ");
      Serial.print(setpoint);
      Serial.print("/t");
      Serial.print("RPM: ");
      Serial.print(feedback);
      Serial.print("/t");
      Serial.print("Error: ");
      Serial.print(previous_error);
      Serial.print("/t");
      Serial.print("Output: ");
      Serial.println(motorOutput);      
    #endif
    timeold = millis();
    interrupts();
  }
}
