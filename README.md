# DK-Quanta
_QUANTA_ is a one-button MIDI controller that displays the beats and bars along the project timeline in many popular music production programs such as Cubase, Logic, Ableton Live, Pro Tools, FL Studio, and more.

![Quanta MIDI Controller](/images/quanta_on.jpg)
![Quanta MIDI Controller](/images/quanta_keyboard.jpg)

This controller is a single large "super button" containing a ring of 16 LEDs which synchronize with the MIDI clock to count beats (in blue), and bars (in green). When starting playback at the first bar of the project, it can also track phrases (groups of 16 bars, in red). The center Q softly pulses with a white backlight on each beat. The giant button can be used to control any MIDI parameter you like. 

## About
This project is one of my more experimental high-quality MIDI controllers that is designed to be low-cost. It is available in my 
[Etsy Store: Circuits & Sawdust](https://www.etsy.com/listing/555989439).

## How it works
This is a [Teensy LC](https://www.pjrc.com/teensy/) based USB MIDI controller with an extra large arcade-style button that fits a [Neopixel](https://github.com/adafruit/Adafruit_NeoPixel) LED ring inside it. The controller sends and receives MIDI start/stop signals from the audio production program and displays the current position bars/beats/phrases on the button display.