#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "MIDIUSB.h" // New MIDI library for 32U4 chips

//#define DEBUG

#define midiChannel 1
#define LightPIN 5 // Pin with PWM
#define ButtonPIN 7
#define NeopixelPIN 10
#define NeopixelSIZE 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NeopixelSIZE, NeopixelPIN, NEO_GRB + NEO_KHZ800);

byte midiPulseCounter = 0;
byte midiBeatCounter = 0;
byte midiBarCounter = 0;
byte midiPhraseCounter = 0;

bool phraseTrackingOn;
bool isPlaying = false;
byte beatBrightness[NeopixelSIZE];
byte barBrightness[NeopixelSIZE];

// Byte values for MIDI transport control
static byte CLOCK = 248;
static byte START = 250;
static byte CONTINUE = 251;
static byte STOP = 252;


void setup() {
  strip.begin();
  colorWipe(strip.Color(0, 0, 0), 1);
  strip.show(); // Initialize all pixels to 'off'
  
  pinMode(LightPIN, OUTPUT);
  pinMode(ButtonPIN, INPUT_PULLUP);

  #ifdef DEBUG
    Serial.begin(115200);
    Serial.println("Initialized");
  #endif

 colorWipe(strip.Color(20, 10, 100), 25);
 colorWipe(strip.Color(0, 0, 0), 10);
}

void loop() {

  midiEventPacket_t rx;
  byte realtimebyte; 
  
  do {
    rx = MidiUSB.read();
    realtimebyte = rx.byte1;

    if(realtimebyte == CLOCK) {
      
      midiPulseCounter++;
      if(midiPulseCounter >= 24) {
        midiPulseCounter = 0;
        midiBeatCounter++;
        if(midiBeatCounter % 4 == 0) {
          midiBarCounter++;
          if(midiBarCounter % 16 == 0) {
            midiPhraseCounter++;
          }
          for(byte i=0; i<NeopixelSIZE; i++) {
            if(midiBarCounter % NeopixelSIZE == i) {
              barBrightness[i] = 64;
            } else {
              barBrightness[i] /= 4;
            }
          }
        }
        for(byte i=0; i<NeopixelSIZE; i++) {
          if(midiBeatCounter % NeopixelSIZE == i) {
            beatBrightness[i] = 16;
          } else {
            beatBrightness[i] /= 2;
          }
        }
      }
      
    }
  
    if(realtimebyte == START || realtimebyte == CONTINUE) { 
      digitalWrite(LightPIN, HIGH);
      midiPulseCounter = 0;
      midiBeatCounter = 0;
      midiBarCounter = 0;
      midiPhraseCounter = 1; 
      isPlaying = true; 
      // midi start byte appears to only be sent at 1:1:1
      phraseTrackingOn = realtimebyte == START;

      #ifdef DEBUG
        Serial.println("START");
      #endif
    }
    
    if(realtimebyte == STOP) {
      digitalWrite(LightPIN, LOW);
      midiPulseCounter = 0;
      midiBeatCounter = 0;
      midiBarCounter = 0;
      midiPhraseCounter = 0;
      phraseTrackingOn = false;
      isPlaying = false;
      for(byte i=0; i<NeopixelSIZE; i++) {
          beatBrightness[i] = 0;
          barBrightness[i] = 0;
      }

      #ifdef DEBUG
        Serial.println("STOP");
      #endif
    } 
  } while (rx.header != 0);

  // Update lights
  analogWrite(LightPIN, midiPulseCounter * 10);
  for(byte i=0; i<NeopixelSIZE; i++) {
    // max contrast - best RGB candy: (rotated so pixel 0 is 7)
    strip.setPixelColor((i+7)%16, strip.Color(realtimebyte == STOP ? 255: (midiPhraseCounter % 16 == i && phraseTrackingOn ? 255 : 0), barBrightness[i], beatBrightness[i]));
    // red, yellow, green older style
    //strip.setPixelColor(i, strip.Color(realtimebyte == STOP ? 255: (midiPhraseCounter % 16 == i && phraseTrackingOn ? 255 : beatBrightness[i]), midiPhraseCounter % 16 == i && phraseTrackingOn ? 0 :beatBrightness[i] / 4 + barBrightness[i] / 2 , midiPhraseCounter % 16 == i && phraseTrackingOn ? 2 : 0));
  }
  
  strip.show(); 

  // Update button press
  if(digitalRead(ButtonPIN) == LOW) {
    #ifdef DEBUG
      Serial.println("BUTTON PRESS");
    #endif
    
    // Send MIDI CC value for stop/start (20/21)
    controlChange(midiChannel, isPlaying ? 20 : 21, 127);

    // Ensure all MIDI is sent over usb before color wipe
    MidiUSB.flush();
    
    colorWipe(strip.Color(255, 128, 5), 30);
    colorWipe(strip.Color(0, 0, 0), 20);
  }
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).
void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
