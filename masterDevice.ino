 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "webApp.h"  //sets variable String html_page=...

#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

#define maxProgram  11
#define maxSend 250

IPAddress local_IP(192,168,11,11);
IPAddress gateway(192,168,200,200);
IPAddress subnet(255,255,255,0);
int buttonState;

RH_ASK driver(2000, D4, D3, D2); // ESP8266 or ESP32: do not use pin 11
 
ESP8266WebServer server(80);   

void setup()
{
  driver.init();
  pinMode(D5, INPUT_PULLUP);

  
  Serial.begin(115200);         // Serielle schnittstelle initialisieren
  Serial.println("");           // Lehere Zeile ausgeben
  Serial.println("Starte WLAN-Hotspot \"masquerade433x2812\"");
  WiFi.mode(WIFI_AP);           // access point modus
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP("masquerade", "433x2812");    // Name des Wi-Fi netzes
  delay(500);                   //Abwarten 0,5s
  Serial.print("IP Adresse ");  //Ausgabe aktueller IP des Servers
  Serial.println(WiFi.softAPIP());
 
  //  Bechandlung der Ereignissen
  server.on("/",              server_input_none);
  server.on("/alle_rot.html",    [](){  execute(42, 1);});
  server.on("/alle_gruen.html",  [](){  execute(42, 2);});
  server.on("/alle_blau.html",   [](){  execute(42, 3);});
  server.on("/alle_gelb.html",   [](){  execute(42, 4);});
  server.on("/alle_lila.html",   [](){  execute(42, 5);});
  server.on("/alle_tuerkis.html",[](){  execute(42, 6);}); 
  server.on("/alle_weiss.html",  [](){  execute(42, 7);});
  server.on("/alle_kirmes.html", [](){  execute(42, 8);});
  server.on("/alle_flash.html",  [](){  execute(42, 9);});
  server.on("/alle_rot_lauf.html",   [](){  execute(42, 10);});
  server.on("/alle_feuerwerk.html",  [](){  execute(42, 11);});
  server.on("/alle_fckoeln.html",    [](){  execute(42, 12);});
  server.on("/alle_pegel2019.html",    [](){  execute(42, 13);});
  
  server.on("/alle_randomsleeve.html",    [](){  execute(42, 14);});
  server.on("/alle_paparazzi.html",    [](){  execute(42, 15);});
  server.on("/alle_funkeln.html",    [](){  execute(42, 16);});
  server.on("/alle_dev17.html",    [](){  execute(42, 17);});
  server.on("/alle_dev18.html",    [](){  execute(42, 18);});
  
  server.on("/ping_jan.html", [](){  execute(1, 3);});
  server.on("/ping_jo.html", [](){  execute(2, 3);});
  server.on("/ping_marc.html", [](){  execute(3, 3);});
  server.on("/ping_max.html", [](){  execute(4, 3);});
  server.on("/ping_niklas.html", [](){  execute(5, 3);});
  server.on("/ping_niko.html", [](){  execute(6, 3);});
  server.on("/ping_ulli.html", [](){  execute(7, 3);});
  server.on("/ping_vince.html", [](){  execute(8, 3);});
   
  server.begin();               // Starte den Server
  Serial.println("HTTP Server gestartet");
}
 
void loop()
{
  server.handleClient();

  buttonState = digitalRead(D5);
  if(buttonState == 0) {execute(42, 11);};
}

void execute(int clientID, int programID) {
    uint8_t message[2];
    message[0] =  (uint8_t) clientID;
    message[1] = ( uint8_t ) programID;
  
    driver.send((uint8_t*)message, sizeof(message));
    if (driver.waitPacketSent()) {
      Serial.print("Device : ");
      Serial.print(message[0]);
      Serial.print(" / Code : ");
      Serial.println(message[1]);

    }
server.send(200, "text/html", forwarding_page);
}

void server_input_none() 
{ Serial.println("program_id = none");
  server.send(200, "text/html", html_page);}
