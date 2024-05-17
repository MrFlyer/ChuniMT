/*
 * 以下的函数基本上完全照搬了ilufang在
 * https://dev.s-ul.eu/ilufang/serial-slider
 * 的maindev.h
 */
#ifndef KEY_CHUNI_H
#define KEY_CHUNI_H
#include "serial.h"
#include "HID-Project.h"
#include "Adafruit_MPR121.h"
Adafruit_MPR121 capA = Adafruit_MPR121();
Adafruit_MPR121 capB = Adafruit_MPR121();
Adafruit_MPR121 capC = Adafruit_MPR121();
uint16_t keypress[16] = {'a', 'z', 's', 'x', 'd', 'c', 'f', 'v', 'g', 'b', 'h', 'n', 'j', 'm', 'k', ','};
//初始化RGB灯的颜色
struct {
  uint8_t r = 0;
  uint8_t g = 255;
  uint8_t b = 255;
} initializationRGB;
//键盘模式下，当被触摸时的颜色
struct {
  uint8_t r = 255;
  uint8_t g = 0;
  uint8_t b = 255;
} keyboardPressRGB;
//键盘模式下，当触摸后释放时的颜色
struct {
  uint8_t r = 0;
  uint8_t g = 255;
  uint8_t b = 255;
} keyboardReleaseRGB;
#define CLAMP(val, lo, hi) (val < lo ? lo : (val > hi ? hi : val))
uint8_t checkrelease[16];
uint8_t checkled = 0;
#define SEND_PIN 7


bool auto_scan = false;
void slider_set_led();
void slider_scan_start();
void slider_scan_stop();
void slider_reset();
void slider_get_board_info();
void slider_scan();
void SERIAL_NOT_FOUND();
void keyboardmod();
int caltouch(int bl, int fd);
void CON_SET() {
  SerialUSB.begin(115200);
  SerialUSB.setTimeout(0);
  Serial1.begin(9600)
  capA.begin(0x5A);
  capB.begin(0x5B);
  capC.begin(0x5C);
  NKROKeyboard.begin();
  pinMode(SEND_PIN, OUTPUT);
}
void maindev_loop() {
  switch (sliderserial_readreq()) {
    case SLIDER_CMD_SET_LED:
      slider_set_led();
      break;
    case SLIDER_CMD_AUTO_SCAN_START:
      slider_scan_start();
      break;
    case SLIDER_CMD_AUTO_SCAN_STOP:
      slider_scan_stop();
      break;
    case SLIDER_CMD_RESET:
      slider_reset();
      break;
    case SLIDER_CMD_GET_BOARD_INFO:
      slider_get_board_info();
      break;
    default:
      slider_scan();
  }
  sliderserial_writeresp();
}
#define BSD_HASH(checksum, ch) do {\
    checksum = (checksum >> 1) + ((checksum & 1) << 15);\
    checksum += ch;\
  } while(0);
