#include <Adafruit_DotStar.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h> 

#define NUMSTRIP 38 // Number of LEDs in strip

// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
Adafruit_DotStar strip(NUMSTRIP, DOTSTAR_BRG);

void setup() {
  // put your setup code here, to run once:
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
}

uint32_t white = 0xFFFFFF;      // 'On' color (white)

void loop() {
  // put your main code here, to run repeatedly:
  int start = random(0, NUMSTRIP/2); 
  int finish = random(NUMSTRIP/2, NUMSTRIP); 
  int brightness = random(75, 255); 
  strike(start, finish, brightness); 
  flash(start, NUMSTRIP-start, brightness);
  int wait = random(0, 5000); 
  delay(wait);

}

void strike(int start, int finish, int brightness) {
  int cur = start; 
  int del = random(5, 25); 
  while(cur != finish) {
    strip.setPixelColor(cur, white);
    strip.setBrightness(brightness); 
    if (finish > start) cur++; 
    else cur--; 
    strip.show(); 
    delay(del); 
  }     
  strip.clear(); 
  strip.show(); 
}

void flash(int first, int count, int brightness) {
  int numFlashes = random(1, 5); 
  for (int i=0; i <= numFlashes; i++) {
    strip.fill(white, first, count);
    strip.setBrightness(brightness);  
    strip.show(); 
    delay(random(250)); 
    strip.clear(); 
    strip.show(); 
    delay(random(250)); 
  }
}
