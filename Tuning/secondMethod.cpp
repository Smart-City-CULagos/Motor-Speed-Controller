#include <Arduino.h>

#define Kp 0
#define Ki 0
#define Kd 0

#define ENABLE 0
#define INA 1
#define INB 2

volatile byte pulses = 0;
byte motorOutput = 0;

unsigned long timeold = 0;
unsigned long feedback = 0;

const int wheelDiameter = 0.064;
const int pulsesperTurn = 20;

unsigned long setpoint = 100;
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
  digitalWrite(INA, LOW);
  digitalWrite(INB, LOW);
  delay(100);
  digitalWrite(INA, HIGH);
}

void loop() {
  if(millis() - timeold >= 100)
  {
    noInterrupts();
    feedback = (pulses * 1000 * 60) / ((millis()-timeold)*pulsesperTurn);
    motorOutput = PID(feedback, millis()-timeold);
    pulses = 0;
    analogWrite(ENABLE, motorOutput);
    timeold = millis();
    Serial.println(feedback);
    interrupts();
  }
}
