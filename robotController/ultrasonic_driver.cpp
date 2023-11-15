#include "ultrasonic_driver.h"

#include <Arduino.h>

/* Pin Mappings for ultrasonic sensor */
enum ultrasonic_pin {
  US_TRIGGER = 13,
  US_ECHO = 12
};


/* Initialise Pins */
void ultrasonic_init(void) {
  pinMode(US_ECHO, INPUT);
  pinMode(US_TRIGGER, OUTPUT);
}


/* Poll data from distance sensor */
uint16_t ultrasonic_get(void) {
  unsigned dist = 0;

  digitalWrite(US_TRIGGER, LOW); // pulsse trigger to start measurement
  delayMicroseconds(2);
  digitalWrite(US_TRIGGER, HIGH);

  delayMicroseconds(10); // wait for echo
  digitalWrite(US_TRIGGER, LOW);
  dist = ((unsigned int)pulseIn(US_ECHO, HIGH) / 58); // compute distance

  // max distance is 150
  return dist > 150 ? 150 : dist;
}