 #include "MIDIUSB.h"

#define S1PIN 2
#define S2PIN 3
#define MODEPIN 7

#define MODELED 14
#define S2LED 16
#define S1LED 15

int S1, S1p;
int S2, S2p;

int mode;

int state1 = 0, state2= 0;

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void setup() {
  pinMode(MODEPIN, INPUT_PULLUP);
  pinMode(S1PIN, INPUT_PULLUP);
  pinMode(S2PIN, INPUT_PULLUP);

  pinMode(S1LED, OUTPUT);
  pinMode(S2LED, OUTPUT);  
  pinMode(MODELED, OUTPUT);

  S1 = digitalRead(S1PIN);
  S2 = digitalRead(S2PIN);
  S1p = S1;
  S2p = S2;

  digitalWrite(S1LED, !S1);
  digitalWrite(S2LED, !S2);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void loop() {
  S1 = digitalRead(S1PIN);
  S2 = digitalRead(S2PIN);
  mode = digitalRead(MODEPIN);
  if (S1 != S1p) {
    S1p = S1;
    if (mode) {
      state1 = !state1;
      if (state1) {
        controlChange(0, 0, 127);
      } else {
        controlChange(0, 0, 0);
      }
    } else {
      noteOn(0, 0, 127);
    }
  }
  if (S2 != S2p) {
    S2p = S2;
    if (mode) {
      state2 = !state2;
      if (state2) {
        controlChange(0, 1, 127);
      } else { 
        controlChange(0, 1, 0);
      }
    } else {
      noteOn(0, 1, 127);
    }
  }
  MidiUSB.flush();      
  digitalWrite(S1LED, state1);
  digitalWrite(S2LED, state2);
  digitalWrite(MODELED, !mode);
  delay(10);
}
