#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>
#define NUM_LEDS 24
CRGB leds[NUM_LEDS];
#define PIN 24
#define POT_PIN 38
#define MAX_BRIGHTNESS 150

#include <Bounce2.h>
#define POT_1_PIN 18
#define NUM_POTS 6
#define BUTTON_1_PIN 1
#define NUM_BUTTONS 4
#define HWSERIAL Serial8
#define NUM_DISPLAY_MODES 5
int analogReadings[NUM_POTS];
int digitalReadings[NUM_BUTTONS];
float FFTValues[NUM_LEDS];
int LEDOffset = 0;
Bounce2::Button buttons[NUM_BUTTONS];
unsigned long previousSensorRead = 0;
unsigned long sensorReadInterval = 20;
unsigned long previousParameterRead = 0;
unsigned long parameterReadInterval = 20;
bool buttonUpdate = false;
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


// GUItool: begin automatically generated code
AudioInputUSB            usb1;           //xy=178,201
AudioAnalyzeFFT256       fft256_R;       //xy=357,396
AudioAnalyzeRMS          rmsR;           //xy=368,335
AudioAnalyzeRMS          rmsL;           //xy=414,71
AudioAnalyzeFFT256       fft256_L;       //xy=415,32
AudioMixer4              mixer1;         //xy=465,150
AudioOutputUSB           usb2;           //xy=652,200
AudioAnalyzeRMS          rmsLR;           //xy=682,132
AudioOutputI2S           i2s1;           //xy=765,358
AudioConnection          patchCord1(usb1, 0, fft256_L, 0);
AudioConnection          patchCord2(usb1, 0, usb2, 0);
AudioConnection          patchCord3(usb1, 0, rmsL, 0);
AudioConnection          patchCord4(usb1, 0, mixer1, 0);
AudioConnection          patchCord5(usb1, 1, fft256_R, 0);
AudioConnection          patchCord6(usb1, 1, usb2, 1);
AudioConnection          patchCord7(usb1, 1, rmsR, 0);
AudioConnection          patchCord8(usb1, 1, mixer1, 1);
AudioConnection          patchCord9(mixer1, rmsLR);
// GUItool: end automatically generated code


//Start and stop indices of FFT bins
//this is 256 sample fft so there are 256/2 = 128 usable bins
//This range from startBin[i] to stopBin[i] is mapped to led[i]
//cqt - replacement
const int startBins[12] = {0, 1, 2, 3, 5,  8, 12, 18, 26, 36, 56, 81};
const int stopBins[12] =  {0, 1, 2, 4, 7, 11, 17, 25, 35, 55, 80, 127};

unsigned long previousPotMillis = 0;
const long potInterval = 200;
unsigned long previousDisplayMillis = 0;
long displayInterval = 30;
void printNumber(float n);
int displayMode = 0;

void setup() {
  FastLED.addLeds<WS2812SERIAL, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(50);
  Serial.begin(115200);
  Serial.println("Initializing...");
  analogReadResolution(8);
  AudioMemory(15);
  //clears out the leds before beginning
  for (int i=0; i<NUM_LEDS; ++i){
    leds[i].r = leds[i].g = leds[i].b = 0;
  }
  //set up buttons
  for (int i = 0; i < NUM_BUTTONS; i++){
    buttons[i].attach(BUTTON_1_PIN + i, INPUT_PULLDOWN);
    buttons[i].interval(5);
    buttons[i].setPressedState(HIGH);
  }
  //set up potentiometers
  for (int i = 0; i < NUM_POTS; i++)
    pinMode(POT_1_PIN+i, INPUT);
  FastLED.show();
  delay(500);
}


uint8_t baseHue = 0;
uint8_t maxHue = 50;
uint8_t hueRange = maxHue - baseHue;
float maxFFT = 0.05;

