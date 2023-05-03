/*************************************************************
  MOOG MODEL 15 MIDI CONTROLLER

  David Cool
  https://davidcool.com
  https://sleeplesswaves.bandcamp.com
  v0 Feb 2021

  Midi controller software for the Moog Model 15 app adapted to the MIDIUSB library for Arduino Leonardo / Pro Micro

  Original source code base:
  by Notes and Volts
  www.notesandvolts.com

*************************************************************/

#include "MIDIUSB.h"
#include "Controller.h"

//************************************************************
//***SET THE NUMBER OF CONTROLS USED**************************
//************************************************************
//---How many buttons are connected directly to pins?---------
byte NUMBER_BUTTONS = 0;
//---How many potentiometers are connected directly to pins?--
byte NUMBER_POTS = 0;
//---How many buttons are connected to a multiplexer?---------
byte NUMBER_MUX_BUTTONS = 48;
//---How many potentiometers are connected to a multiplexer?--
byte NUMBER_MUX_POTS = 48;
//************************************************************

//***ANY MULTIPLEXERS? (74HC4067)************************************
//MUX address pins must be connected to Arduino UNO pins 2,3,4,5
//A0 = PIN2, A1 = PIN3, A2 = PIN4, A3 = PIN5
//*******************************************************************
//Mux NAME (OUTPUT PIN, , How Many Mux Pins?(8 or 16) , Is It Analog?);

// digital mux
Mux M2(7, 16, false); //Digital multiplexer on Arduino Pro Micro pin 7
Mux M4(6, 16, false); //Digital multiplexer on Arduino Pro Micro pin 6
Mux M5(5, 16, false); //Digital multiplexer on Arduino Pro Micro pin 5

// analog mux
Mux M1(A0, 16, true); //Analog multiplexer on Arduino analog pin A0
Mux M3(A1, 16, true); //Analog multiplexer on Arduino analog pin A1
Mux M6(A2, 16, true); //Analog multiplexer on Arduino analog pin A2

//*******************************************************************


//***DEFINE DIRECTLY CONNECTED POTENTIOMETERS************************
//Pot (Pin Number, Command, CC Control, Channel Number)
//**Command parameter is for future use**

//Pot P01(A0, 0, 55, 1); //Potentiometer 1

//*******************************************************************
//Add pots used to array below like this->  Pot *POTS[] {&PO1, &PO2, &PO3, &PO4, &PO5, &PO6};
Pot *POTS[] {};
//*******************************************************************


//***DEFINE DIRECTLY CONNECTED BUTTONS*******************************
//Button (Pin Number, Command, Note Number, Channel, Debounce Time)
//** Command parameter 0=NOTE  1=CC  2=Toggle CC **

//Button BU1(9, 1, 116, 1, 5 );  //Button 4

//*******************************************************************
//Add buttons used to array below like this->  Button *BUTTONS[] {&BU1, &BU2, &BU3, &BU4, &BU5, &BU6, &BU7, &BU8};
Button *BUTTONS[] {};
//*******************************************************************


//***DEFINE BUTTONS CONNECTED TO MULTIPLEXER*************************
//Button::Button(Mux mux, byte muxpin, byte command, byte value, byte channel, byte debounce)
//** Command parameter 0=NOTE  1=CC  2=Toggle CC 3= custom 3 & 6 position switches 4= custom 3 position toggle 5=invert vertical toggle **

//digital muxes M2, M4, M5

Button MBU01(M2, 0, 3, 60, 1, 5); //3 position rotary switch 1 -1st position
Button MBU02(M2, 1, 3, 60, 1, 5); //3 position rotary switch 1 -2nd position
Button MBU03(M2, 2, 3, 60, 1, 5); //3 position rotary switch 1 -3rd position

Button MBU04(M2, 3, 1, 61, 1, 5); //Rocker Switch 1
Button MBU05(M2, 4, 1, 63, 1, 5); //Rocker Switch 2
Button MBU06(M2, 5, 1, 66, 1, 5); //Rocker Switch 3

