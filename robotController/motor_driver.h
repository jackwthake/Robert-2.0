#ifndef __TB6612_H__
#define __TB6612_h__

#include <stdint.h>
#include <stdbool.h>

enum tb6612_motor_mask {
  AIN_enable = 0b00000001,
  BIN_enable = 0b00000010,
  STBY_enable = 0b00000100
};

extern void tb6612_init_driver(void);
extern void tb6612_send_command(uint8_t instr, int a_spd, int b_spd);

/* Servo Control */
extern void servo_init(void);
extern void servo_send_command(uint8_t position);
extern unsigned servo_get_last(void);

#endif