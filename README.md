# Project - Selective counter

Application of analog joy-stick (2 ADC channels, 1 push button), rotary encoder, and Digilent PmodCLP LCD module.

Our application consists of two separate counters - manual and automatic. The manual one is controlled by joy-stick component. The way you move the joy-stick determines direction of counting. The automatic one starts counting up immediately. You can stop the counting by pushing the button on encoder. By pushing the button on joy-stick, you resets the counter.

### Team members

* Adam Neradilek
* Ondřej Nesvadba

## Hardware description

For our application, we use three components properly connected to the Arduino Uno board. For each component, we have to connect GND pins to corresponding GND pins on Arduino board and VCC pins to 5V pin. Then there is number of data pins we have to connect according to the schematics bellow.

### Rotary encoder KY
![KY](https://user-images.githubusercontent.com/99417291/205458608-6e49dab6-0f86-4f83-8a98-29bd163364f7.png)

* Pin GND: ground
* Pin +: power supply 5 V
* Pin SW: transmission of information
* CLK, CT: signal pins

### Joystick HW-504
![HW-504](https://user-images.githubusercontent.com/99417291/205458618-b3d640bc-a72d-437f-a9a0-8da89a7361f3.png)

* Pin GND: ground
* Pin +5: power supply 5 V
* Pin Vrx: information about x-axis position 
* Pin Vry: information about y-axis position
* Pin SW: transmission of information

### LCD Digilent Pmod
![pmodclp-arduinouno_bb_gmplcfqZqg](https://user-images.githubusercontent.com/99417291/205458621-2fdcf0f4-b484-4f0c-83ce-bfd333b01045.png)

* Pins J1.1,2,3,4,5,6,7,8,9,10: transmission of information
* Piny J1.5, J1.11, J2.5: ground
* Piny J1.6, J1.12, J2.6: power supply 5 V
* Pin J2.1: register selection
* Pin J2.2: read/write
* Pin J2.3: enable signal

To connect all components to the right pins, we use breadboard. Final connection with all necessary components connected to the Arduino board is on the picture bellow.
![complete connection](https://user-images.githubusercontent.com/99417291/205458941-f0b1735d-0ad1-4454-a5d3-f12c880d6b9c.jpg)


## Software description
### Functions
As stated above, application consists of two counters. 
* Manual counter: Position of joy-stick determines the direction of counting. When pushed to the right in direction of x-axis, counter starts counting up. When pushed to the left, it starts counting down. It is set to stop decrementing when reaching value of zero.
* Automatic counter: Counting starts immediately. Counter automaticaly counts up. When pushed the button on rotary encoder, counting stops. To reset the counter, push the button on joy-stick.

### Project Structure
---------------------------------------------------------------
PROJECT_1        
- include        
  - [timer.h](https://github.com/xnerad04/Project_1/blob/main/project-01/include/timer.h)

- lib             
  - gpio
    - [gpio.c](https://github.com/xnerad04/Project_1/blob/main/project-01/lib/gpio/gpio.c)
    - [gpio.h](https://github.com/xnerad04/Project_1/blob/main/project-01/lib/gpio/gpio.h)
  - lcd
    - [lcd.c](https://github.com/xnerad04/Project_1/blob/main/project-01/lib/lcd/lcd.c)
    - [lcd.h](https://github.com/xnerad04/Project_1/blob/main/project-01/lib/lcd/lcd.h)
    - [lcd_definitions.h](https://github.com/xnerad04/Project_1/blob/main/project-01/lib/lcd/lcd_definitions.h)

- src           
  - [main.c](https://github.com/xnerad04/Project_1/blob/main/project-01/src/main.c)
- README.md       
---------------------------------------------------------------

### Flowcharts

#### ADC_vect
![adc](https://user-images.githubusercontent.com/99417291/206279790-4ba4b0f7-333a-4c79-bc20-40900500a751.jpg)

#### Counter_0 (Manual counter)
![counter_0](https://user-images.githubusercontent.com/99417291/206279970-d2351319-19d3-40a7-97ea-6fabb8df4635.jpg)

#### Counter_1 (Automatic counter)
![counter_1](https://user-images.githubusercontent.com/99417291/206279994-457aca6c-2abd-41c8-a1ef-0bf598abcdf8.jpg)

#### Reset Counter_1
![reset](https://user-images.githubusercontent.com/99417291/206280024-fab43dd7-17b8-4d36-8758-679fbd59ef6f.jpg)

#### Display values
![display](https://user-images.githubusercontent.com/99417291/206280259-6ce28f3e-b830-4681-95dc-dafe6d5ec4c4.jpg)

## Video

* For video see: [video presentation](https://drive.google.com/file/d/1CWMb7DfZeLsw64z7y-cciqaf_-bbAd5J/view?usp=share_link).
* For simulIde simulation see: [simulation](https://drive.google.com/file/d/1Q2TSgKQxPg-4rg3kyLVwANQnGo4HSZ4N/view?usp=share_link)

## References

1. https://digilent.com/reference/_media/reference/pmod/pmodclp/pmodclp_rm.pdf
2. https://digilent.com/reference/pmod/pmodclp/start?redirect=1
3. https://lastminuteengineers.com/rotary-encoder-arduino-tutorial/
4. https://components101.com/modules/joystick-module