Button MBU07(M2, 6, 3, 69, 1, 5); //6 position rotary switch 1 -1st position
Button MBU08(M2, 7, 3, 69, 1, 5); //6 position rotary switch 1 -2nd position
Button MBU09(M2, 8, 3, 69, 1, 5); //6 position rotary switch 1 -3rd position
Button MBU10(M2, 9, 3, 69, 1, 5); //6 position rotary switch 1 -4th position
Button MBU11(M2, 10, 3, 69, 1, 5); //6 position rotary switch 1 -5th position
Button MBU12(M2, 11, 3, 69, 1, 5); //6 position rotary switch 1 -6th position

Button MBU13(M2, 12, 4, 70, 1, 5); //3 position toggle switch 1 -1st position
Button MBU14(M2, 13, 4, 70, 1, 5); //3 position toggle switch 1 -2nd position

Button MBU15(M2, 14, 4, 72, 1, 5); //3 position toggle switch 2 -1st position
Button MBU16(M2, 15, 4, 72, 1, 5); //3 position toggle switch 2 -2nd position

Button MBU17(M4, 0, 3, 73, 1, 5); //6 position rotary switch 2 -1st position
Button MBU18(M4, 1, 3, 73, 1, 5); //6 position rotary switch 2 -2nd position
Button MBU19(M4, 2, 3, 73, 1, 5); //6 position rotary switch 2 -3rd position
Button MBU20(M4, 3, 3, 73, 1, 5); //6 position rotary switch 2 -4th position
Button MBU21(M4, 4, 3, 73, 1, 5); //6 position rotary switch 2 -5th position
Button MBU22(M4, 5, 3, 73, 1, 5); //6 position rotary switch 2 -6th position

Button MBU23(M4, 6, 1, 76, 1, 5); //2 position toggle switch 1
Button MBU24(M4, 7, 1, 77, 1, 5); //2 position toggle switch 2

Button MBU25(M4, 8, 3, 78, 1, 5); //6 position rotary switch 3 -1st position
Button MBU26(M4, 9, 3, 78, 1, 5); //6 position rotary switch 3 -2nd position
Button MBU27(M4, 10, 3, 78, 1, 5); //6 position rotary switch 3 -3rd position
Button MBU28(M4, 11, 3, 78, 1, 5); //6 position rotary switch 3 -4th position
Button MBU29(M4, 12, 3, 78, 1, 5); //6 position rotary switch 3 -5th position
Button MBU30(M4, 13, 3, 78, 1, 5); //6 position rotary switch 3 -6th position

Button MBU31(M4, 14, 3, 79, 1, 5); //6 position rotary switch 4 -1st position
Button MBU32(M4, 15, 3, 79, 1, 5); //6 position rotary switch 4 -2nd position
Button MBU33(M5, 0, 3, 79, 1, 5); //6 position rotary switch 4 -3rd position
Button MBU34(M5, 1, 3, 79, 1, 5); //6 position rotary switch 4 -4th position
Button MBU35(M5, 2, 3, 79, 1, 5); //6 position rotary switch 4 -5th position
Button MBU36(M5, 3, 3, 79, 1, 5); //6 position rotary switch 4 -6th position

Button MBU37(M5, 4, 1, 99, 1, 5); //Rocker Switch 4
Button MBU38(M5, 5, 5, 100, 1, 5); //Rocker Switch 5
Button MBU39(M5, 6, 5, 101, 1, 5); //Rocker Switch 6
Button MBU40(M5, 7, 1, 102, 1, 5); //Rocker Switch 7
Button MBU41(M5, 8, 1, 103, 1, 5); //Rocker Switch 8
Button MBU42(M5, 9, 1, 104, 1, 5); //Rocker Switch 9
Button MBU43(M5, 10, 1, 108, 1, 5); //Rocker Switch 10

