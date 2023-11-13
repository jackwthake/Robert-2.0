#include "motor_driver.h"
#include "ultrasonic_driver.h"

auto setup(void) -> void {
  if (Serial) {
    Serial.begin(9600);
  }

  tb6612_init_driver();
  servo_init();
  ultrasonic_init();
}

auto loop(void) -> void {
  if (Serial.available() > 0) {
    int motor_a, motor_b, a_spd, b_spd, servo_pos;

    motor_a = Serial.parseInt();
    motor_b = Serial.parseInt();
    a_spd = Serial.parseInt();
    b_spd = Serial.parseInt();
    servo_pos = Serial.parseInt();

    uint8_t mask = STBY_enable;

    if (motor_a) mask | AIN_enable;
    if (motor_b) mask | BIN_enable;

    tb6612_send_command(mask, a_spd, b_spd);
    servo_send_command(servo_pos);
  }
}