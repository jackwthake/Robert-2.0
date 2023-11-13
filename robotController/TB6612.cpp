#include "TB6612.h"

#include <Arduino.h>
#include <stdbool.h>

enum TB6612_PIN {
   TB6612_MOTOR_PWMA = 5,
   TB6612_MOTOR_PWMB = 6,
   TB6612_MOTOR_AIN_1 = 8, // the board has motor inputs duplicated?
   TB6612_MOTOR_BIN_1 = 7,
   TB6612_MOTOR_STBY = 3
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