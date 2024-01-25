// #include <Arduino.h>
// #include "BluetoothA2DPSink.h"
// #include "dsps_fir.h"
// // #include <queue>
// #include <Adafruit_NeoPixel.h>
// #include "AudioTools.h"
// #include "AudioLibs/AudioRealFFT.h"

// // std::queue<int16_t> packets;

// #define ELECTROMAG 32
// #define LED 14
// #define NUMPIXELS 50
// #define WINDOW_SIZE 3
// #define BEAT_THRESHOLD 5

// BluetoothA2DPSink a2dp_sink;

// // put function declarations here
// void read_data_stream(const uint8_t *data, uint32_t length);

// Adafruit_NeoPixel pixels(NUMPIXELS, LED, NEO_GRB + NEO_KHZ800);
// AudioRealFFT fft;

// int16_t sample1, sample2, sample3, sample4, sample5;

// void fftResult(AudioFFTBase &fft){
//     float diff;
//     auto result = fft.result();
//     // if (result.magnitude>100){
//     //     // Serial.print(result.frequency);
//     //     // Serial.print(" ");
//     //     // Serial.print(result.magnitude);  
//     //     // Serial.print(" => ");
//     //     // Serial.print(result.frequencyAsNote(diff));
//     //     // Serial.print( " diff: ");
//     //     // Serial.println(diff);
//     // }
// }

// void setup() {
//     // For controlling the electromagnet (MOSFET)
//     pinMode(ELECTROMAG, OUTPUT);
//     pinMode(LED, OUTPUT);
//     pixels.begin();
//     Serial.begin(115200);
//   // put your setup code here, to run once
//     static const i2s_config_t i2s_config = {
//         .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
//         .sample_rate = 44100, // corrected by info from bluetooth
//         .bits_per_sample = (i2s_bits_per_sample_t) 16, /* the DAC module will only take the 8bits from MSB */
//         .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
//         .communication_format = (i2s_comm_format_t)I2S_COMM_FORMAT_STAND_MSB,
//         .intr_alloc_flags = 0, // default interrupt priority
//         .dma_buf_count = 8,
//         .dma_buf_len = 64,
//         .use_apll = false
//     };

//     // setup for FFT library tools

//     AudioLogger::instance().begin(Serial, AudioLogger::Info);

//     // Setup FFT
//     auto tcfg = fft.defaultConfig();
//     tcfg.length = 4096;
//     tcfg.channels = 2;
//     tcfg.sample_rate = a2dp_sink.sample_rate();;
//     tcfg.bits_per_sample = 16;
//     tcfg.callback = &fftResult;
//     fft.begin(tcfg);


//     a2dp_sink.set_i2s_config(i2s_config);
//     a2dp_sink.start("JiggleJiggleBox");
//     Serial.println("Started!");
//     a2dp_sink.set_stream_reader(read_data_stream);
//     a2dp_sink.set_auto_reconnect(false);
// }

// /*
//   define a library for Window
// */

// // struct DataWindow {
// //   std::queue<int16_t> windowQueue;
// //   int16_t currSize;
// //   int16_t currAvg; 
// // };

// // typedef DataWindow* Window;

// // Window newWindow() {
// //   Window myWindow = (Window) (calloc(1, sizeof(DataWindow)));
// //   return myWindow;
// // }

// // Window window = newWindow();

// // bool runWindow (int16_t currIntensity) {
// //   bool isBeat = (currIntensity >= (window->currAvg) + BEAT_THRESHOLD);
// //   if (window->currSize < WINDOW_SIZE) {
// //     (window->windowQueue).push(currIntensity);
// //     window->currSize++;
// //     window->currAvg = ((window->currAvg) / (window->currSize)) + (currIntensity / (window->currSize)); 
// //   }
// //   else {
// //     int16_t removed = (window->windowQueue).front();
// //     (window->windowQueue).push(currIntensity);
// //     window->currAvg = ((window->currAvg) / (window->currSize)) + (currIntensity / (window->currSize)) - (removed / (window->currSize));
// //   }
// //   return isBeat;
// // }

// /*
//   define high as DIGITAL HIGH + ASSOCIATED DELAY
// */

// void perform_HIGH() {
//   digitalWrite(ELECTROMAG, HIGH);
//   Serial.println("HIGH");
//   delay(500);
// }

// /*
//   define high as DIGITAL LOW + ASSOCIATED DELAY
// */

// void perform_LOW() {
//   digitalWrite(ELECTROMAG, LOW);
//   Serial.println("LOW");
//   delay(100);
// }

// /*
//   a data packet constitutes a beat iff it is greater than the average intensity of the window by a 
//   particular threshold
// */
// bool isBeat () {
//   return (sample5 >= ((sample1 + sample2 + sample3 + sample4 + sample5) / 5) + BEAT_THRESHOLD);
// }

// /*
//   if queue is not long enough, we never perform a high
//   if it is, we perform a high when a beat is detected
// */


// void loop() {
//   // put your main code here, to run repeatedly
//   // test code for led
//   // pixels.setPixelColor(2, pixels.Color(0, 255, 90));
//   // pixels.setPixelColor(3, pixels.Color(0, 0, 0));
//   // pixels.setPixelColor(6, pixels.Color(0, 90, 255));
//   // pixels.setPixelColor(8, pixels.Color(255, 255, 255));
//   // pixels.show();
//   // lightUpLED();
//   // if (packets.empty()) {
//   //   perform_LOW();
//   // }
//   // else {
//   //   if (isBeat()) {
//   //     perform_HIGH();
//   //   }
//   //   else {
//   //     perform_LOW();
//   //   }
//   // }
//   if (isBeat()) {
//     perform_HIGH();
//   }
//   else {
//     perform_LOW();
//   }
// }


// /*
//   we unconditionally store each received packet in the global queue, storing it as the average of 
//   left and right stereo
// */

// // put function definitions here
// void read_data_stream(const uint8_t *data, uint32_t length) {
//   int16_t *samples = (int16_t*) data;
//   int16_t thing = *samples;
//   int16_t left = (thing >> 8) & (0xFF);
//   int16_t right = thing & (0xFF);
//   // if (packets.size() < 5) {
//   //   packets.push((int16_t)((left + right) / 2));
//   // }
//   // else {
//   //   packets.front();
//   //   packets.push((int16_t)((left + right) / 2));
//   // }
//   sample1 = sample2;
//   sample2 = sample3;
//   sample3 = sample4;
//   sample4 = sample5;
//   sample5 = (left + right) / 2;
//   uint32_t sample_count = length / 2;
//   //Serial.printf("L: %d R: %d\n", left, right);
//   fft.write(data, length);
// }

// // create a global data structure for buffer
// // whenever a packet is received, add to buffer
// // in loop, if the queue is not large enough, perpetual low. Else, compute whether the signal is 
// // a beat and set to high