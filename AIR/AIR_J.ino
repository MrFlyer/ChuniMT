int ADCNUM[6] = {PA0, PA1, PA4, PA5, PA6, PA7};

void setup()
{
  Serial.begin(9600);
  Serial.println("OK");
  for (int i = 0; i < 6; i++)
  {
    pinMode(ADCNUM[i], INPUT_PULLUP);
  }
}

void loop()
{
  String inString="";
  if (Serial.available())
  {
    inString = char(Serial.read());
    if (inString == "1")
    {
      String data = "";
      for (int i = 0; i < 6; i++)
      {
        int ADCread = analogRead(ADCNUM[i]);
        if (ADCread >= 850)
        {
          // adcData[i] = 1;
          // Serial.print(adcData[i]);
          data += "0";
        }
        else
        {
          // adcData[i] = 0;
          // Serial.print(adcData[i]);
          data += "1";
        }
      }
      Serial.println(data);
    }
  }
}
