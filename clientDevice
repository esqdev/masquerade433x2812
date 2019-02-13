#include <Adafruit_NeoPixel.h>  //Lichter
#include <RH_ASK.h>             //433MHz Empfänger
#include <SPI.h>                //Kommunikation mit Empfänger
#ifdef __AVR__
#include <avr/power.h>
#endif

// DIP Schalter
#define DIP1 3
#define DIP2 2
#define DIP3 1
#define DIP4 0


//Pushbuttons
#define BUTTON1 19
#define BUTTON2 18
#define BUTTON3 17
//#define BUTTON2 16
//#define BUTTON3 15
#define PIN433 6

int button1_state;
int button2_state;
int button3_state;

//Last PIN
#define LoadPin 8
#define LoadTime 500
#define LoadIntervall 4000

//LED Setup
#define DATA_LED1 10
#define DATA_LED2 9
#define NUM_LEDS 30

Adafruit_NeoPixel led_strip_1 = Adafruit_NeoPixel(NUM_LEDS, DATA_LED1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel led_strip_2 = Adafruit_NeoPixel(NUM_LEDS, DATA_LED2, NEO_GRB + NEO_KHZ800);

//Variablen
int myDeviceId;
int myFavProgram = 12; //8 = Kirmesbeleuchtung
int triggerProgramId;
int brightness = 5;
const int maxProgramId = 12;
const int brightness_indicator_program = 7;
int arrayR[10] = {1, 0, 0, 1, 1, 0, 1, 1, 0, 1};
int arrayG[10] = {0, 1, 0, 1, 0, 1, 1, 0, 1, 1};
int arrayB[10] = {0, 0, 1, 0, 1, 1, 0, 1, 1, 1};
bool claim_pegel_2019[2][5][30] = {{
{0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,1,1,1,0,1,1,1,0,1,1,1},
{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,1,0,1},
{0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,1,0,1,0,1,1,1,0,1,1,1},
{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,1,0,0,0,1,0,0,0,1},
{0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,1,1,1,1,0,1,1,1,0,0,0,1},
},
{
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,1,0,1,1,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,1,0,1,0,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,1,0,0,1,0,1,1,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,1,0,0,0,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,1,0,1,1,1},
}};
RH_ASK driver(2000, PIN433);
uint8_t buf;
uint8_t buflen;
long lastLoad = 0;

void setup() {
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);

  pinMode(DIP1, INPUT_PULLUP);
  pinMode(DIP2, INPUT_PULLUP);
  pinMode(DIP3, INPUT_PULLUP);
  pinMode(DIP4, INPUT_PULLUP);
  //Serial.begin(9600);
  set_myDeviceId();

  led_strip_1.begin();
  led_strip_2.begin();
  driver.init();
}

