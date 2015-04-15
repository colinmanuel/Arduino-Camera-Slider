Arduino Slider
===============================

## Goal
Capture the scale and spirit of Southwest Texas with a sliding time lapse video. The [resulting video](https://vimeo.com/93436904) was shot on a late March vacation to Marfa, Alpine, Terlingua and Big Bend National Park. In hindsight, the project was pretty much just an excuse to squeeze an ungodly amount of time, money and frustration into a minute and a half of frenetic storytelling. On that count, I think it succeeded.

Arduino LCD Screen
~~~~~~~~~~~~~~~~~~~~~~~~~
![Arduino Closeup](/img/arduino-closeup.jpg?raw=true "Arduino Closeup")

Slider Close-up 
~~~~~~~~~~~~~~~~~~~~~~~~~
![Slider Closeup](/img/slider-closeup.jpg?raw=true "Slider Closeup")

Completed Slider
~~~~~~~~~~~~~~~~~~~~~~~~~
![Slider Complete](/img/slider-complete.jpg?raw=true "Slider Complete")

Slider in Action
~~~~~~~~~~~~~~~~~~~~~~~~~
![Slider in Action](/img/slider-action.jpg?raw=true "Slider in Action")

## Features
- Arduino Uno drives a 12V motor on a fixed track and fires the camera at fixed intervals
- LCD screen grants fine grain user control of the interval duration (how long the motor carrying the camera runs on the track and how long it pauses) as well as how long to wait before triggering a shot

## Software
- C/C++
- Arduino
- Lightroom 3
- [LRTimelapse](http://lrtimelapse.com/)

## Hardware
- Canon 7D
- Lenses - EF-S 10-22mm f/3.5-4.5, EF 28mm f/1.8, EF 50mm f/1.4
- [Arduino Uno](http://www.adafruit.com/products/50)
- [OSEPP Motor Shield](http://osepp.com/products/shield-arduino-compatible/motor-servo-shield/)
- [OSEPP RGB LCD Shield](http://osepp.com/products/shield-arduino-compatible/16x2-lcd-display-keypad-shield/)
- Dolly Track / Camera Slider - [Igus Rail DIY slider conversion](zazaslider.com/instructions.html)

## Shooting Locations
- [El Cosmico](http://elcosmico.com/stay) - Marfa, TX
- Prada Marfa - Valentine, TX
- Ross Maxwell Scenic Drive - Big Bend National Park
- Chisos Basin Road - Big Bend National Park
- [Wild Horse Station](http://www.tripadvisor.com/Hotel_Review-g56758-d122796-Reviews-Wildhorse_Station-Terlingua_Texas.html) - Terlingua, TX
- Hot Springs on Rio Grande - Big Bend National Park

## TODO
- Create an Instructable detailing steps to build and code the intervalometer
- Add diode to prevent potential current surge to camera
- Create a bluetooth version that can be controlled by a smartphone
