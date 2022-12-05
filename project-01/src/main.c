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

/* Defining buttons and encoders ------------------------------------ */
#define CLK PB4 // Encoder
#define DT PB3  // Encoder
#define SW PB2  // Button on encoder
#define BTN PD2 // Button on joystick

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion every 100 ms.
 *           When AD conversion ends, send converted value to LCD screen.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    GPIO_mode_input_pullup(&DDRB, CLK);
    GPIO_mode_input_pullup(&DDRB, DT);

    // Initialize display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(0, 0); lcd_puts("Joystick: ");
    lcd_gotoxy(0, 1); lcd_puts("Automatic:");    
    lcd_gotoxy(11, 0); lcd_puts("0000");
    lcd_gotoxy(11, 1); lcd_puts("0000");

    // Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
    ADMUX = ADMUX | (1<<REFS0);
    // Select input channel ADC0 (voltage divider pin)
    ADMUX = ADMUX & ~(1<<MUX3 | 1<<MUX2 | 1<<MUX1 | 1<<MUX0);                       // ADC0
    /*ADMUX = ADMUX & ~(1<<MUX3 | 1<<MUX2 | 1<<MUX1); ADMUX = ADMUX | (1<<MUX0);*/  // ADC1
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
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    // Start ADC conversion
    ADCSRA |= (1<<ADSC);
}

/**********************************************************************
 * Function: ADC complete interrupt
 **********************************************************************/

ISR(ADC_vect)
{
    static uint8_t no_of_overflows = 0;

    // Values for Counter0
    static uint8_t ones = 0;    // Ones
    static uint8_t tens = 0;    // Tens
    static uint8_t hunds = 0;   // Hundreds
    static uint8_t thous = 0;   // Thousands

    // Values for Counter1
    static uint8_t ones1 = 0;    // Ones
    static uint8_t tens1 = 0;    // Tens
    static uint8_t hunds1 = 0;   // Hundreds
    static uint8_t thous1 = 0;   // Thousands    

    char string[2];             // String for converted numbers by itoa()
    uint16_t value = 0;         // Value for ADC conversion

    int KY_BTN = GPIO_read(&PINB, SW);  // Encoder button

    value = ADC;    // Value from joystick

    no_of_overflows++;
    if (no_of_overflows >= 6)
    {
        no_of_overflows = 0;

        /* Counter1 --------------------------------------------------------------------- */
        if(KY_BTN == 0)     // If button is pressed, Counter1 stops
        {}

        else                // If button is released, Counter1 runs
        {
            ones1++;
            if(ones1 > 9)
            {
                ones1 = 0;
                tens1++;
                if(tens1 > 9)
                {
                    tens1 = 0;
                    hunds1++;
                    if(hunds1 > 9)
                    {
                        hunds1 = 0;
                        thous1++;
                        if(thous1 > 9)
                        {
                            ones1 = 0;
                            tens1 = 0;
                            hunds1 = 0;
                            thous1 = 0;
                        }
                    }
                }
            }
        }
        /* ------------------------------------------------------------------------------ */



        /* Counter0 --------------------------------------------------------------------- */
        // Counting UP
        if (value <= 300)
        {
            ones++;
            if (ones > 9)
            {
                ones = 0;                
                tens++;
                if (tens > 9)
                {
                    tens = 0;
                    hunds++;

                    if (hunds > 9)
                    {
                        hunds = 0;
                        thous++;
                    }
                }
            }
        }

        // Counting DOWN
        else if (value >= 700)
        {
            ones--;
            if (ones > 200)
            {
                ones = 9;
                tens--;
                if (tens > 200)
                {
                    tens = 9;
                    hunds--;
                    if (hunds > 200)
                    {
                        hunds = 9;
                        thous--;
                        if(thous > 200)
                        {
                            thous = 0;
                            hunds = 0;
                            tens = 0;
                            ones = 0;
                        }
                    }
                }
            }
        }

        // Counting stopped
        else{}
        /* ------------------------------------------------------------------------------ */



        /* Joystick button - RESET Counter1 --------------------------------------------- */
        int JS_BTN = GPIO_read(&PIND,BTN);

        if(JS_BTN == 0)
        {
            ones1 = 0;
            tens1 = 0;
            hunds1 = 0;
            thous1 = 0;
        }
        else{}
        /* ------------------------------------------------------------------------------ */



        /* Converting values to string and displaying them ------------------------------ */
        // Displaying Counter0
        itoa(ones, string, 10);
        lcd_gotoxy(14, 0);
        lcd_puts(string);
        itoa(tens, string, 10);
        lcd_gotoxy(13, 0);
        lcd_puts(string);
        itoa(hunds, string, 10);
        lcd_gotoxy(12, 0);
        lcd_puts(string);
        itoa(thous, string, 10);
        lcd_gotoxy(11, 0);
        lcd_puts(string);
        lcd_gotoxy(15, 0);
        lcd_puts(" ");

        // Displaying Counter1
        itoa(ones1, string, 10);
        lcd_gotoxy(14, 1);
        lcd_puts(string);
        itoa(tens1, string, 10);
        lcd_gotoxy(13, 1);
        lcd_puts(string);
        itoa(hunds1, string, 10);
        lcd_gotoxy(12, 1);
        lcd_puts(string);
        itoa(thous1, string, 10);
        lcd_gotoxy(11, 1);
        lcd_puts(string);
        lcd_gotoxy(15, 1);
        lcd_puts(" ");
        /* ------------------------------------------------------------------------------ */

    }

}