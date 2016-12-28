#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#include <SonosUPnP.h>
#include <MicroXPath_P.h>

#include <WemoSwitch.h>
#include <WemoManager.h>
#include <CallbackFunction.h>

#define SERIAL_DATA_THRESHOLD_MS 500
#define SERIAL_ERROR_TIMEOUT "E: Serial"
#define ETHERNET_ERROR_DHCP "E: DHCP"
#define ETHERNET_ERROR_CONNECT "E: Connect"

//pre-define wifi stuff
boolean connectWifi();

//Pre defines
void lightOn();
void lightOff();
void ethConnectError();

//init
WiFiClient client;
WiFiManager wifiManager;
SonosUPnP g_sonos = SonosUPnP(client, ethConnectError);

WemoManager wemoManager;
WemoSwitch *light = NULL;

IPAddress g_JoeyIP(192, 168, 1, 250);
const char g_JoeyID[] = "949F3E0C46646";

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
  wifiManager.autoConnect("ConfigAP");
  Serial.println("Connected to WiFi, mac: "+String(WiFi.macAddress()));
  Serial.println();
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
