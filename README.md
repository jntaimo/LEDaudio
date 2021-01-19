# LEDaudio
A set of functions that analyze audio on Teensy microcontrollers and display it in cool ways with addressable LEDs

## Table of Contents
- [LEDaudio](#ledaudio)
  - [Table of Contents](#table-of-contents)
  - [General Info](#general-info)
  - [Technologies](#technologies)
    - [Arduino IDE with Teensyduino](#arduino-ide-with-teensyduino)
    - [Teensy 4.1 microcontroller](#teensy-41-microcontroller)
    - [Teensy Audio Library](#teensy-audio-library)
    - [WS2812Serial](#ws2812serial)
    - [FastLED](#fastled)
    - [WS2812B Addressable RGB LED strips](#ws2812b-addressable-rgb-led-strips)
  - [Features](#features)
    - [To do:](#to-do)
  - [Sources](#sources)
  - [Inspiration](#inspiration)
  - [About me](#about-me)
  - [Contact](#contact)
## General Info
While all-in-one LED controllers have a lot of cool effects built in, I have found the sound-responsive ones to be something of a disappointment. The algorithms they use seem *super* basic, and ultimately fail to represent the **vibe** of the music, which is what I am always looking for. LEDaudio attempts to fix that by combining the excellent Teensy audio library with scalable functions that visualize music in cool ways!

## Technologies
### Arduino IDE with Teensyduino
  - The most straightforward way to program Teensy microcontrollers
### Teensy 4.1 microcontroller
  - Really fast microntroller made by PJRC, with an absurd number of ports and examples to base code off of.
### Teensy Audio Library
  - A fast and lightweight library made by the folks over at PJRC (who also designed Teensy)
  - Has support for easy audio analysis, especially the 1024 point Fast Fourier transform, which I use to get the audio spectrum.
### WS2812Serial
  - Another beauteous library by PJRC that allows for non-blocking control of LEDs, letting the audio library and LEDs run at the same time without breaking each other.
### FastLED
  - LED control library used in conjunction with WS2812BSerial
  - Has a lot of built-in functions that reduce the amount of necessary code.
### WS2812B Addressable RGB LED strips
  - Run on 5V, and are some of the most commonly used LED strips
  - Requires a single pin to control the entire length.

## Features
 - Basic 16 pixel spectrum analyzer display over USB audio
### To do:
- Scalable rainbow audio spectrum analyzer
- Beat detection
  - Changes response mode based on bpm
  - Led goes wump when the beat goes wump
- Adjustable global brightess with potentiometer
- Wifi control over ethernet connection
- One to multiple wireless LED control via serial connection to 
## Sources
 - Todo
## Inspiration
 - Todo
## About me
Welcome to the bottom of the page! You made it! My name is Joseph Ntaimo, and I'm a sophomore at MIT studying mechanical engineering and computer science. My primary interests include music technology (e.g.sound reactive LEDs) and hacking stuff together with microcontrollers. Additionaly, I DJ for my hall at MIT (Woah PUTZ!), and will definitely be using these functions when the hype train returns after Corona.
## Contact
Check out more about me and my projects at jntaimo.com.

Do you want to get in touch? Shoot me an email at jntaimo@mit.edu
