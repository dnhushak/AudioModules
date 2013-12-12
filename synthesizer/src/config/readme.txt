*******************************************************************************
***                             Voices Config file                          ***
*******************************************************************************

The first five values are the defaults for the 5 modules. 
Each line contains a set of values in the following order:

DESCRIPTION         TYPE    COMMENT

Attack              int     msec
Decay               int     msec
Sustain             float   0<=value<=1. If value>0, sustain lasts as long as key is pressed
Release             int     msec
WaveType            string  SQUARE, PULSE, TRIANGLE, SAWTOOTH, or NOISE
Vibrato Amplitutde  float   Maximum amplitude of the vibrato (percentage of the frequency)
Vibrato Period      int     
Vibrato Delay       int     msec
