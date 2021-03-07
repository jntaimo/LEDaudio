
#include <ESP8266WiFi.h>
#include <espnow.h>
uint8_t mainMACAddress[] = {0x40, 0xF5, 0x20, 0x2D, 0x4E, 0x10};

typedef struct MiniGyro{
  bool left;
  uint16_t acceleration;
  //uint16_t velocity;
} MiniGyro;
uint16_t gyroData[2];
MiniGyro miniGyro;


// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&miniGyro, incomingData, sizeof(miniGyro));

//  Serial.print("Bytes received: ");
//  Serial.println(len);
  if (miniGyro.left){
    gyroData[0] = miniGyro.acceleration;
  } else {
    gyroData[1] = miniGyro.acceleration;
  }
  Serial.print(miniGyro.left? "L :":"R: ");
  Serial.print(" a: ");
  Serial.println(miniGyro.acceleration);
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