Button MBU44(M5, 11, 1, 113, 1, 5); //Button 1
Button MBU45(M5, 12, 1, 114, 1, 5); //Button 2
Button MBU46(M5, 13, 1, 115, 1, 5); //Button 3
Button MBU47(M5, 14, 1, 116, 1, 5); //Button 4
Button MBU48(M5, 15, 1, 117, 1, 5); //Button 5

//*******************************************************************
////Add multiplexed buttons used to array below like this->  Button *MUXBUTTONS[] {&MBU1, &MBU2, &MBU3, &MBU4, &MBU5, &MBU6.....};
Button *MUXBUTTONS[] {&MBU01, &MBU02, &MBU03, &MBU04, &MBU05, &MBU06, &MBU07, &MBU08, &MBU09, &MBU10, &MBU11, &MBU12, &MBU13, &MBU14, &MBU15, &MBU16, &MBU17, &MBU18, &MBU19, &MBU20, &MBU21, &MBU22, &MBU23, &MBU24, &MBU25, &MBU26, &MBU27, &MBU28, &MBU29, &MBU30, &MBU31, &MBU32, &MBU33, &MBU34, &MBU35, &MBU36, &MBU37, &MBU38, &MBU39, &MBU40, &MBU41, &MBU42, &MBU43, &MBU44, &MBU45, &MBU46, &MBU47, &MBU48};
 
//*******************************************************************


//***DEFINE POTENTIOMETERS CONNECTED TO MULTIPLEXER*******************
//Pot::Pot(Mux mux, byte muxpin, byte command, byte control, byte channel)
//**Command parameter is for future use**

//analog muxes M1, M3, M6

Pot MPO01(M1, 0, 0, 45, 1); //Potentiometer 01
Pot MPO02(M1, 1, 0, 46, 1); //Potentiometer 02
Pot MPO03(M1, 2, 0, 47, 1); //Potentiometer 03
Pot MPO04(M1, 3, 0, 48, 1); //Potentiometer 04
Pot MPO05(M1, 4, 0, 49, 1); //Potentiometer 05
Pot MPO06(M1, 5, 0, 50, 1); //Potentiometer 06
Pot MPO07(M1, 6, 0, 51, 1); //Potentiometer 07
Pot MPO08(M1, 7, 0, 52, 1); //Potentiometer 08
Pot MPO09(M1, 8, 0, 53, 1); //Potentiometer 09
Pot MPO10(M1, 9, 0, 54, 1); //Potentiometer 10
Pot MPO11(M1, 10, 0, 55, 1); //Potentiometer 11
Pot MPO12(M1, 11, 0, 56, 1); //Potentiometer 12
Pot MPO13(M1, 12, 0, 57, 1); //Potentiometer 13
Pot MPO14(M1, 13, 0, 58, 1); //Potentiometer 14
Pot MPO15(M1, 14, 0, 59, 1); //Potentiometer 15
Pot MPO16(M1, 15, 0, 62, 1); //Potentiometer 16
Pot MPO17(M3, 0, 0, 64, 1); //Potentiometer 17
Pot MPO18(M3, 1, 0, 65, 1); //Potentiometer 18
Pot MPO19(M3, 2, 0, 67, 1); //Potentiometer 19
Pot MPO20(M3, 3, 0, 68, 1); //Potentiometer 20
Pot MPO21(M3, 4, 0, 71, 1); //Potentiometer 21
Pot MPO22(M3, 5, 0, 74, 1); //Potentiometer 22
Pot MPO23(M3, 6, 0, 75, 1); //Potentiometer 23
Pot MPO24(M3, 7, 0, 80, 1); //Potentiometer 24
Pot MPO25(M3, 8, 0, 81, 1); //Potentiometer 25
Pot MPO26(M3, 9, 0, 82, 1); //Potentiometer 26
Pot MPO27(M3, 10, 0, 83, 1); //Potentiometer 27
Pot MPO28(M3, 11, 0, 85, 1); //Potentiometer 28
Pot MPO29(M3, 12, 0, 86, 1); //Potentiometer 29
Pot MPO30(M3, 13, 0, 87, 1); //Potentiometer 30
Pot MPO31(M3, 14, 0, 88, 1); //Potentiometer 31
Pot MPO32(M3, 15, 0, 89, 1); //Potentiometer 32
Pot MPO33(M6, 0, 0, 90, 1); //Potentiometer 33
Pot MPO34(M6, 1, 0, 91, 1); //Potentiometer 34
Pot MPO35(M6, 2, 0, 92, 1); //Potentiometer 35
Pot MPO36(M6, 3, 0, 93, 1); //Potentiometer 36
Pot MPO37(M6, 4, 0, 94, 1); //Potentiometer 37
Pot MPO38(M6, 5, 0, 95, 1); //Potentiometer 38
Pot MPO39(M6, 6, 0, 96, 1); //Potentiometer 39
Pot MPO40(M6, 7, 0, 97, 1); //Potentiometer 40
Pot MPO41(M6, 8, 0, 98, 1); //Potentiometer 41
Pot MPO42(M6, 9, 0, 105, 1); //Potentiometer 42
Pot MPO43(M6, 10, 0, 106, 1); //Potentiometer 43
Pot MPO44(M6, 11, 0, 107, 1); //Potentiometer 44
Pot MPO45(M6, 12, 0, 109, 1); //Potentiometer 45
Pot MPO46(M6, 13, 0, 110, 1); //Potentiometer 46
Pot MPO47(M6, 14, 0, 111, 1); //Potentiometer 47
Pot MPO48(M6, 15, 0, 112, 1); //Potentiometer 48

