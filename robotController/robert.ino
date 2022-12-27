auto setup() -> void {
  Serial.begin(9600);
}

auto loop() -> void {
  if (Serial.available() > 0) {
    Serial.println((int8_t)Serial.read());
  }
}
