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
    Serial.println(analogRead(DATA_PIN));
    if (analogRead(DATA_PIN) > 500)
    {
        Serial.println("HIGH");
        for (int i = 0; i < 6; i++)
        {
            digitalWrite(pinNUM[i], HIGH);
        }
    }
    if (analogRead(DATA_PIN) < 200)
    {
        Serial.println("LOW");
        for (int i = 0; i < 6; i++)
        {
            digitalWrite(pinNUM[i], LOW);
        }
    }
}
