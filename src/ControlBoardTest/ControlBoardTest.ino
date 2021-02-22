#include <Bounce2.h>
#define POT_1_PIN 18
#define NUM_POTS 6
#define BUTTON_1_PIN 1
#define NUM_BUTTONS 4
int analogReadings[NUM_POTS];
int digitalReadings[NUM_BUTTONS];
Bounce2::Button buttons[NUM_BUTTONS];
unsigned long previousSensorRead = 0;
unsigned long sensorReadInterval = 100;
//Reads a series of potentiometer values and stores them in analogReadings
void readPots() {
  for (int i = 0; i < NUM_POTS; i++){
    analogReadings[i] = analogRead(POT_1_PIN + i);
  }
}

//Reads a series of button values and stores them in analogReadings
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("Initalizing...");
  analogReadResolution(8);
  //set up buttons
  for (int i = 0; i < NUM_BUTTONS; i++){
    buttons[i].attach(BUTTON_1_PIN + i, INPUT_PULLDOWN);
    buttons[i].interval(5);
    buttons[i].setPressedState(HIGH);
  }
}
void printSensors(){
     readPots();
  
    for (int i = 0; i < NUM_POTS; i++){
      Serial.print("Pot ");
      Serial.print(i);
      Serial.print(": ");
      Serial.print(analogReadings[i]);
      Serial.print('\t'); 
    }
    for ( int i = 0; i < NUM_BUTTONS; i++){
      Serial.print("Button ");
      Serial.print(i);
      Serial.print(": ");
      Serial.print(digitalReadings[i]? '!':'.');
      Serial.print('\t');
    }
    Serial.println();
}

void loop() {
  bool buttonUpdate = false;
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
    printSensors();
    previousSensorRead = millis();
  }
  
  if (millis() - previousSensorRead > sensorReadInterval){
    previousSensorRead = millis();
    printSensors();
    sensorReadInterval = analogReadings[0];
  }
  
}
