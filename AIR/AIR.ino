int ADCNUM[6] = {A0, A1, A2, A3, A4, A5};
void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(ADCNUM[i], INPUT_PULLUP);
  }
}

void loop() {
  for (int i = 0; i < 6; i++) {
    int ADCread = analogRead(ADCNUM[i]);
    if (ADCread >= 850) {
      digitalWrite(pinNUM[i + 6], LOW);
    }
    else {
      digitalWrite(pinNUM[i + 6], HIGH);
    }
  }