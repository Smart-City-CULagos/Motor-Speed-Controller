#include <Arduino.h>
#include <Wire.h>
#include <FastPID.h>

float Kp=1, Ki=0.35, Kd=0.0005, Hz=10;
int output_bits = 8;
bool output_signed = false;
unsigned int rpm = 0;
unsigned int targetRPM = 0;
int targetDirection = 0;
float velocity = 0;
volatile byte pulses = 0;
unsigned long timeold = 0;
unsigned int pulsesperturn = 20;
const int wheel_diameter = 64;
static volatile unsigned long debounce = 0;
const int D0_pin = 2;
const int ENA_pin = 11;
const int IN1_pin = 10;
const int IN2_pin = 9;

FastPID myPID(Kp, Ki, Kd, Hz, output_bits, output_signed);

void counter()
{
  if(digitalRead(D0_pin) && (micros()-debounce > 500) && digitalRead(D0_pin))
  { 
  debounce = micros(); 
  pulses++;
  }
  else ; 
}

void receiveEvent(int requested)
{
  myPID.clear();
  String data = "";
  int x = 0;
  while(Wire.available())
  {
    data += (char)Wire.read();
  }
  analogWrite(ENA_pin, 0);
  if (data[0]=='+')
  {
    targetDirection = 1;
  }
  else
  {
    targetDirection = 0;
  }
  delay(20);
  targetRPM = atoi(&data[1]);
}

void setup() {
  pinMode(D0_pin, INPUT);
  pinMode(ENA_pin, OUTPUT);
  pinMode(IN1_pin, OUTPUT);
  pinMode(IN2_pin, OUTPUT);
  attachInterrupt(0, counter, RISING);
  Serial.begin(115200);
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
}

void loop() {
  if (millis() - timeold >= 200)
  {
    noInterrupts();
    rpm = (60 * 1000 / pulsesperturn )/ (millis() - timeold)* pulses;
    velocity = rpm * 3.1416 * wheel_diameter * 60 / 1000000;
    timeold = millis();
    pulses = 0;
    uint8_t output = myPID.step(targetRPM, rpm);
    if(targetRPM == 0)
    {
      output = 0;
    }
    analogWrite(ENA_pin, output);
    if(targetDirection)
    {
      digitalWrite(IN1_pin,HIGH);
      digitalWrite(IN2_pin,LOW);
    }
    else
    {
      digitalWrite(IN1_pin,LOW);
      digitalWrite(IN2_pin,HIGH);
    }
    interrupts();
  }
}
