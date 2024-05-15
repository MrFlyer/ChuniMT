int ADCNUM[6] = {A0, A1, A2, A3, A4, A5};
void setup() {
  serial.begin(9600);
  for (int i = 0; i < 6; i++) {
    pinMode(ADCNUM[i], INPUT_PULLUP);
  }
}

void loop() {
  int adcData[6] = {0, 0, 0, 0, 0, 0};
  for (int i = 0; i < 6; i++) {
    int ADCread = analogRead(ADCNUM[i]);
    if (ADCread >= 850) {
      adcData[i] = 1;
      serial.print(adcData[i]);
    }
    else {
      adcData[i] = 0;
      serial.print(adcData[i]);
    }
    if (i < 5) {
      serial.print(" ");
    }
  }
  serial.println();
}