//*******************************************************************
//Add multiplexed pots used to array below like this->  Pot *MUXPOTS[] {&MPO1, &MPO2, &MPO3, &MPO4, &MPO5, &MPO6.....};
Pot *MUXPOTS[] {&MPO01, &MPO02, &MPO03, &MPO04, &MPO05, &MPO06, &MPO07, &MPO08, &MPO09, &MPO10, &MPO11, &MPO12, &MPO13, &MPO14, &MPO15, &MPO16, &MPO17, &MPO18, &MPO19, &MPO20, &MPO21, &MPO22, &MPO23, &MPO24, &MPO25, &MPO26, &MPO27, &MPO28, &MPO29, &MPO30, &MPO31, &MPO32, &MPO33, &MPO34, &MPO35, &MPO36, &MPO37, &MPO38, &MPO39, &MPO40, &MPO41, &MPO42, &MPO43, &MPO44, &MPO45, &MPO46, &MPO47, &MPO48};

//*******************************************************************

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

void setup() {

}

void loop() {
  if (NUMBER_BUTTONS != 0) updateButtons();
  if (NUMBER_POTS != 0) updatePots();
  if (NUMBER_MUX_BUTTONS != 0) updateMuxButtons();
  if (NUMBER_MUX_POTS != 0) updateMuxPots();
}


