

void servo_balance(Servo &SERVO_, int &CRNT_POS) {
  Serial.println("Balance");
  SERVO_.write(0);
  CRNT_POS = 0;
  delay(10);
}


void servo_control(Servo &SERVO_, int CRNT_POS) {
    SERVO_.write(CRNT_POS);
}
