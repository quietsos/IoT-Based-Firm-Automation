#include <WiFi.h>


void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_MODE_STA);

  // print MAC address to serial monitor

  Serial.print("MAC Adress: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // put your main code here, to run repeatedly:

}
