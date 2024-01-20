#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "BluetoothA2DPSink.h"
#include "dsps_fir.h"
#include <queue>

#define ELECTROMAG 32


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        14 // On Trinket or Gemma, suggest changing this to 1
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 200 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

BluetoothA2DPSink a2dp_sink;


void setup() {
    // For controlling the electromagnet (MOSFET)
    pinMode(ELECTROMAG, OUTPUT);
    pinMode(PIN, OUTPUT);

    pixels.begin();
    Serial.begin(115200);

// initializes the esp32 bluetooth interface

static const i2s_config_t i2s_config = {
        .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
        .sample_rate = 44100, // corrected by info from bluetooth
        .bits_per_sample = (i2s_bits_per_sample_t) 16, /* the DAC module will only take the 8bits from MSB */
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)I2S_COMM_FORMAT_STAND_MSB,
        .intr_alloc_flags = 0, // default interrupt priority
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false
    };

    a2dp_sink.set_i2s_config(i2s_config);
    a2dp_sink.start("JiggleJiggleBox");
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