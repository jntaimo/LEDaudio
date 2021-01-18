// Advanced Microcontroller-based Audio Workshop
//
// http://www.pjrc.com/store/audio_tutorial_kit.html
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
// 
// Part 3-2: Fourier Transform
#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>
#define NUM_LEDS 16
CRGB leds[NUM_LEDS];
#define PIN 8
//editing test


///////////////////////////////////
// copy the Design Tool code here
///////////////////////////////////
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputUSB            usb1;           //xy=212.20001220703125,211.1999969482422
AudioMixer4              mixer1;         //xy=438.20001220703125,224.1999969482422
AudioOutputUSB           usb2;           //xy=576.1999969482422,460.1999969482422
AudioAnalyzeFFT1024      fft1024_1;      //xy=602.1999969482422,341.1999969482422
AudioOutputI2S           i2s1;           //xy=614.1999969482422,212.1999969482422
AudioConnection          patchCord1(usb1, 0, mixer1, 0);
AudioConnection          patchCord2(usb1, 1, mixer1, 1);
AudioConnection          patchCord3(mixer1, 0, i2s1, 0);
AudioConnection          patchCord4(mixer1, 0, i2s1, 1);
AudioConnection          patchCord5(mixer1, fft1024_1);
AudioConnection          patchCord6(mixer1, 0, usb2, 1);
AudioConnection          patchCord7(mixer1, 0, usb2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=362.1999969482422,408.1999969482422
// GUItool: end automatically generated code



void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812SERIAL, PIN, RGB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(.95);
  mixer1.gain(0, 1);
  mixer1.gain(1, 1);
  // Uncomment one these to try other window functions
  //fft1024_1.windowFunction(NULL);
  //fft1024_1.windowFunction(AudioWindowBartlett1024);
  // fft1024_1.windowFunction(AudioWindowFlattop1024);
  delay(1000);

}




void loop() {

  // print Fourier Transform data to the Arduino Serial Monitor
//    if (fft1024_1.available()) {
//    
//    Serial.print("FFT: ");
//    for (int i=0; i<30; i++) {  // 0-25  -->  DC to 1.25 kHz
//      float n = fft1024_1.read(i);
//      printNumber(n);
//    }
//    Serial.println();
//  }
rainbowCycle(10);
}
void printFFT(){
  if (fft1024_1.available()) {
    // each time new FFT data is available
    // print to the Arduino Serial Monitor
    Serial.print("FFT: ");
    printNumber(fft1024_1.read(0));
    printNumber(fft1024_1.read(1));
    printNumber(fft1024_1.read(2,3));
    printNumber(fft1024_1.read(4,6));
    printNumber(fft1024_1.read(7,10));
    printNumber(fft1024_1.read(11,15));
    printNumber(fft1024_1.read(16,22));
    printNumber(fft1024_1.read(23,32));
    printNumber(fft1024_1.read(33,46));
    printNumber(fft1024_1.read(47,66));
    printNumber(fft1024_1.read(67,93));
    printNumber(fft1024_1.read(94,131));
    printNumber(fft1024_1.read(132,184));
    printNumber(fft1024_1.read(185,257));
    printNumber(fft1024_1.read(258,359));
    printNumber(fft1024_1.read(360,511));
    Serial.println();
  }
}

void printNumber(float n) {
  
  if (n >= 0.014) {
    Serial.print(n, 3);
    Serial.print(" ");
  } else {
    Serial.print("   -  "); // don't print "0.00"
  }
  
//  if (n > 0.25) {
//    Serial.print("***** ");
//  } else if (n > 0.18) {
//    Serial.print(" ***  ");
//  } else if (n > 0.06) {
//    Serial.print("  *   ");
//  } else if (n > 0.005) {
//    Serial.print("  .   ");
//  }
}
void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));  
      printFFT();  
    }

    showStrip();
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
 
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}
//***REPLACE TO HERE***
void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}
