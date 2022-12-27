#define RXD2 33
#define TXD2 4

#define com Serial
#define com_ard Serial2

auto setup() -> void {
  com.begin(9600); // To Computer
  com_ard.begin(9600, SERIAL_8N1, RXD2, TXD2); // To Arduino

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

uint8_t x = 0;

auto loop() -> void {
  com.println(++x);
  com_ard.println((int8_t)++x);
  delay(75);
}