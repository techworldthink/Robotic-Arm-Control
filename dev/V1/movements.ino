

void servo_balance(Servo &SERVO_, int &CRNT_POS) {
  Serial.println("Balance");
  SERVO_.write(0);
  CRNT_POS = 0;
  delay(10);
}

void servo_right(Servo &SERVO_, int &CRNT_POS) {
  Serial.print("Right");
  Serial.println(CRNT_POS);
  int crnt_pos = CRNT_POS;
  int pos;
  for (pos = crnt_pos; pos <= 180; pos += 1) {
    SERVO_.write(pos);
    CRNT_POS = pos;
    delay(10);
  }
}

void servo_left(Servo &SERVO_, int &CRNT_POS) {
  Serial.println("Left");
  Serial.println(CRNT_POS);
  int crnt_pos = CRNT_POS;
  int pos;
  for (pos = crnt_pos; pos >= 0; pos -= 1) {
    SERVO_.write(pos);
    CRNT_POS = pos;
    delay(10);
  }
}
