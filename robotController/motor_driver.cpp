#include "motor_driver.h"

#include <Arduino.h>
#include <Servo.h>

/* Pin mappings for TB6612 Motor Driver */
enum TB6612_PIN {
   TB6612_MOTOR_PWMA = 5,
   TB6612_MOTOR_PWMB = 6,
   TB6612_MOTOR_AIN_1 = 8, // the board has motor inputs duplicated?
   TB6612_MOTOR_BIN_1 = 7,
   TB6612_MOTOR_STBY = 3
};


/* Set pins for motor driver */
void tb6612_init_driver(void) {
  pinMode(TB6612_MOTOR_PWMA, OUTPUT);
  pinMode(TB6612_MOTOR_PWMB, OUTPUT);
  pinMode(TB6612_MOTOR_AIN_1, OUTPUT);
  pinMode(TB6612_MOTOR_BIN_1, OUTPUT);
  pinMode(TB6612_MOTOR_STBY, OUTPUT);
}


/**
 * Send motor driver commands
 * 
 * TODO: Change this to just take in the PWM speeds rather than
 *       using a bit mask and speeds
*/
void tb6612_send_command(uint8_t instr, int a_spd, int b_spd) {
  if ((instr & STBY_enable) > 0) /* Set standby */
    digitalWrite(TB6612_MOTOR_STBY, HIGH);
  else
    digitalWrite(TB6612_MOTOR_STBY, LOW);

  if ((instr & AIN_enable) > 0) /* Set Motor A Enable*/
    digitalWrite(TB6612_MOTOR_AIN_1, HIGH);
  else 
    digitalWrite(TB6612_MOTOR_AIN_1, LOW);
  
  if ((instr & BIN_enable) > 0) /* Set Motor B Enable */
    digitalWrite(TB6612_MOTOR_BIN_1, HIGH);
  else 
    digitalWrite(TB6612_MOTOR_BIN_1, LOW);

  /* Set motor speeds */
  analogWrite(TB6612_MOTOR_PWMA, a_spd);
  analogWrite(TB6612_MOTOR_PWMB, b_spd);
}


/* Servo Control */
static const unsigned SERVO_PIN = 10;
static Servo camera_servo;


/* initialise servo, send it to the center position */
void servo_init(void) {
  servo_send_command(90); // Center servo
}


/* Send new position to servo */
void servo_send_command(uint8_t position) {
  camera_servo.attach(SERVO_PIN);
  camera_servo.write(position);
}


/* Read last sent position */
unsigned servo_get_last(void) {
  return camera_servo.read();
}
