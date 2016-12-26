#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "StreamCommand.h"

#define CLIENT_PORT 7500

int status = WL_IDLE_STATUS;
const char* ssid = "Wifi with wires";  //  your network SSID (name)
const char* pass = "enelespacio";       // your network password
char turretName[64]; //nombre del esp

WiFiClient client;
StreamCommand sCmdClient(client);

void cmd_name()
{
}

void cmd_posicion()
{
}

void cmd_rotacion()
{
}

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  // setting up Station AP
  WiFi.begin(ssid, pass);

  // Wait for connect to AP
  Serial.print("[Connecting]");
  Serial.print(ssid);
  int tries=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tries++;
    if (tries > 30){
      break;
    }
  }
  Serial.println();

  sCmdClient.addCommand(":NAME", cmd_name);
  sCmdClient

  //client.connect("192.168.1.41", CLIENT_PORT);
  client.connect("192.168.1.15", CLIENT_PORT);

  client.print(":HOLA");
  client.print("\r\n");

}

void loop()
{
    sCmdClient.readStream();
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
