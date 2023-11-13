#include "TB6612.h"

auto setup(void) -> void {
  tb6612_init_driver();
}

auto loop(void) -> void {
  tb6612_send_command(STBY_enable | AIN_enable, 0, 0); 
  delay(1000);
  tb6612_send_command(STBY_enable | BIN_enable, 0, 100);
  delay(1000);
}