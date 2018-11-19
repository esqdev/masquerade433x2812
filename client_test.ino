#include <Adafruit_NeoPixel.h>  //Lichter
//#include <RH_ASK.h>             //433MHz Empfänger
#include <SPI.h>                //Kommunikation mit Empfänger

// DIP Schalter
#define DIP1 3
#define DIP2 2
#define DIP3 1
#define DIP4 0


//Pushbuttons
#define BUTTON1 17
#define BUTTON2 18
#define BUTTON3 19
int button1_state;
int button2_state;
int button3_state;

//Last PIN
#define LAST_PIN 8

//LED Setup
#define DATA_LED1 10
#define DATA_LED2 9
#define NUM_LEDS 30

Adafruit_NeoPixel led_strip_1 = Adafruit_NeoPixel(NUM_LEDS, DATA_LED1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel led_strip_2 = Adafruit_NeoPixel(NUM_LEDS, DATA_LED2, NEO_GRB + NEO_KHZ800);

//Variablen
int myDeviceId;
int triggerDeviceId;
int triggerProgramId;
int brightness = 50;
const int maxProgramId = 10;


void setup() {
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(DIP1, INPUT_PULLUP);
  pinMode(DIP2, INPUT_PULLUP);
  pinMode(DIP3, INPUT_PULLUP);
  pinMode(DIP4, INPUT_PULLUP);
  Serial.begin(9600);
  set_myDeviceId();
}

void loop() {
  triggerDeviceId = 0;
  triggerProgramId = 0;

  // Helligkeit ändern
  button1_state = digitalRead(BUTTON1); 
  if(button1_state == LOW){decrease_brightness();}
  button3_state = digitalRead(BUTTON3); 
  if(button3_state == LOW){increase_brightness();}

  // Zufallsprogramm
  button2_state = digitalRead(BUTTON2); 
  if(button2_state == LOW){randomTrigger();}

  //READ 433 Receiver
  //Assign Device-Message to "triggerDeviceId"
  //Assign Program-Message to "triggerProgramId"

  if(myDeviceId == triggerDeviceId || 42 == triggerDeviceId){
    if( 1 == triggerProgramId){    show_color(255,   0,   0, 3000);} // Rot 3 Sek
    if( 2 == triggerProgramId){    show_color(  0, 255,   0, 5000);} // grün 5 Sek
    if( 3 == triggerProgramId){    show_color(  0,   0, 255, 1000);} // blau 1 Sek
    if( 4 == triggerProgramId){    show_color(255, 255,   0, 3000);} // gelb 3 Sek
    if( 5 == triggerProgramId){    show_color(255,   0, 255, 3000);} // lila 3 Sek
    if( 6 == triggerProgramId){    show_color(  0, 255, 255, 3000);} // türkis 3 Sek
    if( 7 == triggerProgramId){    show_color(255, 255, 255, 3000);} // weiss 3 Sek
    if( 8 == triggerProgramId){    show_color(255,   0,   0, 3000);} // Rot 3 Sek
    if( 9 == triggerProgramId){    show_color(255,   0,   0, 3000);} // Rot 3 Sek
    if(10 == triggerProgramId){running_pixels(255,   0,   0,  100, 5);} // Running Rot 5 Pixel

    all_off(); //switch LEDs off for standby 
  } // device IF
} //loop

//#######################################

//running pixels
void running_pixels(uint16_t color_r, uint16_t color_g, uint16_t color_b, uint16_t duration, uint16_t running_pixels) {
  uint16_t i;
  color_r = adjust_brightness(color_r);
  color_g = adjust_brightness(color_g);
  color_b = adjust_brightness(color_b);
  
  for (i = 0; i < (2*NUM_LEDS + running_pixels); i++) {
    
    // led_strip_1 AN
    if(i < NUM_LEDS){ 
      led_strip_1.setPixelColor(NUM_LEDS - 1 - i, led_strip_1.Color(color_r, color_g, color_b));
    }
    
    // led_strip_1 AUS
    if(i >= running_pixels && i < NUM_LEDS + running_pixels){
      led_strip_1.setPixelColor(NUM_LEDS - 1 - i + running_pixels, led_strip_1.Color(0, 0, 0));
    }

    // led_strip_2 AN
    if(i >= NUM_LEDS && i < 2 * NUM_LEDS){ 
      led_strip_2.setPixelColor(i - NUM_LEDS, led_strip_2.Color(color_r, color_g, color_b));
    }
    
    // led_strip_2 AUS
    if(i >= running_pixels + NUM_LEDS && i < 2*NUM_LEDS + running_pixels){
      led_strip_2.setPixelColor(i - NUM_LEDS - running_pixels, led_strip_2.Color(0, 0, 0));
    }
 
    led_strip_1.show();
    led_strip_2.show();
    delay(duration);
  
  }

}

//#######################################

//show 1 colour
void show_color(uint16_t color_r, uint16_t color_g, uint16_t color_b, uint16_t duration) {
  uint16_t i;
  color_r = adjust_brightness(color_r);
  color_g = adjust_brightness(color_g);
  color_b = adjust_brightness(color_b);
  
  for (i = 0; i < NUM_LEDS; i++) {
    led_strip_1.setPixelColor(i, led_strip_1.Color(color_r, color_g, color_b));
    led_strip_2.setPixelColor(i, led_strip_2.Color(color_r, color_g, color_b));
  }
  led_strip_1.show();
  led_strip_2.show();
  delay(duration);
}


//#######################################
void all_off() {
    for (int i = 0; i < NUM_LEDS; i++) {
    led_strip_1.setPixelColor(i, led_strip_1.Color(0, 0, 0));
    led_strip_2.setPixelColor(i, led_strip_2.Color(0, 0, 0));
    }
  led_strip_1.show();
  led_strip_2.show();
}

//#######################################
uint16_t adjust_brightness(uint16_t c){
  c = (c * brightness) / 100;
  return c;
}

//#######################################
void set_myDeviceId(){
  myDeviceId = 0;
  int dipRead = digitalRead(DIP1);
  if(dipRead == LOW){myDeviceId += 1;}
  dipRead = digitalRead(DIP2);
  if(dipRead == LOW){myDeviceId += 2;}
  dipRead = digitalRead(DIP3);
  if(dipRead == LOW){myDeviceId += 4;}
  dipRead = digitalRead(DIP4);
  if(dipRead == LOW){myDeviceId += 8;}
  Serial.print("myDeviceId: ");
  Serial.println(myDeviceId);
}

//#######################################
void increase_brightness(){
  brightness = max( brightness + 5, 100);
  Serial.print("Brightness increased: ");
  Serial.println(brightness);
}

//#######################################
void decrease_brightness(){
  brightness = min( brightness - 5, 10);
  Serial.print("Brightness decreased: ");
  Serial.println(brightness);
}

//#######################################
void randomTrigger(){
  triggerDeviceId = myDeviceId;
  triggerProgramId = random(1,maxProgramId + 1);
  Serial.print("Random Program: ");
  Serial.println(triggerProgramId);
}

