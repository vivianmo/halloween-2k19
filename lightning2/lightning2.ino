#include <APA102.h>

APA102<51, 52> ledStrip;

// Set the number of LEDs to control.
const uint16_t ledCount = 11;

// Create a buffer for holding the colors (3 bytes per color).
rgb_color colors[ledCount];

// Set the brightness to use (the maximum is 31).
const uint8_t brightness =31;

void setup() {
  // put your setup code here, to run once:
  for(uint16_t i = 0; i < ledCount; i++)
  {
       colors[i] = rgb_color(235, 112, 0);
  }
  ledStrip.write(colors, ledCount, brightness);

}

void loop() {
  // put your main code here, to run repeatedly:
  ledStrip.write(colors, ledCount, brightness);
  delay(500); 
  ledStrip.write(colors, ledCount, 0);
  delay(500); 
}
