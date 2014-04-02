#ifndef OUT_H_
#define OUT_H_

/******************************************************************************/
/* Channel Selection Buttons and LEDs                                         */
/******************************************************************************/
const int redSelectorButton = A1;
const int yelSelectorButton = A0;
const int grnSelectorButton = A3;
const int bluSelectorButton = A2;
const int whtSelectorButton = A5;

const int redSelectorLED = 39;
const int yelSelectorLED = 38;
const int grnSelectorLED = 37;
const int bluSelectorLED = 36;
const int whtSelectorLED = 35;

/******************************************************************************/
/* Channel Edit Buttons and LEDs                                              */
/******************************************************************************/
const int arpeggioButton = A4;
const int arpeggioRedLED = 34;
const int arpeggioGrnLED = 33;
const int arpeggioBluLED = 32;

const int glissButton = A7;
const int glissRedLED = 31;
const int glissGrnLED = 30;
const int glissBluLED = 29;

/******************************************************************************/
/* Songbox Buttons and LEDs                                                                    */
/******************************************************************************/

const int playButton = A9;
const int playLED = 26;
const int pausButton = A8;
const int pausLED = 25;
const int stopButton = A6;
const int stopLED = 27;
const int recdButton = A10;
const int recdLED = 28;

/******************************************************************************/
/* Encoders                                                                   */
/******************************************************************************/
const int encoderAChannelVolumeA = 2;
const int encoderAChannelVolumeB = 3;
const int encoderBArpeggioA = 4;
const int encoderBArpeggioB = 5;
const int encoderCGlissandoA = 6;
const int encoderCGlissandoB = 7;
const int encoderDTempoA = 8;
const int encoderDTempoB = 9;



#endif /* OUT_H_ */
