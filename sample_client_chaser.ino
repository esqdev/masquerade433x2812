#include <Adafruit_NeoPixel.h>
#include <RH_ASK.h>
#include <SPI.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define LED1PIN 5
#define buttonPin  3
#define maxProgram  7
#define NumLED1 15

int program = 7;
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int chaserID = 1;

//Receiver vars
RH_ASK driver(2000, 11);

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NumLED1, LED1PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  Serial.begin(9600);
  randomSeed(analogRead(0));

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  pinMode(buttonPin, INPUT_PULLUP);

  //randomSeed();

  // Init the Receiver
  if (!driver.init())
    Serial.println("init failed");

}

void loop() {

  buttonState = digitalRead(buttonPin);
  /*
    // compare the buttonState to its previous state
    if (buttonState != lastButtonState) {
      // if the state has changed, increment the counter
      if (buttonState == HIGH) {
        // if the current state is HIGH then the button went from off to on:
        if(program==maxProgram){
          program = 0;
        } else {
          program++;
        }

      }
    }
    // save the current state as the last state, for next time through the loop
    lastButtonState = buttonState;
  */


  // Read the 433
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);
  if (driver.recv(buf, &buflen) || buttonState == LOW) // Non-blocking
  {

    if (buttonState == LOW || (buflen == 2 && ( (int) buf[0] == chaserID || (int) buf[0] == 42 )))

    {
      if (buttonState == LOW) {
        program = (int) random(maxProgram + 1);
      } else
      {
        program = (int) buf[1];
      }
    }





    if (program == 0) {
      //FC running
      colorWipe(strip.Color(255, 0, 0), 200); // Red
      colorWipe(strip.Color(255, 240, 50), 200); // white
    }

    if (program == 1) {
      //Blauer, langsamer, hin und her wandernder Punkt
      colorAlternate(strip.Color(0, 0, 255), 200); //blue
    }

    if (program == 2) {
      //K.I.T.T.
      colorAlternate(strip.Color(255, 0, 0), 50); //blue
    }

    if (program == 3) {
      // schwacher Blauer Alternator 1px
      pinChaser(strip.Color(0, 0, 10), 100); //blue
    }

    if (program == 4) {
      // RGB Chaser
      chaser3Wblackout(strip.Color(255, 0, 0), strip.Color(0, 255, 0), strip.Color(0, 0, 255), 900, 900, 900, 900, 900, 900);
    }

    if (program == 5) {
      //Starkes Blaulicht
      chaser3Wblackout(strip.Color(0, 0, 255), strip.Color(0, 0, 255), strip.Color(0, 0, 255), 50, 50, 50, 50, 50, 500);
    }

    if (program == 6) {
      // Baustellen Warnlicht
      chaser3Wblackout(strip.Color(255, 170, 0), strip.Color(255, 170, 0), strip.Color(255, 170, 0), 50, 50, 50, 50, 50, 500);
    }

    if (program == 7) {
      //Zufall
      randomPixel(500);
    }


    // Send a theater pixel chase in...
    //theaterChase(strip.Color(127, 127, 127), 50); // White
    //theaterChase(strip.Color(127, 0, 0), 50); // Red
    //theaterChase(strip.Color(0, 0, 127), 50); // Blue

    //rainbow(20);
    //rainbowCycle(20);
    //theaterChaseRainbow(50);
  }
}


//fillRandomPixel
void randomPixel(uint16_t wait) {
  int arrayR[10] = {1, 0, 0, 1, 1, 0, 1, 1, 0, 1};
  int arrayG[10] = {0, 1, 0, 1, 0, 1, 1, 0, 1, 1};
  int arrayB[10] = {0, 0, 1, 0, 1, 1, 0, 1, 1, 1};
  int arayI = random(0, 9);
  uint16_t colorR = random(0, 255) * arrayR[arayI];
  uint16_t colorG = random(0, 255) * arrayG[arayI];
  uint16_t colorB = random(0, 255) * arrayB[arayI];
  uint16_t i = random(0, strip.numPixels());

  strip.setPixelColor(i, strip.Color(colorR, colorG, colorB));
  strip.show();
  delay(wait);
}

void all_off() {
    for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}


//rgb chaser with blackout
void chaser3Wblackout(uint32_t c1, uint32_t c2, uint32_t c3, uint16_t w1, uint16_t w2, uint16_t w3, uint16_t w4, uint16_t w5, uint16_t w6) {
  uint16_t i;
  //color 1 ################
  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c1);
  }
  strip.show();
  delay(w1);
  //black
  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  delay(w2);
  //color 2 ################
  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c2);
  }
  strip.show();
  delay(w3);
  //black
  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  delay(w4);
  //color 3 ################
  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c3);
  }
  strip.show();
  delay(w5);
  //black
  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  delay(w6);
}


//singleLEDchasing
void pinChaser(uint32_t c, uint16_t wait) {
  uint16_t i = 0;
  strip.setPixelColor(i, c);
  strip.show();
  while (i < strip.numPixels()) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    i++;
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }

}

// alternating
void colorAlternate(uint32_t c, uint16_t wait) {
  uint16_t i = 0;
  strip.setPixelColor(i, c);
  strip.show();
  while (i < strip.numPixels() - 1) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    i++;
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
  while (i > 0) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    i--;
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }

}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint16_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint16_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint16_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint16_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint16_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
