
#include <ESP8266WiFi.h>
#include <espnow.h>
uint8_t mainMACAddress[] = {0x40, 0xF5, 0x20, 0x2D, 0x4E, 0x10};
typedef struct Gyro{
  bool left;
  float ax;
  float ay;
  float az;
  float x;
  float y;
  float z;
} Gyro;
//L FC:F5:C4:AB:BF:11
//R D8:F1:5B:11:1D:CC

Gyro gyroL {1, 0, 0, 0, 0, 0, 0};
Gyro gyroR {0, 0, 0, 0, 0, 0, 0};
Gyro dummyGyro; 
typedef struct MiniGyro{
  bool left;
  uint16_t acceleration;
  //uint16_t velocity;
} MiniGyro;
MiniGyro miniGyro;
void printGyro(Gyro &gyro){
  Serial.print(gyro.left? "L: ":"R: ");
  Serial.print("ax: ");
  Serial.print(gyro.ax);
  Serial.print(" ay: ");
  Serial.print(gyro.ay);
  Serial.print(" az: ");
  Serial.print(gyro.az);
  Serial.print(" vx: ");
  Serial.print(gyro.x);
  Serial.print(" vy: ");
  Serial.print(gyro.y);
  Serial.print(" vz: ");
  Serial.println(gyro.z);
}
// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&miniGyro, incomingData, sizeof(miniGyro));
//  if (dummyGyro.left){
//    gyroL = dummyGyro;
//  } else {
//    gyroR = dummyGyro;
//  }
////  Serial.print("Bytes received: ");
////  Serial.println(len);
  Serial.print(miniGyro.left? "L :":"R: ");
  Serial.print(" a: ");
  Serial.println(miniGyro.acceleration);
  //Serial.print(" v: ");
  //Serial.println(miniGyro.velocity);
  
  //printGyro(dummyGyro);
  //Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() != 0){
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
