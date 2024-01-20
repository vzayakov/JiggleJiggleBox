#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "dsps_fir.h"
#include <queue>

std::queue<int16_t> packets;

#define ELECTROMAG 32


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        14 // On Trinket or Gemma, suggest changing this to 1
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 200 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);


void setup() {
    // For controlling the electromagnet (MOSFET)
    pinMode(ELECTROMAG, OUTPUT);
    pinMode(PIN, OUTPUT);

    pixels.begin();
    Serial.begin(115200);
  // put your setup code here, to run once
    Serial.println("Started!");
    
}

//flashes LED and acts as delay for perform_high
void flashLED(float delayValUs){
  for (int i=0; i<40; i++){
       pixels.setPixelColor(i, pixels.Color(0,255,90,35));
      
       pixels.show();   // Send the updated pixel colors to the hardware.
       delayMicroseconds(delayValUs/40);
  }
}

//same as old perform high but without delay
void perform_HIGH() {
  digitalWrite(ELECTROMAG, HIGH);
  Serial.println("HIGH2");
}

/*
  define high as DIGITAL LOW + ASSOCIATED DELAY
*/
//same as perform low but now passes daly value as an argument
void perform_LOW(int delayValUs) {
  digitalWrite(ELECTROMAG, LOW);
  Serial.println("LOW2");
  delayMicroseconds(delayValUs);
}

//sends the sqare wave and LED flash in sync
//input is the period of the square wave in microseconds
//all delays in microseconds now as delay time between each led is low
void sendSqareWaveLED(int period){
  int periodUs=period*1000; //the period in microseconds
  perform_HIGH();
  flashLED(periodUs/2);
  pixels.clear();
  pixels.show();
  //delay(2000);
  
  perform_LOW(periodUs/2);
}


void loop() {
  // put your main code here, to run repeatedly
  //beat frequency of 5hz, which is period of 200ms
  sendSqareWaveLED(200);

}
