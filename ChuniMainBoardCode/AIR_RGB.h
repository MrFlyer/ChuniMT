#ifndef AIR_RGB_H
#define AIR_RGB_H
#include <FastLED.h>
#include "HID-Project.h"
#define NUM_LEDS 33
#define DATA_PIN A5
#define SEND_PIN 7
CRGB leds[NUM_LEDS];

#define air_right_pin 5
#define air_right_num 6
CRGB AIR_RIGHT[air_right_num];

#define air_left_pin 9
#define air_left_num 6
CRGB AIR_LEFT[air_left_num];

uint8_t air_state[6];

uint16_t airkey[6] = {'/', '.', '\'', ';', ']', '['};
// uint16_t airpin[6] = {7, 6, 12, 10, 13, 11};
int data[6];
uint8_t airRelease[6];
uint8_t airPress[6];
long air_time = 0;

void RGBset() {
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, air_right_pin, GRB>(AIR_RIGHT, air_right_num);
  FastLED.addLeds<WS2812B, air_left_pin, GRB>(AIR_LEFT, air_left_num);
}

uint8_t air_rgb_loop = 0;
uint16_t air_colour = 0;
uint16_t air_colour_state = 0;
void air_rgb_Serial() {
  AIR_RIGHT[air_rgb_loop] = CHSV(air_colour_state, 255, 255);
  AIR_LEFT[air_rgb_loop] = CHSV(air_colour_state, 255, 255);
  air_rgb_loop++;
  if (air_rgb_loop >= 6) {
    air_colour_state++;
    air_rgb_loop = 0;
  }
  if (air_colour_state > 255) {
    air_colour_state = 0;
  }
  for (uint8_t i = 0; i < 6; i++) {
    if (air_state[i]) {
      AIR_RIGHT[i] = 0xffffff;
      AIR_LEFT[i] = 0xffffff;
    }
  }
  FastLED[1].showLeds(255);
  FastLED[2].showLeds(255);
}
void airset() {
  for (uint8_t i = 0; i < 6; i++) {
    // pinMode(airpin[i], INPUT_PULLUP);
    airPress[i] = 0;
    airRelease[i] = 0;
    air_state[i] = 0;
  }
}

void serial_cheak() {
  digitalWrite(SEND_PIN, HIGH);
  Serial.print("1");

  if (Serial.available())
  {
    String dataString = Serial.readStringUntil('\n');
    for (int i = 0; i < 6; i++) {
      data[i] = dataString[i] - '0';
      // SerialUSB.print(data[i]);
      // SerialUSB.print(" ");
    }
    // Serial.println();
  }
  digitalWrite(SEND_PIN, LOW);
}


void aircheck() {
  for (uint8_t i = 0; i < 6; i++) {
    uint8_t airstate;
    airstate = data[i];
    if (airstate == 0) {
      if (!airPress[i]) {
        NKROKeyboard.press(airkey[i]);
        airRelease[i] = 0;
        airPress[i] = 1;
        air_state[i] = 1;
      }
    }
    else {
      if (!airRelease[i]) {
        NKROKeyboard.release(airkey[i]);
        airRelease[i] = 1;
        airPress[i] = 0;
        air_state[i] = 0;
      }
      continue;
    }
  }
}
#endif
