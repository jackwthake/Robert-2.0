#include "ultrasonic_driver.h"

#include <Arduino.h>

enum ultrasonic_pin {
  US_TRIGGER = 13,
  US_ECHO = 12
};

extern void ultrasonic_init(void) {
  pinMode(US_ECHO, INPUT);
  pinMode(US_TRIGGER, OUTPUT);
}


extern uint16_t ultrasonic_get(void) {
  unsigned dist = 0;

  digitalWrite(US_TRIGGER, LOW); // pulsse trigger to start measurement
  delayMicroseconds(2);
  digitalWrite(US_TRIGGER, HIGH);

  delayMicroseconds(10);
  digitalWrite(US_TRIGGER, LOW);
  dist = ((unsigned int)pulseIn(US_ECHO, HIGH) / 58);

  return dist > 150 ? 150 : dist;
}