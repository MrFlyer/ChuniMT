#define DATA_PIN A0
int pinNUM[6] = {PB0, PB1, PB2, PB3, PF0, PF1};

void setup()
{
    Serial.begin(9600);
    pinMode(DATA_PIN, INPUT);
    for (int i = 0; i < 6; i++)
    {
        pinMode(pinNUM[i], OUTPUT);
    }
}

void loop()
{
    if (analogRead(DATA_PIN) > 512)
    {

        for (int i = 0; i < 6; i++)
        {
            digitalWrite(pinNUM[i], HIGH);
            delayMicroseconds(50);
            digitalWrite(pinNUM[i], LOW);
        }
    }
}