#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include <arduinoFFT.h>
#include <AsyncTimer.h>
#include "SystemFont5x7.h"
#include "Arial_black_16.h"
#define SAMPLES         64         // Must be a power of 2
#define SAMPLING_FREQ   2000         // Hz, must be 4000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
                                     // Convesation time was 112 microSecs, 8.9 KHZ.
#define AMPLITUDE       20         // Depending on your audio source level, can be used as a 'sensitivity' control.
#define AUDIO_IN_PIN    A5           // Signal in on this pin

#define NUM_BANDS       16            //
#define NOISE           500           // Used as a crude noise filter, values below this are ignored
#define ROW 1
#define COLUMN 1
#define FONT Arial_Black_16
int currCol=0;
DMD led_module(ROW, COLUMN);
unsigned long time_now = 0;
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime;
long period;
arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);
DMD dmd(1,1);

unsigned int sampling_period_us;
//double realComponent[64];
//double imagComponent[64];
int sensorValue;
boolean digitalVal=0;
int bandValues[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int oldBarHeights[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
AsyncTimer t;
//int spectralHeight[] = {0b00000000,0b10000000,0b11000000,
  //                      0b11100000,0b11110000,0b11111000,
    //                    0b11111100,0b11111110,0b11111111};
//int index, c, value;

void scan_module()
{
  dmd.scanDisplayBySPI();
}
void turnOff(){
    for (int i=0; i<32; i++){
    for(int j =15; j>oldBarHeights[i/2]; j--){
      dmd.writePixel( i, j, GRAPHICS_NORMAL, 0 );
      dmd.writePixel( i+1, j, GRAPHICS_NORMAL, 0 );
      //delay(30);
      }
      i++;
    }
  }
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A5,INPUT);  
  t.setInterval(turnOff, 100);
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ));
  Timer1.initialize(5000);
  Timer1.attachInterrupt(scan_module);
  dmd.clearScreen( true );
}



void loop() {
    // Reset bandValues[]
  dmd.drawLine( 0, 0, 31, 0, GRAPHICS_NORMAL );
  for (int i = 0; i<NUM_BANDS; i++){
    bandValues[i] = 0;
  }
    //dmd.clearScreen( true );

   for (int i = 0; i < SAMPLES; i++) {
    newTime = micros();
    vReal[i] = analogRead(A5); // A conversion takes about 112uS on an Arduino Uno chip.
    vImag[i] = 0;
    while ((micros() - newTime) < sampling_period_us) { /* chill lol */ }
  }
  
  FFT.DCRemoval();
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();
  for (int i =0; i<64;i++){
  }
  
   // Analyse FFT results
  for (int i = 1; i < (SAMPLES/2); i++){       // Don't use sample 0 and only first SAMPLES/2 are usable. Each array element represents a frequency bin and its value the amplitude.                 // Add a crude noise filter
      //16 bands
      if (i<=2 )           bandValues[0]  += (int)vReal[i];
      if (i>2   && i<=3  ) bandValues[1]  += (int)vReal[i];
      if (i>3   && i<=4  ) bandValues[2]  += (int)vReal[i];
      if (i>4   && i<=5  ) bandValues[3]  += (int)vReal[i];
      if (i>5   && i<=6  ) bandValues[4]  += (int)vReal[i];
      if (i>6   && i<=7 ) bandValues[5]  += (int)vReal[i];
      if (i>7  && i<=8) bandValues[6]  += (int)vReal[i];
      if (i>8  && i<=9) bandValues[7]  += (int)vReal[i];
      if (i>9  && i<=11 ) bandValues[8]  += (int)vReal[i];
      if (i>11  && i<=13 ) bandValues[9]  += (int)vReal[i];
      if (i>13  && i<=15 ) bandValues[10] += (int)vReal[i];
      if (i>15  && i<=18 ) bandValues[11] += (int)vReal[i];
      if (i>18  && i<=21) bandValues[12] += (int)vReal[i];
      if (i>21 && i<=25) bandValues[13] += (int)vReal[i];
      if (i>25 && i<=30) bandValues[14] += (int)vReal[i];
      if (i>30          ) bandValues[15] += (int)vReal[i];
    }
    int col=0;
    // Process the FFT data into bar heights
    for (int band = 0; band < NUM_BANDS; band++) {
  
      // Scale the bars for the display
      int barHeight = bandValues[band] / AMPLITUDE;
      if ( barHeight > 16) 
        barHeight = 16;
      Serial.println(barHeight);
     // barHeight = (oldBarHeights[band] + barHeight) / 2;

      if(barHeight>0){
      dmd.drawLine( col, 0, col, barHeight, GRAPHICS_NORMAL );
      dmd.drawLine( col+1, 0, col+1, barHeight, GRAPHICS_NORMAL );
      
      oldBarHeights[band] = barHeight;
     }
     col=col+2;
    }
    t.handle();
 }
