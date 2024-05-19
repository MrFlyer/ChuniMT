#include "cdcusb.h"
#include "hidkeyboard.h"

HIDkeyboard HidKeyboard;
CDCusb USBSerial;

class MyUSBCallbacks : public CDCCallbacks {
    void onCodingChange(cdc_line_coding_t const* p_line_coding)
    {
        int bitrate = USBSerial.getBitrate();
        Serial.printf("new bitrate: %d\n", bitrate);
    }

    bool onConnect(bool dtr, bool rts)
    {
        Serial.printf("connection state changed, dtr: %d, rts: %d\n", dtr, rts);
        return true;  // allow to persist reset, when Arduino IDE is trying to enter bootloader mode
    }

    void onData()
    {
        int len = USBSerial.available();
        Serial.printf("\nnew data, len %d\n", len);
        uint8_t buf[len] = {};
        USBSerial.read(buf, len);
        Serial.write(buf, len);
    }

    void onWantedChar(char c)
    {
        Serial.printf("wanted char: %c\n", c);
    }
};


void setup()
{
    // Serial.begin(115200);
    USBSerial.setCallbacks(new MyUSBCallbacks());
    USBSerial.setWantedChar('x');
    HidKeyboard.begin();

    if (!USBSerial.begin())
        Serial.println("Failed to start CDC USB stack");

}

void loop()
{
    while (USBSerial.available())
    {
        int len = USBSerial.available();
        uint8_t buf1[len];
        USBSerial.read(buf1, len);
        int a = USBSerial.write((uint8_t*)buf1, len);
        HidKeyboard.sendChar(*buf1);
    }
}

