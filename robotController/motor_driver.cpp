#include "motor_driver.h"

#include <stdbool.h>
#include <Arduino.h>

#include <Servo.h>

enum TB6612_PIN {
   TB6612_MOTOR_PWMA = 5,
   TB6612_MOTOR_PWMB = 6,
   TB6612_MOTOR_AIN_1 = 8, // the board has motor inputs duplicated?
   TB6612_MOTOR_BIN_1 = 7,
   TB6612_MOTOR_STBY = 3
};

enum SERVO_PIN {
  SERVO_PIN_Z = 10,
  SERVO_PIN_Y = 11
};

void tb6612_init_driver(void) {
  pinMode(TB6612_MOTOR_PWMA, OUTPUT);
  pinMode(TB6612_MOTOR_PWMB, OUTPUT);
  pinMode(TB6612_MOTOR_AIN_1, OUTPUT);
  pinMode(TB6612_MOTOR_BIN_1, OUTPUT);
  pinMode(TB6612_MOTOR_STBY, OUTPUT);
}


void tb6612_send_command(uint8_t instr, int a_spd, int b_spd) {
  if ((instr & STBY_enable) > 0)
    digitalWrite(TB6612_MOTOR_STBY, HIGH);
  else
    digitalWrite(TB6612_MOTOR_STBY, LOW);

  if ((instr & AIN_enable) > 0)
    digitalWrite(TB6612_MOTOR_AIN_1, HIGH);
  else 
    digitalWrite(TB6612_MOTOR_AIN_1, LOW);
  
  if ((instr & BIN_enable) > 0)
    digitalWrite(TB6612_MOTOR_BIN_1, HIGH);
  else 
    digitalWrite(TB6612_MOTOR_BIN_1, LOW);

  analogWrite(TB6612_MOTOR_PWMA, a_spd);
  analogWrite(TB6612_MOTOR_PWMB, b_spd);
}

/* Servo Control */
static const unsigned SERVO_PIN = 10;
static Servo camera_servo;

void servo_init(void) {
  camera_servo.attach(SERVO_PIN);
  camera_servo.attach(SERVO_PIN);
  camera_servo.write(90); // Center Servo
  delay(50);
  camera_servo.detach();
}


extern void servo_send_command(uint8_t position) {
  camera_servo.attach(SERVO_PIN);
  camera_servo.write(position);
  
  delay(50);

  camera_servo.detach();
}


extern unsigned servo_get_last(void) {
  return camera_servo.read();
}

