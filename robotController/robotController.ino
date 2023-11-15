#include "motor_driver.h"
#include "ultrasonic_driver.h"

auto setup(void) -> void {
  if (Serial) {
    Serial.begin(9600);
  }

  /* Initialise hardware */
  tb6612_init_driver();
  servo_init();
  ultrasonic_init();
}

auto loop(void) -> void {
  if (Serial.available() > 0) { /* Wait for serial input*/
    int motor_a, motor_b, servo_pos;

    /* comand looks like: "motor_a_speed motor_b_speed servo_position" */
    motor_a = Serial.parseInt();
    motor_b = Serial.parseInt();
    servo_pos = Serial.parseInt();

    /* Set enable bit */
    uint8_t mask = STBY_enable;

    /* Set motor mask according to command */
    if (motor_a) mask | AIN_enable;
    if (motor_b) mask | BIN_enable;

    tb6612_send_command(mask, motor_a, motor_b);
    servo_send_command(servo_pos);
  }
}