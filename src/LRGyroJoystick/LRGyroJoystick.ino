
#include <ESP8266WiFi.h>
#include <espnow.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#define AVG_ACCEL 950
Adafruit_MPU6050 mpu;
unsigned long previousMillis = 0;
const long sendInterval = 200;
////40:F5:20:2D:4E:10
//uint8_t mainMACAddress[] = {0x40, 0xF5, 0x20, 0x2D, 0x4E, 0x10};
//D8:BF:C0:10:B6:63
uint8_t mainMACAddress[] = {0xD8, 0xBF, 0xC0, 0x10, 0xB6, 0x63};
typedef struct Gyro{
  bool left;
  float ax;
  float ay;
  float az;
  float x;
  float y;
  float z;
} Gyro;

typedef struct MiniGyro{
  bool left;
  uint16_t acceleration;
  //uint16_t velocity;
} MiniGyro;
MiniGyro miniGyro;
Gyro gyro {0, 0, 0, 0, 0, 0, 0};

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  char macStr[18];
  //Serial.print("Packet to:");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
         mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  //Serial.print(macStr);
  //Serial.print(" send status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void setup(void) {
  Serial.begin(115200);
//  while (!Serial) {
//    delay(10); // will pause Zero, Leonardo, etc until serial console opens
//  }
  delay(100);
  Serial.println("Initializing...");
  
  //L FC:F5:C4:AB:BF:11
  //R D8:F1:5B:11:1D:CC
  //COMMENT FOLLOWING LINE IF PROGRAMMING RIGHT JOYSTICK!
  //gyro.left = true;
  miniGyro.left = false;
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(mainMACAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  Serial.println("Completed Initialization");
  delay(100);
}

void loop() {


  if (previousMillis - millis() > sendInterval){
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    if(mpu.getEvent(&a, &g, &temp)){
      gyro.ax = a.acceleration.x;
      gyro.ay = a.acceleration.y;
      gyro.az = a.acceleration.z;
      gyro.x = g.gyro.x;
      gyro.y = g.gyro.y;
      gyro.z = g.gyro.z;
      miniGyro.acceleration = abs(AVG_ACCEL-sqrt(pow(gyro.ax,2) + pow(gyro.ay,2) + pow(gyro.az,2))*100);
      //miniGyro.velocity = sqrt(pow(gyro.x,2) + pow(gyro.y,2) + pow(gyro.z,2))*1000;
      esp_now_send(0, (uint8_t *) &miniGyro, sizeof(miniGyro));
    }


    
      /* Print out the values */
//    Serial.print(gyro.ax);
//    Serial.print(",");
//    Serial.print(gyro.ay);
//    Serial.print(",");
//    Serial.print(gyro.az);
//    Serial.print(", ");
//    Serial.print(gyro.x);
//    Serial.print(",");
//    Serial.print(gyro.y);
//    Serial.print(",");
//    Serial.print(gyro.z);
//    Serial.println("");

    previousMillis = millis();        
  }
}