float maxRMS = 0.5;
int sinRate = 150;
void updatePixels(){
  switch (displayMode){
    
    case 0://basic white fft setup
      for (int i = 0; i < 24; i++){
        int whiteVal = int(FFTValues[i]*2560)%256;
        leds[i].r = whiteVal;
        leds[i].g = whiteVal;
        leds[i].b = whiteVal;
      }  
      break;
    case 1: //maps to colors instead of brightness
      for (int i = 0; i < 24; i++){
        leds[(i+LEDOffset)%NUM_LEDS].setHue(abs(map(FFTValues[i],0,maxFFT,baseHue,maxHue)));
      }
      break;
    case 2:
      float LRMS = constrain(rmsL.read()*map(analogReadings[2], 0, 255, 0, 100), 0, 12);
      float RRMS = constrain(rmsR.read()*map(analogReadings[2], 0, 255, 0, 100), 0 ,12);
      // Serial.print("L ");
      // Serial.print(FFTValues[0]);
      // Serial.print(" R ");
      // Serial.println(FFTValues[NUM_LEDS-1]);
      for (int i = 0; i < NUM_LEDS; i++){
        leds[i].subtractFromRGB(10);
      }
      for (int i = 0; i < LRMS; i++){
        leds[i].setHue(constrain(i*NUM_LEDS+(int)LRMS, 0, 255));
      }
      for (int i = 0; i < RRMS; i++){
        leds[12+ (11-i)].setHue(constrain(i*NUM_LEDS+(int)RRMS, 0, 255));
      }
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
  if (displayMode == 3){
    Serial.println(sin((float)millis()/sinRate));
  } else if (displayMode == 4){

  }
}
//maps values from the input to the output range, but with float computations instead of integer operations.
long floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (float)(x - in_min) * (float)(out_max - out_min) / (float)(in_max - in_min) + out_min;
}
//Reads a series of potentiometer values and stores them in analogReadings
void readPots() {
  for (int i = 0; i < NUM_POTS; i++){
    analogReadings[i] = analogRead(POT_1_PIN + i);
  }
}

void printSensors(){
  
    for (int i = 0; i < NUM_POTS; i++){
      Serial.print("P");
      Serial.print(i);
      Serial.print(": ");
      Serial.print(analogReadings[i]);
      Serial.print('\t'); 
    }
    for ( int i = 0; i < NUM_BUTTONS; i++){
      Serial.print("B");
      Serial.print(i);
      Serial.print(": ");
      Serial.print(digitalReadings[i]? '!':'.');
      Serial.print('\t');
    }
    Serial.println();
}

void updateButtons(bool printOutput = false){
   buttonUpdate = false;
  for (int i = 0; i < NUM_BUTTONS; i++){
    buttons[i].update();
    if (buttons[i].pressed()){
      digitalReadings[i] = 1;
      buttonUpdate = true;
    } else {
      digitalReadings[i] = 0;
    }
  }
  if (buttonUpdate) {
    readPots();
    if (printOutput) printSensors();
    previousSensorRead = millis();
  }    
  updateDisplayModes(0,1);
}

void updatePots(bool printOutput = false){
  if (millis() - previousSensorRead > sensorReadInterval){
    previousSensorRead = millis();
    readPots();
    if (printOutput) printSensors();
  }  
}

void updateBrightness(){
  if (millis() - previousPotMillis >= potInterval){
    previousPotMillis = millis();
    FastLED.setBrightness(analogRead(POT_PIN)*MAX_BRIGHTNESS/255);
  }  
}

void updateFFT(bool printOutput = false){
 if (millis() - previousDisplayMillis > displayInterval){
    previousDisplayMillis = millis();
    if (fft256_L.available() || fft256_R.available()) {
        // each time new FFT data is available
        // print to the Arduino Serial Monitor
        
        for (int i = 0; i < 12; i++){
            //left side
            FFTValues[i] = fft256_L.read(startBins[i], stopBins[i]);
            FFTValues[12 + (11 - i)] = fft256_R.read(startBins[i], stopBins[i]);            
        }
        updatePixels();

        FastLED.show();
      }   
    }
}

void printNumber(float n) {
  if (n >= 0.01) {
    Serial.print(n, 3);
    Serial.print(" ");
  } else {
    Serial.print("   -  "); // don't print "0.00"
  }
}

void updateDisplayInterval(int potNum){
  displayInterval = analogReadings[potNum];
}
void updateNumFFT(int potNum){
  fft256_L.averageTogether(potNum/10);
  fft256_R.averageTogether(potNum/10);  
}
void updateMaxHue(int potNum){
  maxHue = analogReadings[potNum];
}
void updateLEDOffset(int potNum){
  LEDOffset = map(analogReadings[potNum], 0, 255, 0,23);
}
void updateSineRate(int potNum){
  sinRate = analogReadings[potNum];
}
void updateDisplayModes(int button1, int button2){
  if (digitalReadings[button1] && (displayMode > 0 )){
      displayMode--;
  } else if (digitalReadings[button2]){
      displayMode++;
  }
  displayMode %= NUM_DISPLAY_MODES;
}


void loop() {
    updateButtons();
    updatePots(false);
    updateBrightness();
    updateFFT();
    if (millis() - previousParameterRead > parameterReadInterval){
      updateDisplayInterval(0);
      updateNumFFT(1);
      updateMaxHue(2);
      updateLEDOffset(3);
      updateSineRate(4);      
      previousParameterRead = millis();
    }

}
