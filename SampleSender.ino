// ask_transmitter.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

#define maxProgram  7
#define maxSend 250


// RH_ASK driver;


RH_ASK driver(2000, D4, D3, D2); // ESP8266 or ESP32: do not use pin 11
bool di;
int program = 0;
unsigned long lastSend = 0;
int buttonState = 0;



void setup()
{
    Serial.begin(9600);   // Debugging only
    
    di = driver.init();
    if (!di)
         Serial.println("init failed");
    pinMode(D5, INPUT_PULLUP);
}
void loop()
{
    if (program > maxProgram) program = 0;
    buttonState = digitalRead(D5);
    if ( buttonState == LOW && ((millis() - lastSend) > maxSend))
    {
    lastSend = millis();
    


     uint8_t message[2];
    message[0] =  42;
    message[1] = ( uint8_t ) program;
  
    driver.send((uint8_t*)message, sizeof(message));
    if (driver.waitPacketSent()) {
      Serial.print("Code : ");
      Serial.println(message[1]);

    }
    program++;
    }
   
}
