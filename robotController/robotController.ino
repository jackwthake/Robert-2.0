#include "motor_driver.h"
#include "ultrasonic_driver.h"

auto setup(void) -> void {
  Serial.begin(9600);

  tb6612_init_driver();
  servo_init();
  ultrasonic_init();
}

auto loop(void) -> void {
  tb6612_send_command(STBY_enable | AIN_enable, 100, 0); 
  delay(1000);

  for (int i = 0; i < 180; ++i) {
    servo_send_command(i);
  }

  tb6612_send_command(STBY_enable | BIN_enable, 0, 100);
  delay(1000);
  tb6612_send_command(0x00, 0, 0);

  for (;;) {
    Serial.println(ultrasonic_get());
  }
}