#if !defined MOTOR_H_INCLUDED__
#define MOTOR_H_INCLUDED__

class Motor {
 private:
  int pin0_, pin1_, pin_pwm_;

 public:
  static const int NN = 0;
  static const int FW = 1;
  static const int BW = 2;
  static const int BR = 3;

  Motor();
  Motor(int p0, int p1, int pwm=-1);
  void setPin(int p0, int p1);
  void setPWM(int pin);
  void initPinMode();
  void run(int direc, int value=0);
  void stop();
};

#endif // MOTOR_H_INCLUDED__