//*****************************************************************
void updateButtons() {

  // Cycle through Button array
  for (int i = 0; i < NUMBER_BUTTONS; i = i + 1) {
    byte message = BUTTONS[i]->getValue();

    //  Button is pressed
    if (message == 0) {
      switch (BUTTONS[i]->Bcommand) {
        case 0: //Note
          noteOn(BUTTONS[i]->Bchannel, BUTTONS[i]->Bvalue, 127 );
          break;
        case 1: //CC
          controlChange(BUTTONS[i]->Bchannel, BUTTONS[i]->Bvalue, 127);
          break;
        case 2: //Toggle
          if (BUTTONS[i]->Btoggle == 0) {
            controlChange(BUTTONS[i]->Bchannel, BUTTONS[i]->Bvalue, 127);
            BUTTONS[i]->Btoggle = 1;
          }
          else if (BUTTONS[i]->Btoggle == 1) {
            controlChange(BUTTONS[i]->Bchannel, BUTTONS[i]->Bvalue, 0);
            BUTTONS[i]->Btoggle = 0;
          }
          break;
      }
    }

    //  Button is not pressed
    if (message == 1) {
      switch (BUTTONS[i]->Bcommand) {
        case 0:
          noteOff(BUTTONS[i]->Bchannel, BUTTONS[i]->Bvalue, 0);
          break;
        case 1:
          controlChange(BUTTONS[i]->Bchannel, BUTTONS[i]->Bvalue, 0);
          break;
      }
    }
  }
}
//*******************************************************************
void updateMuxButtons() {

  // Cycle through Mux Button array
  for (int i = 0; i < NUMBER_MUX_BUTTONS; i = i + 1) {

    MUXBUTTONS[i]->muxUpdate();
    byte message = MUXBUTTONS[i]->getValue();

    //  Button is pressed
    if (message == 0) {
      switch (MUXBUTTONS[i]->Bcommand) {
        case 0: //Note
          noteOn(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 127);
          break;
        case 1: //CC
          controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 127);
          break;
        case 2: //Toggle
          if (MUXBUTTONS[i]->Btoggle == 0) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 127);
            MUXBUTTONS[i]->Btoggle = 1;
          }
          else if (MUXBUTTONS[i]->Btoggle == 1) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 0);
            MUXBUTTONS[i]->Btoggle = 0;
          }
          break;
        case 3: //custom 3 & 6 position switches (one pin for each position sending different values over same MIDI CC number)

          //3 position rotary switch 1
          if (MUXBUTTONS[i]->Bvalue == 60 && MUXBUTTONS[i]->Bmuxpin == 0) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 0);
          }
          if (MUXBUTTONS[i]->Bvalue == 60 && MUXBUTTONS[i]->Bmuxpin == 1) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 64);
          }
          if (MUXBUTTONS[i]->Bvalue == 60 && MUXBUTTONS[i]->Bmuxpin == 2) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 127);
          }

          //6 position rotary switch 1
          if (MUXBUTTONS[i]->Bvalue == 69 && MUXBUTTONS[i]->Bmuxpin == 6) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 0);
          }
          if (MUXBUTTONS[i]->Bvalue == 69 && MUXBUTTONS[i]->Bmuxpin == 7) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 21);
          }
          if (MUXBUTTONS[i]->Bvalue == 69 && MUXBUTTONS[i]->Bmuxpin == 8) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 42);
          }
          if (MUXBUTTONS[i]->Bvalue == 69 && MUXBUTTONS[i]->Bmuxpin == 9) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 84);
          }
          if (MUXBUTTONS[i]->Bvalue == 69 && MUXBUTTONS[i]->Bmuxpin == 10) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 105);
          }
          if (MUXBUTTONS[i]->Bvalue == 69 && MUXBUTTONS[i]->Bmuxpin == 11) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 127);
          }

          //6 position rotary switch 2
          if (MUXBUTTONS[i]->Bvalue == 73 && MUXBUTTONS[i]->Bmuxpin == 0) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 0);
          }
          if (MUXBUTTONS[i]->Bvalue == 73 && MUXBUTTONS[i]->Bmuxpin == 1) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 21);
          }
          if (MUXBUTTONS[i]->Bvalue == 73 && MUXBUTTONS[i]->Bmuxpin == 2) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 42);
          }
          if (MUXBUTTONS[i]->Bvalue == 73 && MUXBUTTONS[i]->Bmuxpin == 3) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 84);
          }
          if (MUXBUTTONS[i]->Bvalue == 73 && MUXBUTTONS[i]->Bmuxpin == 4) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 105);
          }
          if (MUXBUTTONS[i]->Bvalue == 73 && MUXBUTTONS[i]->Bmuxpin == 5) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 127);
          }

          //6 position rotary switch 3
          if (MUXBUTTONS[i]->Bvalue == 78 && MUXBUTTONS[i]->Bmuxpin == 8) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 0);
          }
          if (MUXBUTTONS[i]->Bvalue == 78 && MUXBUTTONS[i]->Bmuxpin == 9) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 21);
          }
          if (MUXBUTTONS[i]->Bvalue == 78 && MUXBUTTONS[i]->Bmuxpin == 10) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 42);
          }
          if (MUXBUTTONS[i]->Bvalue == 78 && MUXBUTTONS[i]->Bmuxpin == 11) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 84);
          }
          if (MUXBUTTONS[i]->Bvalue == 78 && MUXBUTTONS[i]->Bmuxpin == 12) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 105);
          }
          if (MUXBUTTONS[i]->Bvalue == 78 && MUXBUTTONS[i]->Bmuxpin == 13) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 127);
          }

          //6 position rotary switch 4
          if (MUXBUTTONS[i]->Bvalue == 79 && MUXBUTTONS[i]->Bmuxpin == 14) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 0);
          }
          if (MUXBUTTONS[i]->Bvalue == 79 && MUXBUTTONS[i]->Bmuxpin == 15) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 21);
          }
          if (MUXBUTTONS[i]->Bvalue == 79 && MUXBUTTONS[i]->Bmuxpin == 0) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 42);
          }
          if (MUXBUTTONS[i]->Bvalue == 79 && MUXBUTTONS[i]->Bmuxpin == 1) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 84);
          }
          if (MUXBUTTONS[i]->Bvalue == 79 && MUXBUTTONS[i]->Bmuxpin == 2) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 105);
          }
          if (MUXBUTTONS[i]->Bvalue == 79 && MUXBUTTONS[i]->Bmuxpin == 3) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 127);
          }
          break;
        case 4: //3 position toggle case
        
          //3 position toggle switch 1
          if (MUXBUTTONS[i]->Bvalue == 70 && MUXBUTTONS[i]->Bmuxpin == 12) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 0);
          }
          if (MUXBUTTONS[i]->Bvalue == 70 && MUXBUTTONS[i]->Bmuxpin == 13) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 127);
          }
          /*
          middle toggle setting in button off area below
          */


          //3 position toggle switch 2
          if (MUXBUTTONS[i]->Bvalue == 72 && MUXBUTTONS[i]->Bmuxpin == 14) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 0);
          }
          if (MUXBUTTONS[i]->Bvalue == 72 && MUXBUTTONS[i]->Bmuxpin == 15) {
            controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 127);
          }
          /*
          middle toggle setting in button off area below
          */
          break;
        case 5: //invert vertical toggle
          controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 0);
          break;
      }
    }
    //  Button is not pressed
    if (message == 1) {
      switch (MUXBUTTONS[i]->Bcommand) {
        case 0:
          noteOff(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 0);
          break;
        case 1:
          controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 0);
          break;
        case 4:
          controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 64);
          break;
        case 5: //invert vertical toggle
          controlChange(MUXBUTTONS[i]->Bchannel, MUXBUTTONS[i]->Bvalue, 127);
          break;
      }
    }
  }
}
//***********************************************************************
void updatePots() {
  for (int i = 0; i < NUMBER_POTS; i = i + 1) {
    byte potmessage = POTS[i]->getValue();
    if (potmessage != 255) controlChange(POTS[i]->Pchannel, POTS[i]->Pcontrol, potmessage);
  }
}
//***********************************************************************
void updateMuxPots() {
  for (int i = 0; i < NUMBER_MUX_POTS; i = i + 1) {
    MUXPOTS[i]->muxUpdate();
    byte potmessage = MUXPOTS[i]->getValue();
    if (potmessage != 255) controlChange(MUXPOTS[i]->Pchannel, MUXPOTS[i]->Pcontrol, potmessage);
  }
}
