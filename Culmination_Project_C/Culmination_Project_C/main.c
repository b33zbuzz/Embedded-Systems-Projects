#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "avr/interrupt.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																																	//
// Final Exam Task:																													//
// Set up three LEDs which will slowly change from dark to bright, with the max brightness controlled by a potentiometer.			//
// Hook a button up to EXT_INT0 which will toggle whether the LEDs fade from dark to bright or bright to dark.						//
// selected LED.																													//
//																																	//
// Circuit setup:  Button for triggering EXT_INT0, connected to 5V, GND, and pin D2													//
//				   Potentiometer for controlling max brightness, connected to analog pin A0 (PORT C), 5V and GND					//
//				   3 LEDs connected to PORT B pins 9, 10 and 11 respectively, and GND												//
//																																	//
// Requirements:   1. The LEDs will have their max brightness proportional to the value set by a potentiometer.					    //
//																																    //
//				   2. The button connected to EXT_INT0 will toggle whether the LEDs fade from dark to bright or bright to dark.     //
//																																	//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int direction_of_fading = 0;
int brightness = 0;

int main(void)
{
	cli();						
	
	// Timer 1 and Timer 2 Setup
	TCCR1A |= (1<<WGM10) | (1<<COM1A1) | (1<<COM1B1);		// Fast PWM mode, 8-bit for OCR1A and OCR1B
	TCCR1B |= (1<<WGM12) | (1<<CS10);						// Set prescalar to 1
	TCCR2A |= (1<<WGM21) | (1<<WGM20) | (1<<COM2A1);		// Fast PWM mode, 8-bit for OCR2A
	TCCR2B |= (1<<CS20);									// Set prescalar to 1
	
	// ADC Setup (Potentiometer)
	ADCSRA  = 0b10000111;									// Enable Analog Digital Converter, set prescaler to 128
	ADCSRB  = 0b00000000;									// Free Running mode
	ADMUX   = 0b00000000;									// Select ADC0 as input, AREF pin to be V_Ref
	DDRC    = 0x00;											// Set PORTC as input
	
	// External Interrupt Setup (Button)
	EIMSK   = 0b000000001;									// Enable external interrupt (INT0)
	EICRA   = 0b000000011;									// Trigger on rising edge
	DDRD   &= (0 << 2);										// Set INT0 pin to input
	
	// LED Setup
	DDRB    = 0xFF;											// Setting PORTB as output
	PORTB   = 0b00001110;									// Setting LED pins as output
	
	sei();													// Enabling interrupts globally

	
	// Loop for fading
	while(1)
	{
		if(direction_of_fading == 0)
		{
			// Setting fast PWM to non-inverting mode
			TCCR1A = 0b10100001;
			TCCR2A = 0b10000011;
			
			// Start an ADC conversion by setting ADSC bit
			ADCSRA |= (1 << ADSC);
			
			// Wait until the ADSC bit has been cleared (the conversion has been completed)
			while(ADCSRA & (1 << ADSC));
			
			// Start from brightness = 0 and increase brightness until value set by potentiometer
			OCR1A = brightness;
			OCR1B = brightness;
			OCR2A = brightness;
			_delay_ms(20);
			brightness += 1;
			if (brightness >= (ADC/4))	// Here ADC is divided by 4 as ADC is a 10-bit value and we are using 8-bit fast PWM mode
			{							// 10-bit fast PWM is only available in timer 1 not in timer 2 in data sheet
				brightness = 0;
				OCR1A = 0;
				OCR1B = 0;
				OCR2A = 0;
			}
		}
		else // Direction_of_fading = 1
		{
			// Setting fast PWM to inverting mode
			TCCR1A = 0b11110001;
			TCCR2A = 0b11000011;
			
			// Start an ADC conversion by setting ADSC bit
			ADCSRA |= (1 << ADSC);
			
			// Wait until the ADSC bit has been cleared (the conversion has been completed)
			while(ADCSRA & (1 << ADSC));
			
			// Start from brightness = 0 and increase brightness until value set by potentiometer
			OCR1A = brightness;
			OCR1B = brightness;
			OCR2A = brightness;
			_delay_ms(20);
			brightness += 1;
			if (brightness >= (ADC/4))	// Here ADC is divided by 4 as ADC is a 10-bit value and we are using 8-bit fast PWM mode
			{							// 10-bit fast PWM is only available in timer 1 not in timer 2 in data sheet
				brightness = 0;
				OCR1A = 0;
				OCR1B = 0;
				OCR2A = 0;
			}
		}
	}
}

// Timer compare function
ISR(TIMER1_COMPA_vect)
{
	PORTB ^= 0xFF;
}

// Pressing the button simply changes the variable 'direction_of_fading'
// This determines whether the timers are in non-inverting fast PWM mode or inverting fast PWM mode
ISR(INT0_vect)
{
	if(direction_of_fading == 0)
	{
		direction_of_fading = 1;
	}
	else // Direction_of_fading = 1
	{
		direction_of_fading = 0;
	}
}