#include <ESP8266WiFi.h>
#include <SonosUPnP.h>
#include <MicroXPath_P.h>

#include "WemoSwitch.h"
#include "WemoManager.h"
#include "CallbackFunction.h"

#define SERIAL_DATA_THRESHOLD_MS 500
#define SERIAL_ERROR_TIMEOUT "E: Serial"
#define ETHERNET_ERROR_DHCP "E: DHCP"
#define ETHERNET_ERROR_CONNECT "E: Connect"

// prototypes
boolean connectWifi();

//on/off callbacks
void lightOn();
void lightOff();
void ethConnectError();

WiFiClient client;
SonosUPnP g_sonos = SonosUPnP(client, ethConnectError);
IPAddress g_JoeyIP(192, 168, 1, 250);
const char g_JoeyID[] = "949F3E0C46646";

//------- Replace the following! ------
char ssid[] = "ssid";       // your network SSID (name)
char password[] = "password";  // your network key

WemoManager wemoManager;
WemoSwitch *light = NULL;

void setup()
{
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  wemoManager.begin();
  // Format: Alexa invocation name, local port no, on callback, off callback
  light = new WemoSwitch("Kitchen Speaker", 80, lightOn, lightOff);
  wemoManager.addDevice(*light);
}

void loop()
{
  wemoManager.serverLoop();
}

void lightOn() {
    Serial.print("Speaker turned on ...");
     g_sonos.play(g_JoeyIP);
}

void lightOff() {
    Serial.print("Speaker turned off ...");
    g_sonos.pause(g_JoeyIP);
}

void ethConnectError()
{
  Serial.println(ETHERNET_ERROR_CONNECT);
  Serial.println("Wifi died.");
}
