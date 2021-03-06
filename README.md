# DK-Quanta
_QUANTA_ is a one-button MIDI controller that displays the beats and bars along the project timeline in many popular music production programs such as Cubase, Logic, Ableton Live, Pro Tools, and FL Studio.

![Quanta MIDI Controller](/images/quanta_on.jpg)

This controller is a single large "_super button_" containing a ring of 16 LEDs which synchronize with the MIDI clock to count beats in **blue**, and bars in **green**. When starting playback at the first bar of the project, it can also track _phrases:_ groups of 16 bars, in **red**. The center **Q** softly pulses with a white backlight on each beat. The giant button can be used to control any MIDI parameter you like. 

![Quanta MIDI Controller](/images/quanta_keyboard.jpg)

## About
This project is one of my more experimental high-quality MIDI controllers that is designed to be low-cost. Think of it as a fancy pause button that shows you where you are within the song. This project is available in my 
[Etsy Store: Circuits & Sawdust](https://www.etsy.com/listing/774303547/quanta-one-button-midi-metronome).

## How it works
This is a ~~[Teensy LC](https://www.pjrc.com/teensy/) based~~ Arduino Pro Micro USB MIDI controller with an extra large arcade-style button that fits a [Neopixel](https://github.com/adafruit/Adafruit_NeoPixel) LED ring inside it. The controller sends and receives MIDI start/stop signals from the audio production program and displays the current position bars/beats/phrases on the button display.

*Version 1* used a Teensy MIDI implementation. *Version 2* uses the newer [MIDIUSB library](https://www.arduino.cc/en/Reference/MIDIUSB) for all Arduino boards with an ATmega32u4 chip.

The interesting part of this project is how it handles MIDI real time clock information. Check out the code to see how the project handles start, continue, and stop commands from the project timeline.

![Quanta with Project Timeline](/images/quanta_timeline.jpg)

## Ingredients
- ~~[Teensy LC](https://www.pjrc.com/teensy/) microcontroller~~
- Arduino Pro Micro, from Sparkfun or any 5V generic clone
- [Large 60mm arcade button](https://www.adafruit.com/product/1192) with white LED
- [16 Neopixel 5050 RGB LED ring](https://www.adafruit.com/product/1463)
- 2" PVC female pipe coupler, black paint, clear lacquer
