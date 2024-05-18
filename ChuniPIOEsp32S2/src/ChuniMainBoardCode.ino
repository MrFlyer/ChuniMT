#include "AIR_RGB.h"
#include "keyboard_chuni.h"
void setup() {
  RGBset();
  airset();
  CON_SET();
}
void loop() {
  maindev_loop(); 
}
