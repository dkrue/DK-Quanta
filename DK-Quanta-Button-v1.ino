#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define midiChannel 1
#define LightPIN 4
#define ButtonPIN 3
#define NeopixelPIN 17
#define NeopixelSIZE 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NeopixelSIZE, NeopixelPIN, NEO_GRB + NEO_KHZ800);

byte midiPulseCounter = 0;
byte midiBeatCounter = 0;
byte midiBarCounter = 0;
byte midiPhraseCounter = 0;

bool phraseTrackingOn;
bool buttonToggle;
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
  
  Serial.begin(9600);
  pinMode(LightPIN, OUTPUT);
  pinMode(ButtonPIN, INPUT_PULLUP);
  analogWrite(LightPIN, 30); // Initialize center backlight

  usbMIDI.setHandleRealTimeSystem(RealTimeSystem);
}

void loop() {
  usbMIDI.read();
  
  if(digitalRead(ButtonPIN) == LOW) {
    buttonToggle = !buttonToggle;
    
    // Send MIDI CC value for stop/start (20/21)
    usbMIDI.sendControlChange(buttonToggle ? 20 : 21, 127, midiChannel);
    
    colorWipe(strip.Color(255, 128, 0), 30);
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


// Handle MIDI clock pulse
void RealTimeSystem(byte realtimebyte) {
 
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
  // midi start byte appears to only be sent at 1:1:1
    phraseTrackingOn = realtimebyte == START;
  }
  
  if(realtimebyte == STOP) {
    digitalWrite(LightPIN, LOW);
    midiPulseCounter = 0;
    midiBeatCounter = 0;
    midiBarCounter = 0;
    midiPhraseCounter = 0;
    phraseTrackingOn = false;
    buttonToggle = false;
    for(byte i=0; i<NeopixelSIZE; i++) {
        beatBrightness[i] = 0;
        barBrightness[i] = 0;
    }
    
  }

  analogWrite(LightPIN, midiPulseCounter * 10);
  for(byte i=0; i<NeopixelSIZE; i++) {
    // max contrast - best RGB candy: (rotated so pixel 0 is 7)
    strip.setPixelColor((i+7)%16, strip.Color(realtimebyte == STOP ? 255: (midiPhraseCounter % 16 == i && phraseTrackingOn ? 255 : 0), barBrightness[i], beatBrightness[i]));
    // red, yellow, green older style
    //strip.setPixelColor(i, strip.Color(realtimebyte == STOP ? 255: (midiPhraseCounter % 16 == i && phraseTrackingOn ? 255 : beatBrightness[i]), midiPhraseCounter % 16 == i && phraseTrackingOn ? 0 :beatBrightness[i] / 4 + barBrightness[i] / 2 , midiPhraseCounter % 16 == i && phraseTrackingOn ? 2 : 0));
  }
  
  
  
  strip.show(); 
} 
