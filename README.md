# Project 1

Application of analog joy-stick (2 ADC channels, 1 push button), rotary encoder, and Digilent PmodCLP LCD module.

### Team members

* Adam Neradilek
* Ondřej Nesvadba

## Hardware description

For our application, we use three components properly connected to the Arduino Uno board. For each component, we have to connect GND pins to corresponding GND pins on Arduino board and VCC pins to 5V pin. Then there is number of data pins we have to connect according to the schematics bellow.

### Rotary encoder KY
![KY](https://user-images.githubusercontent.com/99417291/205458608-6e49dab6-0f86-4f83-8a98-29bd163364f7.png)

Pin GND: ground
Pin +: power supply 5 V
Pin SW: transmission of information
CLK, CT: signal pins

### Joystick HW-504
![HW-504](https://user-images.githubusercontent.com/99417291/205458618-b3d640bc-a72d-437f-a9a0-8da89a7361f3.png)

Pin GND: ground
Pin +5: power supply 5 V
Pin Vrx: information about x-axis position 
Pin Vry: information about y-axis position
Pin SW: transmission of information

### LCD Digilent Pmod
![pmodclp-arduinouno_bb_gmplcfqZqg](https://user-images.githubusercontent.com/99417291/205458621-2fdcf0f4-b484-4f0c-83ce-bfd333b01045.png)

Pins J1.1,2,3,4,5,6,7,8,9,10: transmission of information
Piny J1.5, J1.11, J2.5: ground
Piny J1.6, J1.12, J2.6: power supply 5 V
Pin J2.1: register selection
Pin J2.2: read/write
Pin J2.3: enable signal

To connect all components to the right pins, we use breadboard. Final connection with all necessary components connected to the Arduino board is on the picture bellow.
![complete connection](https://user-images.githubusercontent.com/99417291/205458941-f0b1735d-0ad1-4454-a5d3-f12c880d6b9c.jpg)


## Software description

Put flowchats of your algorithm(s). Write descriptive text of your libraries and source files. Put direct links to these files in `src` or `lib` folders.

## Video

Insert a link to a short video with your practical implementation example (1-3 minutes, e.g. on YouTube).

## References

1. Write your text here.
2. ...