void loop() {

  //Last generieren um Powerbank auszutricksen
  if (millis() > (lastLoad + LoadIntervall)) {
    if (millis() < (lastLoad + LoadTime + LoadIntervall))
    {
      digitalWrite(LoadPin, HIGH);
    } else {
      digitalWrite(LoadPin, LOW);
      lastLoad = millis();
    }
  } else {
    digitalWrite(LoadPin, LOW);
  }


  triggerProgramId = read433();

  //FAV Programm
  button1_state = digitalRead(BUTTON1);
  if (button1_state == LOW) {
    triggerProgramId = myFavProgram;
  }

  // Zufallsprogramm
  button2_state = digitalRead(BUTTON2);
  if (button2_state == LOW) {
    triggerProgramId = random(1, maxProgramId + 1);
  }

  // Helligkeit ändern
  button3_state = digitalRead(BUTTON3);
  if (button3_state == LOW) {
    change_brightness();
  }



  if (triggerProgramId) {
    if ( 1 == triggerProgramId) {
      show_color(255,   0,   0, 1000); // Rot
    }
    if ( 2 == triggerProgramId) {
      show_color(  0, 255,   0, 1000); // grün
    }
    if ( 3 == triggerProgramId) {
      show_color(  0,   0, 255, 1000); // blau
    }
    if ( 4 == triggerProgramId) {
      show_color(255, 200,   0, 1000); // gelb
    }
    if ( 5 == triggerProgramId) {
      show_color(255,   0, 255, 1000); // lila
    }
    if ( 6 == triggerProgramId) {
      show_color(  0, 255, 255, 1000); // türkis
    }
    if ( 7 == triggerProgramId) {
      show_color(255, 255, 255, 1000); // weiss
    }
    if ( 8 == triggerProgramId) {
      kirmesbeleuchtung(150, 100, 4); // Kirmesbeleuchtung
    }
    if ( 9 == triggerProgramId) { // 3x blitzen
      show_color(255,   255,   255,   50); 
      show_color(  0,     0,     0, 1000); 
      show_color(255,   255,   255,   50); 
      show_color(  0,     0,     0, 1000); 
      show_color(255,   255,   255,   50); 
    }
    if (10 == triggerProgramId) { // Running Rot 5 Pixel
      running_pixels(255,   0,   0, 0,0,0,  100, 5, 2);
    }
    if (11 == triggerProgramId) {// feuerwerk
      running_pixels(255,   255,   255,0,0,0,  25, 3, 1); 
      show_color(255,   0,   0, 50);
      show_color(255, 255,   0, 50);    
      show_color(255, 255, 255, 50);       
      show_color(  0,   0, 255, 50);
    }
    if (12 == triggerProgramId) { // fc chaser
      running_pixels(255,   0,   0, 255,200,150,  100, 30, 5); 
    }
    if (13 == triggerProgramId) { // claim "pegel 2019" row 1
      show_claim(claim_pegel_2019,  0,255,255,  100, 10);
    }
    if (14 == triggerProgramId) { // 
      show_random_sleeves(20,   500); // Random Sleeves 
    }
    if (15 == triggerProgramId) { // 
      show_color(255,   0, 255, 1000); // lila 
    }
    if (16 == triggerProgramId) { // 
      show_color(255,   0, 255, 1000); // lila 
    }
    if (17 == triggerProgramId) { // 
      show_color(255,   0, 255, 1000); // lila 
    }

    all_off(); //switch LEDs off for standby
  } // device IF
} //loop

//################
int read433() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);
  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    if (buflen == 2 && ( (int) buf[0] == myDeviceId || (int) buf[0] == 42)) {
      return ( (int) buf[1]);
    }
  }
  else
    return (0);
}



//#######################################
void kirmesbeleuchtung(uint16_t led_speed, uint16_t iterations, uint16_t space) {

  int offset = space;
  for (int i = 0; i < iterations; i++) {

    for (int j = 0; j < NUM_LEDS; j++) {

      if((j + offset) % space == 0){
      int arayI = random(0, 9);
      uint16_t colorR = adjust_brightness(random(0, 255) * arrayR[arayI]);
      uint16_t colorG = adjust_brightness(random(0, 255) * arrayG[arayI]);
      uint16_t colorB = adjust_brightness(random(0, 255) * arrayB[arayI]);

      led_strip_1.setPixelColor(j, led_strip_1.Color(colorR, colorG, colorB));
      led_strip_2.setPixelColor(j, led_strip_2.Color(colorR, colorG, colorB));
      }
      else{
      led_strip_1.setPixelColor(j, led_strip_1.Color(0, 0, 0));
      led_strip_2.setPixelColor(j, led_strip_2.Color(0, 0, 0));  
      }   
    }
    led_strip_1.show();
    led_strip_2.show();
    delay(led_speed);

    offset -= 1;
    if (offset < 1) {
      offset = space;
    }
  }//for

}//function





//#######################################

