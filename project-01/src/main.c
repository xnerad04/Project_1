/***********************************************************************
 * 
 * Use Analog-to-digital conversion to read push buttons on LCD keypad
 * shield and display it on LCD screen.
 * 
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2018 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions


/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion every 100 ms.
 *           When AD conversion ends, send converted value to LCD screen.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(0, 0); lcd_puts("Timer:");
    lcd_gotoxy(0, 1); lcd_puts("Project_1");
    lcd_gotoxy(7, 0); lcd_puts("00");  // Put ADC value in decimal
    lcd_gotoxy(9, 0); lcd_puts(":");
    lcd_gotoxy(10,0); lcd_puts("00");  // Put ADC value in hexadecimal
    lcd_gotoxy(12, 0); lcd_puts(":");
    lcd_gotoxy(13,0); lcd_puts("0");
    lcd_gotoxy(7,0);

    // Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
    ADMUX = ADMUX | (1<<REFS0);
    // Select input channel ADC0 (voltage divider pin)
    ADMUX = ADMUX & ~(1<<MUX3 | 1<<MUX2 | 1<<MUX1 | 1<<MUX0);
    // Enable ADC module
    ADCSRA = ADCSRA | (1<<ADEN);
    // Enable conversion complete interrupt
    ADCSRA = ADCSRA | (1<<ADIE);
    // Set clock prescaler to 128
    ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 33 ms and enable overflow interrupt
    TIM1_overflow_33ms();
    TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Use single conversion mode and start conversion every 100 ms.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    // Start ADC conversion
    ADCSRA |= (1<<ADSC);
}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display converted value on LCD screen.
 **********************************************************************/

ISR(ADC_vect)
{
    static uint8_t no_of_overflows = 0;
    static uint8_t tenths = 0;  // Tenths of a second
    static uint8_t secs = 0;    // Seconds
    static uint8_t mins = 0;;   // Minutes
    char string[2];             // String for converted numbers by itoa()
    static uint8_t value = 1000;

    no_of_overflows++;
    if (no_of_overflows >= 6)
    {
        // Do this every 6 x 16 ms = 100 ms
        no_of_overflows = 0;

        if (value >= 1000)
        {

            // Count tenth of seconds 0, 1, ..., 9, 0, 1, ...
            tenths++;
            if (tenths > 9)
            {
                tenths = 0;
                // Add seconds to stopwatch
                secs++;
                if (secs > 59)
                {
                    secs = 0;
                    mins++;

                    if (mins > 59)
                    {
                        mins = 0;
                    }

                    itoa(mins, string, 10);
                    lcd_gotoxy(7, 0);
                    if (mins < 10)
                        lcd_puts('0');
                    lcd_puts(string);

                }
                // Display seconds
                itoa(secs, string, 10);
                lcd_gotoxy(10, 0);
                if (secs < 10)
                    lcd_putc('0');
                lcd_puts(string);
            }
            // Show tenths of a second on LCD screen
            itoa(tenths, string, 10);  // Convert decimal value to string
            // Display "00:00.tenths"
            lcd_gotoxy(13, 0);
            lcd_puts(string);
        }

        if (value <= 5)
        {

            // Count tenth of seconds 0, 1, ..., 9, 0, 1, ...
            tenths--;
            if (tenths == 0)
            {
                tenths = 9;
                // Add seconds to stopwatch
                secs--;
                if (secs == 0)
                {
                    secs = 59;
                    mins--;

                    if (mins == 0)
                    {
                        mins = 0;
                    }

                    itoa(mins, string, 10);
                    lcd_gotoxy(7, 0);
                    if (mins < 10)
                        lcd_puts('0');
                    lcd_puts(string);

                }
                // Display seconds
                itoa(secs, string, 10);
                lcd_gotoxy(10, 0);
                if (secs < 10)
                    lcd_putc('0');
                lcd_puts(string);
            }
            // Show tenths of a second on LCD screen
            itoa(tenths, string, 10);  // Convert decimal value to string
            // Display "00:00.tenths"
            lcd_gotoxy(13, 0);
            lcd_puts(string);
        }
        // Else do nothing and exit the ISR
    }


}
/*
ISR(ADC_vect)
{
    uint16_t value;
    uint16_t xval;

    char string[4];  // String for converted numbers by itoa()

    char string2[3];  // String for converted numbers by itoa()

    // Read converted value
    // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
    value = ADC;
    // Convert "value" to "string" and display it 

    if (value <= 5)
    {
      xval--;
      lcd_gotoxy(0, 1);
      lcd_puts("Pressing left "); 
    }         

    else if (value >= 1000)
    {
      xval++;
      lcd_gotoxy(0, 1);
      lcd_puts("Pressing right"); 
    }

    lcd_gotoxy(xval, 0);
    lcd_puts("5");   
}
*/