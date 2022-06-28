# AudioVisualizerProj
Data Acquisition project to display audio frequencies using an arduino and an LED display.


This is Arduino project source code to display audio frequencies on an LED P10 Matrix Display.
Audio input is recorded using a microphone module.

This project requires the following libraries to be included in your project:

#include <DMD.h>
** dmd library dedicated for P10 Display and used to display our data **
** Any Display can be used for this project and use their corresponding library **

#include <TimerOne.h>

#include <arduinoFFT.h>
** Fast Fourier Transform library used to analyse the input wave and seperate each frequency recorded from a singular wave input **
** More details and explanation about the FFT library can be found online **

#include <AsyncTimer.h>
** This library I used to multitask between the recording and analyzing of data and displaying them simultaneously **


# AudioVisualerProj/ConvTimeCalc
This code is used to calculate the analog to digital conversion time of your chip (in my case it's Arduino UNO).
Ideally you need to record 40kHz for all sound frequencies.
In my project, I used 2kHz sample conversion rate and recorded 64 samples.
But these numbers can vary with different equipement.
