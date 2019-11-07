#include <Adafruit_DotStar.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h> 

#define NUMPIXELS 150 // Number of LEDs in strip
#define NUMLEDS 96 //Number of LEDS in ring 

// Here's how to control the LEDs from any two pins:
#define LEDPIN  6
#define MICPIND 22
#define MICPINA A0 

#define MIC_SAVED 10 

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
Adafruit_DotStar strip(NUMPIXELS, DOTSTAR_BRG);
Adafruit_NeoPixel rings = Adafruit_NeoPixel(NUMLEDS, LEDPIN);

void setup() {
  // put your setup code here, to run once:
  pinMode(MICPIND, INPUT);
  pinMode(MICPINA, INPUT);  
  Serial.begin(9600);
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  rings.begin(); 
  rings.show(); 
}

// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

uint32_t white = 0xFFFFFF;      // 'On' color (white)
int micThreshold = 400; 
int minBrightness = 75; 
boolean on = false;

int micPastLength = MIC_SAVED; 
int micPast[MIC_SAVED]; 
int micIndex = 0; 
int micTotal = 0; 

int offThreshold = 50; 
int offCount = 0; 

int sample = 0; 


void loop() {
  // put your main code here, to run repeatedly:
  micAnalog(MICPINA); 
  int readValue = readSerial();  
  if (readValue >= 0) {
    micThreshold = readValue; 
    Serial.println(micThreshold); 
  }
//  strike(0, 13); 
//  flash(0, 13); 
//  delay(random(10000)); 
}

int readSerial() {
  String inString = ""; 
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      // clear the string for new input:
      return inString.toInt(); 
    }
  }
  return -1; 
}

void micAnalog(int pin) {
  int sum = 0; 
  for (int i = 0; i < 10; i++) {
    sum += analogRead(pin); 
  }
  int mic = sum/10; 

//  micTotal -= micPast[micIndex]; 
//  micTotal += mic; 
//  micPast[micIndex] = mic; 
//  if (micIndex == micPastLength) {
//    micIndex = 0; 
//  }
//  int micAverage = micTotal/micPastLength; 
  
  if (mic < micThreshold) {
    allWhite(0, NUMPIXELS, NUMLEDS, calculateBrightness(mic)); 
    offCount = 0; 
  } else {
    offCount++; 
    if (offCount > offThreshold) {
      Serial.println(offCount); 
      allOff(); 
    }
  }
}

int calculateBrightness(int mic) {
  float factor = micThreshold/255.0; 
  int brightness = (-mic/factor) + 255; 
  return max(brightness, minBrightness); 
}

void slowAllWhite(int first, int count, int countRings, int brightness) {
  Serial.println("slowAllWhite");
  int loop = 10;  
  int increment = brightness/loop; 
  int curBrightness = 0; 
  for (int i = 0; i < loop; i++) {
    curBrightness += increment; 
    strip.fill(white, first, count); 
    strip.setBrightness(curBrightness); 
    strip.show(); 
    rings.fill(white, 0, countRings); 
    rings.setBrightness(curBrightness); 
    rings.show(); 
    delay(25); 
  }
}

void allWhite(int first, int count, int countRings, int brightness) {
  samplePrint("on"); 
  if (offCount > offThreshold) {
    slowAllWhite(first, count, countRings, brightness); 
  }
  strip.fill(white, first, count); 
  strip.setBrightness(brightness); 
  strip.show(); 

  rings.fill(white, 0, countRings); 
  rings.setBrightness(brightness); 
  rings.show(); 
  on = true; 
}

void allOff() {
  samplePrint("off\n"); 
  strip.clear(); 
  strip.show();   
  rings.clear(); 
  rings.show();   
  on = false; 
}

void samplePrint(String msg) {
  sample++; 
  if (sample == 1) {
    //Serial.print(msg); 
    sample = 0; 
  }
}

void strike(int start, int finish) {
  int cur = start; 
  while(cur != finish) {
    strip.setPixelColor(cur, white);
    if (finish > start) cur++; 
    else cur--; 
    strip.show(); 
    delay(20); 
  }     
  strip.clear(); 
  strip.show(); 
}

void flash(int first, int count) {
  int numFlashes = random(1, 7); 
  for (int i=0; i <= numFlashes; i++) {
    strip.fill(white, first, count); 
    strip.show(); 
    delay(random(250)); 
    strip.clear(); 
    strip.show(); 
    delay(random(250)); 
  }
}
