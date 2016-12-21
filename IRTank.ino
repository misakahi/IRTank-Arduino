#include "motor.h"

#include <IRremote.h>


const int MASK_4BIT = B1111;
const int MASK_16BIT = 0xffff;
const int STBY = 6;
const int RECV_PIN = 10; 

IRrecv irrecv(RECV_PIN);
decode_results results;

Motor left_motor(5, 4, 3);
Motor right_motor(7, 8, 9);


/** モーターを動かす
 *
 *  @param motor モーター
 *  @param msg   命令（4bit）
 */
void work_motor(Motor& motor, int msg);

/** 受信した命令を実行する
 *
 *  @param motor モーター
 *  @param msg   命令（16bit）
 */
void work(int msg);

void dump(const decode_results* const results) {
  if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");
  } else {
    Serial.print("Decoded Unknown: ");
  }
  Serial.println(results->value, HEX);
  Serial.println();
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
  left_motor.initPinMode();
  right_motor.initPinMode();

  irrecv.enableIRIn(); // Start the receiver
}

unsigned long last_received = 0;
void loop() {
  if (irrecv.decode(&results)) {
    dump(&results);
    long value = results.value;
    long msg = (value >> 16) & 0xffff;  // 16bit
    long not_msg = value &     0xffff;

    // error check
    if (results.decode_type == NEC && (msg & not_msg) == 0) {
      work(msg);
    } else {
      Serial.println("error check failed");
    }

    irrecv.resume();
    last_received = millis();
  }
  
  if (millis() - last_received >= 108 * 4) {
    work(0);
  }
  delay(10);
}

void work(int msg) {
  work_motor(left_motor,  (msg >> 12) & MASK_4BIT);
  work_motor(right_motor, (msg >>  8) & MASK_4BIT);

  // future work
  // work_foo(right_motor, (msg >>  4) & MASK_4BIT);
  // work_bar(right_motor, (msg      ) & MASK_4BIT);
}

void work_motor(Motor& motor, int msg) {
  // 先頭のビットで向きを決める
  int direc = (msg & B1000) ? Motor::FW : Motor::BW;
  int value = (msg & B0111) * 0xff / B0111;
  // Serial.println("motor");
  // Serial.println(msg);
  // Serial.println(direc);
  //Serial.println(value);
  motor.run(direc, value);
}
