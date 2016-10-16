# Mood lighting

Code for a little weekend project where I use an arduino to control an RGB LED strip for some mood lighting. The UI consists of a switch and 3 potentiometers.

The switch toggles between "manual" and "program" mode.

Manual means the 3 potentiometers directly control the red, green, and blue levels. Program means whatever program is on the arduino for the other case. The initial program for this is/was fading through the hue spectrum. I intend to re-use the potentiometers as program parameters.

An additional idea is to have one potentiometer be treated as a program selector, by dividing its range into sectors.
