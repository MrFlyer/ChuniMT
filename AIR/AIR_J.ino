int ADCNUM[6] = {PA0, PA1, PA4, PA5, PA6, PA7};
void setup() {
  Serial.begin(9600);
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
      Serial.print(adcData[i]);
    }
    else {
      adcData[i] = 0;
      Serial.print(adcData[i]);
    }
    if (i < 5) {
      Serial.print(" ");
    }
  }
  Serial.println();
}