void slider_set_led() {
  uint16_t prev_hash = -1;
  if (slider_tx_pending) {
    return;
  }
  uint8_t   r, g, b;
  uint16_t hash = 0;
  for (int i = 0; i < 31; i++) {
    b = slider_req.leds[i * 3 + 0];
    r = slider_req.leds[i * 3 + 1];
    g = slider_req.leds[i * 3 + 2];
    uint8_t l = i;
    leds[l].r = g;
    BSD_HASH(hash, g);
    leds[l].g = r;
    BSD_HASH(hash, r);
    leds[l].b = b;
    BSD_HASH(hash, b);
  }
  if (hash != prev_hash) {
    prev_hash = hash;
    FastLED[0].showLeds(255);
  }
  slider_req.cmd = 0;
}
void slider_scan_start() {
  auto_scan = true;
  slider_scan();
}
void slider_scan_stop() {
  auto_scan = false;
  slider_resp.syn = 0xff;
  slider_resp.cmd = SLIDER_CMD_AUTO_SCAN_STOP;
  slider_resp.size = 0;
  slider_req.cmd = 0;
}
void slider_reset() {
  capA.begin(0x5A);
  capB.begin(0x5B);
  capC.begin(0x5C);
  slider_resp.syn = 0xff;
  slider_resp.cmd = SLIDER_CMD_RESET;
  slider_resp.size = 0;
  slider_req.cmd = 0;
}
void slider_get_board_info() {
  slider_resp.syn = 0xff;
  slider_resp.cmd = SLIDER_CMD_GET_BOARD_INFO;
  slider_resp.size = sizeof(slider_resp.version);
  strcpy(slider_resp.version, "15330   \xA0" "06712\xFF" "\x90");
  slider_req.cmd = 0;
}
void slider_scan() {
  if (!auto_scan || slider_tx_pending) {
    SERIAL_NOT_FOUND();//serial尚未接到包头0xff时运行
    return;
  }
  slider_resp.syn = 0xff;
  slider_resp.cmd = SLIDER_CMD_AUTO_SCAN;
  slider_resp.size = sizeof(slider_resp.pressure);
  int16_t bv, fd, pressure;
  int i;
  for (i = 0; i < 12; i++) {
    if (i < 10) {
      bv = capA.baselineData(i);
      fd = capA.filteredData(i);
      pressure = caltouch(bv, fd);
      slider_resp.pressure[i] = CLAMP(pressure, 0, 255);
    }
    bv = capB.baselineData(i);
    fd = capB.filteredData(i);
    pressure = caltouch(bv, fd);
    slider_resp.pressure[i + 10] = CLAMP(pressure, 0, 255);
    if (i < 10) {
      bv = capC.baselineData(i);
      fd = capC.filteredData(i);
      pressure = caltouch(bv, fd);
      slider_resp.pressure[i + 10 + 12] = CLAMP(pressure, 0, 255);
    }
  }
  serial_cheak();
  aircheck();//air检测
  air_rgb_Serial();
}

void SERIAL_NOT_FOUND() {
  if (SerialUSB.available()) {
    checkserial = 1;
    return;
  }
  if (checkserial) {
    for (uint8_t i = 0; i < 32; i++) {
      leds[i] = CRGB(0, 255, 0);
    }
    FastLED.show();
    checkserial = 0;
    checkled = 0;
    capA.begin(0x5A);
    capB.begin(0x5B);
    capC.begin(0x5C);
    delay(100);
    return;
  }
  keyboardmod();
}
void keyboardmod() {
  if (SerialUSB.available()) {
    checkserial = 1;
    return;
  }
  int16_t bl, fl, cal, calpress[32];
  for (uint8_t i = 0; i < 12; i++) {
    if (i < 10) {
      bl = capA.baselineData(i);
      fl = capA.filteredData(i);
      cal = caltouch(bl, fl);
      calpress[i] = CLAMP(cal, 0, 255);
    }
    bl = capB.baselineData(i);
    fl = capB.filteredData(i);
    cal = caltouch(bl, fl);
    calpress[i + 10] = CLAMP(cal, 0, 255);
    if (i < 10) {
      bl = capC.baselineData(i);
      fl = capC.filteredData(i);
      cal = caltouch(bl, fl);
      calpress[i + 10 + 12] = CLAMP(cal, 0, 255);
    }
  }
  air_rgb_Serial();
  //    aircheck();//air检测
  for (uint8_t i = 0; i < 16; i++) {
    uint8_t calkeypress1 = calpress[i * 2];
    uint8_t calkeypress2 = calpress[i * 2 + 1];
    uint8_t val = 60;
    if (checkled <= 15) {//初始化完成后的灯
      leds[31 - i * 2] = CRGB(initializationRGB.g, initializationRGB.r, initializationRGB.b);
      leds[30 - i * 2] = CRGB(initializationRGB.g, initializationRGB.r, initializationRGB.b);
      FastLED[0].showLeds(255);
      checkled++;
    }
    if (calkeypress1 >= val || calkeypress2 >= val) {
      checkrelease[i] = SLIDER_CMD_AUTO_SCAN;
      NKROKeyboard.press(keypress[i]);
      leds[i * 2] = CRGB(keyboardPressRGB.g, keyboardPressRGB.r, keyboardPressRGB.b);
    }
    else {
      if (checkrelease[i]) {
        checkrelease[i] = 0;
        NKROKeyboard.release(keypress[i]);
        leds[i * 2] = CRGB(keyboardReleaseRGB.g, keyboardReleaseRGB.r, keyboardReleaseRGB.b);
      }
      continue;
    }
  }
  FastLED[0].showLeds(255);
}
int caltouch(int bl, int fd) {//触摸数值计算
  int cal = bl - fd;
  return cal > 4 ? 8 * cal : (cal > 0 ? cal : 0) ;
}

#endif
