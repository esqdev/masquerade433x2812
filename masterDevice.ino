 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "FlashTrigger.h"  //sets variable String html_page=...
 
ESP8266WebServer server(80);   

void setup()
{
 
  Serial.begin(115200);         // Serielle schnittstelle initialisieren
  Serial.println("");           // Lehere Zeile ausgeben
  Serial.println("Starte WLAN-Hotspot \"masquerade433x2812\"");
  WiFi.mode(WIFI_AP);           // access point modus
  WiFi.softAP("masquerade", "433x2812");    // Name des Wi-Fi netzes
  delay(500);                   //Abwarten 0,5s
  Serial.print("IP Adresse ");  //Ausgabe aktueller IP des Servers
  Serial.println(WiFi.softAPIP());
 
  //  Bechandlung der Ereignissen
  server.on("/",              server_input_none);
  server.on("/zufall.html",   server_input_zufall);
  server.on("/rot.html",      server_input_rot);
  server.on("/gruen.html",    server_input_gruen);
  server.on("/gelb.html",     server_input_gelb);
  server.on("/bunt.html",     server_input_bunt);
  server.on("/flash.html",    server_input_flash);
  server.on("/schlange.html", server_input_schlange);
  server.on("/bunt2.html",    server_input_bunt);
  server.on("/flash2.html",   server_input_flash);
  server.on("/schlange2.html",server_input_schlange);
   
  server.begin();               // Starte den Server
  Serial.println("HTTP Server gestartet");
}
 
void loop()
{
  server.handleClient();
}

void server_input_none() 
{ Serial.println("program_id = none");
  server.send(200, "text/html", html_page);}

void server_input_zufall()  
{ Serial.println("program_id = 01");
  server.send(200, "text/html", html_page);}

void server_input_rot()
{ Serial.println("program_id = 02");
  server.send(200, "text/html", html_page);}
  
void server_input_gruen() 
{ Serial.println("program_id = 03");
  server.send(200, "text/html", html_page);}

void server_input_gelb()  
{ Serial.println("program_id = 04");
  server.send(200, "text/html", html_page);}

void server_input_bunt()
{ Serial.println("program_id = 05");
  server.send(200, "text/html", html_page);}
void server_input_flash()  
{ Serial.println("program_id = 06");
  server.send(200, "text/html", html_page);}

void server_input_schlange()
{ Serial.println("program_id = 07");
  server.send(200, "text/html", html_page);}
