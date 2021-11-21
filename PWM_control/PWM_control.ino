int tft_led = D8; //PWM
int pin2 = 10; //No PWM

void setup() {
  pinMode(pin, OUTPUT);
  pinMode(pin2, OUTPUT);
}

void loop() {
  for (int i = 0; i < 255; i++) {
    analogWrite(tft_led, i);
    digitalWrite(pin2, HIGH);
    delay(50);
  }
  for (int i = 255; i > 0; i--) {
    analogWrite(tft_led, i);
    digitalWrite(pin2, LOW);
    delay(50);
  }
}
