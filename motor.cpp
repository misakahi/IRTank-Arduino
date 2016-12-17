#include "motor.h"
#include <Arduino.h>

Motor::Motor(int p0, int p1, int pwm) {
  setPin(p0, p1);
  setPWM(pwm);
}

void Motor::setPin(int p0, int p1) {
  pin0_ = p0;
  pin1_ = p1;
}

void Motor::setPWM(int pin) { pin_pwm_ = pin; }

void Motor::initPinMode() {
  pinMode(pin0_, OUTPUT);
  pinMode(pin1_, OUTPUT);
  if (pin_pwm_ > 0) pinMode(pin_pwm_, OUTPUT);
}

void Motor::run(int direc, int value) {
  byte cmd = 0;
  switch (direc) {
    case NN:
      cmd = B00;  // 00
      break;
    case FW:
      cmd = B10;  // 10
      break;
    case BW:
      cmd = B01;  // 01
      break;
    case BR:
      cmd = B11;  // 11
      break;
    default:
      break;
  }
  int v0 = (cmd & 2) >> 1;
  int v1 = cmd & 1;
  digitalWrite(pin0_, v0 ? HIGH : LOW);
  digitalWrite(pin1_, v1 ? HIGH : LOW);
  if (pin_pwm_ > 0 ) {
    analogWrite(pin_pwm_, value);
  }
}

void Motor::stop() {
  digitalWrite(pin0_, LOW);
  digitalWrite(pin1_, LOW);
}
