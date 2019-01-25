// ************************** НАСТРОЙКИ ***********************
// ************************** SETTINGS ***********************
#define BUTTON_PIN   2    //pyc./ пин кнопки                     / eng./button pin
#define PIXEL_PIN    0    //pyc./ пин ленты                      / eng./tape pin
#define PIXEL_COUNT  8    //pyc./ количество светодиодов         / eng./number of LEDs
#define MAXFUN       2    //pyc./ количество эфектов             / eng./number of effects
// ************************** ДЛЯ РАЗРАБОТЧИКОВ ***********************
// ************************** FOR DEVELOPERS **************************
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

#ifdef __AVR__
  #include <avr/power.h>
#endif
int fun = 0;
boolean buttonPressed() {
  boolean pressed = !digitalRead(BUTTON_PIN);
  if (pressed) {
    delay(10);
    pressed = !digitalRead(BUTTON_PIN);
    if (pressed) {
      fun++;
      if (fun>MAXFUN) fun = 0;
      while (!digitalRead(BUTTON_PIN));
    }  
  }
  return (pressed);
}

void setup() {
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  strip.begin();
  strip.show(); 
  pinMode(BUTTON_PIN, INPUT); 
      
}

void loop() {
  switch (fun) {
  case 0:
      rainbow(20);
      break;
  case 1:
      rainbowCycle(20);
      break;
  case 2:
      black();
      break;
 }
}
//********************************************************************


//********************************************************************
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
//********************************************************************
void black(void) {
  colorWipe(strip.Color(0, 0, 0),0);
  strip.show();
  while (!buttonPressed());
}
//********************************************************************
void rainbow(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
  if (buttonPressed()) return;
}


//********************************************************************
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256*5; j++) { 
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
   delay(wait);
  }
  if (buttonPressed()) return;
}

//
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
