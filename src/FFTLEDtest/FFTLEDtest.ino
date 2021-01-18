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
#define PIN 24
// #define FASTLED_ALLOW_INTERRUPTS 0
//uint8_t color = 256;
//editing test


///////////////////////////////////
// copy the Design Tool code here
///////////////////////////////////
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
// #include <SD.h>
// #include <SerialFlash.h>

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
    FastLED.addLeds<WS2812SERIAL, PIN, RGB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(90);
  Serial.begin(115200);
  AudioMemory(30);
  sgtl5000_1.enable();
  sgtl5000_1.volume(.95);
  mixer1.gain(0, .6);
  mixer1.gain(1, .6);
  // Uncomment one these to try other window functions
  //fft1024_1.windowFunction(NULL);
  //fft1024_1.windowFunction(AudioWindowBartlett1024);
  // fft1024_1.windowFunction(AudioWindowFlattop1024);

  for (int i=0; i<NUM_LEDS; ++i){
      leds[i].r = leds[i].g = leds[i].b = 0;
  }
  FastLED.show();
    delay(1000);
}



void setPixel(int Pixel, float brightness){
    
    leds[Pixel].r = int(brightness*2560)%256;
    leds[Pixel].g = int(brightness*2560)%256;
    leds[Pixel].b = int(brightness*2560)%256;
//    Serial.print(float(int(brightness*2560)%256),3);
//    Serial.print(" ");
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


  if (fft1024_1.available()) {
    // each time new FFT data is available
    // print to the Arduino Serial Monitor
    setPixel(0, fft1024_1.read(0));
     setPixel(1, fft1024_1.read(1));
     setPixel(2, fft1024_1.read(2,3));
     setPixel(3, fft1024_1.read(4,6));
     setPixel(4, fft1024_1.read(7,10));
     setPixel(5, fft1024_1.read(11,15));
     setPixel(6, fft1024_1.read(16,22));
     setPixel(7, fft1024_1.read(23,32));
     setPixel(8, fft1024_1.read(33,46));
     setPixel(9, fft1024_1.read(47,66));
     setPixel(10, fft1024_1.read(67,93));
     setPixel(11, fft1024_1.read(94,131));
     setPixel(12, fft1024_1.read(132,184));
     setPixel(13, fft1024_1.read(185,257));
     setPixel(14, fft1024_1.read(258,359));
     setPixel(15, fft1024_1.read(360,511));
    //16 sections total
    Serial.print("FFT: ");
    for (int i=0; i<30; i++) {  // 0-25  -->  DC to 1.25 kHz
      float n = fft1024_1.read(i);
      printNumber(n);
    }
    Serial.println();
    FastLED.show();
  } 
delay(30);
}
void printNumber(float n) {
  
  if (n >= 0.012) {
    Serial.print(n, 3);
    Serial.print(" ");
  } else {
    Serial.print("   -  "); // don't print "0.00"
  }
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
//}