//running pixels
void running_pixels(uint16_t color_r1, uint16_t color_g1, uint16_t color_b1,uint16_t color_r2, uint16_t color_g2, uint16_t color_b2, uint16_t duration, uint16_t running_pixels, uint16_t iterations) {
  uint16_t i;
  uint16_t j;
  color_r1 = adjust_brightness(color_r1);
  color_g1 = adjust_brightness(color_g1);
  color_b1 = adjust_brightness(color_b1);
  color_r2 = adjust_brightness(color_r2);
  color_g2 = adjust_brightness(color_g2);
  color_b2 = adjust_brightness(color_b2);

  for(j = 0; j < iterations; j++){
  
  for (i = 0; i < (2 * NUM_LEDS + running_pixels); i++) {

    // led_strip_1 AN
    if (i < NUM_LEDS) {
      led_strip_1.setPixelColor(NUM_LEDS - 1 - i, led_strip_1.Color(color_r1, color_g1, color_b1));
    }

    // led_strip_1 AUS
    if (i >= running_pixels && i < NUM_LEDS + running_pixels) {
      led_strip_1.setPixelColor(NUM_LEDS - 1 - i + running_pixels, led_strip_1.Color(color_r2, color_g2, color_b2));
    }

    // led_strip_2 AN
    if (i >= NUM_LEDS && i < 2 * NUM_LEDS) {
      led_strip_2.setPixelColor(i - NUM_LEDS, led_strip_2.Color(color_r1, color_g1, color_b1));
    }

    // led_strip_2 AUS
    if (i >= running_pixels + NUM_LEDS && i < 2 * NUM_LEDS + running_pixels) {
      led_strip_2.setPixelColor(i - NUM_LEDS - running_pixels, led_strip_2.Color(color_r2, color_g2, color_b2));
    }

    led_strip_1.show();
    led_strip_2.show();
    delay(duration);

  }
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

//show random sleeves
void show_random_sleeves(uint16_t iterations, uint16_t duration) {
  uint16_t i;
  uint16_t k;
  
  for(k = 0; k < iterations; k++){
  int arayI = random(0, 9);
  uint16_t color_r = adjust_brightness(255 * arrayR[arayI]);
  uint16_t color_g = adjust_brightness(255 * arrayG[arayI]);
  uint16_t color_b = adjust_brightness(255 * arrayB[arayI]);
  
  for (i = 0; i < NUM_LEDS; i++) {
    led_strip_1.setPixelColor(i, led_strip_1.Color(color_r, color_g, color_b));
   
  }
    led_strip_1.show();
   delay(duration);
 
   arayI = random(0, 9);
   color_r = adjust_brightness(255 * arrayR[arayI]);
   color_g = adjust_brightness(255 * arrayG[arayI]);
   color_b = adjust_brightness(255 * arrayB[arayI]);
   for (i = 0; i < NUM_LEDS; i++) {
    
    led_strip_2.setPixelColor(i, led_strip_2.Color(color_r, color_g, color_b));
  }

  led_strip_2.show();
  delay(duration);
  }
}

//#######################################

//show claim
void show_claim(bool claim[2][5][30], uint16_t color_r, uint16_t color_g, uint16_t color_b,  uint16_t duration,  uint16_t iterations){
  uint16_t i;
  uint16_t j;
  uint16_t k;
  
  color_r = adjust_brightness(color_r);
  color_g = adjust_brightness(color_g);
  color_b = adjust_brightness(color_b);
  for(k = 0; k < iterations; k++){
    for(j = 0; j < 5; j++){
      for (i = 0; i < NUM_LEDS; i++) {
        led_strip_1.setPixelColor(i, led_strip_1.Color(color_r * claim[0][j][i], color_g * claim[0][j][i], color_b * claim[0][j][i]));
        led_strip_2.setPixelColor(i, led_strip_2.Color(color_r * claim[1][j][i], color_g * claim[1][j][i], color_b * claim[1][j][i]));
      }
      led_strip_1.show();
      led_strip_2.show();
      delay(duration);
      for (i = 0; i < NUM_LEDS; i++) {
        led_strip_1.setPixelColor(i, led_strip_1.Color(0,0,0));
        led_strip_2.setPixelColor(i, led_strip_2.Color(0,0,0));
      }
      led_strip_1.show();
      led_strip_2.show();
      delay(duration);
    }
   delay(duration * 2); 
  }
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
uint16_t adjust_brightness(uint16_t c) {
  c = (c * brightness) / 100;
  return c;
}

//#######################################
void set_myDeviceId() {
  myDeviceId = 0;
  int dipRead = digitalRead(DIP1);
  if (dipRead == LOW) {
    myDeviceId += 1;
  }
  dipRead = digitalRead(DIP2);
  if (dipRead == LOW) {
    myDeviceId += 2;
  }
  dipRead = digitalRead(DIP3);
  if (dipRead == LOW) {
    myDeviceId += 4;
  }
  dipRead = digitalRead(DIP4);
  if (dipRead == LOW) {
    myDeviceId += 8;
  }
  //Serial.print("myDeviceId: ");
  //Serial.println(myDeviceId);
}

//#######################################
void change_brightness() {
  brightness += 30;
  if (brightness > 97) {
    brightness = 5;
  }
  //Serial.print("Brightness increased: ");
  //Serial.println(brightness);
  triggerProgramId = brightness_indicator_program;
}




