#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <EEPROM.h>
int address = 0xFF;

#include <DNSServer.h>
IPAddress apIP(192, 168, 4, 1);
const byte DNS_PORT = 53;
DNSServer dnsServer;

const byte flag_set = 0x0F;
const byte flag_unset = 0x11;


void setup() {
  Serial.begin(9600);

  Serial.printf("Starting up\n");
  WiFi.mode(WIFI_OFF);
  Serial.printf("Current WiFi Mode %d\n", WiFi.getMode());

  EEPROM.begin(512);
  byte flag =  EEPROM.read(address);
  Serial.printf("Flag is %d\n", flag);


  if (flag != flag_set) {
    Serial.printf("Connecting as Wifi client\n");
    WiFi.begin("blabla", "foobar");

    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Disconnecting...");
      WiFi.disconnect(); //<<----- This does not change the wifi mode
      Serial.printf("Current WiFi Mode %d\n", WiFi.getMode());
      WiFi.enableSTA(false); //<<----- But this does!
      Serial.printf("Current WiFi Mode %d\n", WiFi.getMode());
    }
    else {
      Serial.printf("Current WiFi Mode %d\n", WiFi.getMode());
      Serial.printf("Writing Flag\n");
      EEPROM.write(address, 0x0F);
      EEPROM.commit();
    }
  }
  Serial.printf("Starting AP MyAP\n");
  WiFi.softAP("MyAP");
  Serial.printf("Current WiFi Mode %d\n", WiFi.getMode());
  Serial.printf("Starting DNS Server\n");
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  Serial.printf("Current WiFi Mode %d\n", WiFi.getMode());

  Serial.println("Re-setting flag after 20 secs. Make sure to reset before this time to get directly into AP mode");
  delay(20000);

}

void loop() {
  byte flag =  EEPROM.read(address);
  if (flag != flag_unset) {
    Serial.println("Unsetting flag");
      EEPROM.write(address, flag_unset);
      EEPROM.commit();
  }
}

