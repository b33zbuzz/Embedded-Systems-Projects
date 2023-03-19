#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "avr/interrupt.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Lab Task:
//
// Set up three LEDs which will have their brightness controlled by a potentiometer. Hook a button up to EXT_INT0 which will cycle the currently
// selected LED.
//
// Circuit setup:  Button for triggering EXT_INT0,
//				   Potentiometer connected to ADC0,
//				   3 LEDs (from here on we will call them LED_A, LED_B, and LED_C), connected to analog outputs pins. Do not forget resistors!
//
// Requirements:   1. The active LED will have its brightness proportional to the value set by a potentiometer.
//
//				   2. The button connected to EXT_INT0 will change the "Active LED" from LED_A -> LED_B -> LED_C -> LED-A ...
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Pin Definitions
#define OUTPUT_PORT PORTD // Assume all output LEDs are connect to the same port
#define LED_A_PIN 5
#define LED_B_PIN 6
#define LED_C_PIN 7

volatile uint8_t activeLEDPin = LED_A_PIN;

// Ensure to connect AREF pin to 5V

int main(void)
{
	// ADC Setup
	ADCSRA = 0b10000111;	// Enable Analog Digital Converter, set prescaler to 128
	ADCSRB = 0b00000000;	// Free Running mode
	ADMUX  = 0b00000000;	// Select ADC0 as input, AREF pin to be V_Ref
	DDRC   = 0x00;			// Set PORTC as input

	// Timer setup
	TIMSK1 = 0b00000011;	// Compare and overflow matching enabled
	TCCR1A = 0b00000000;	// Normal port operation (define interrupts to turn LEDs on/off manually)
	TCCR1B = 0b00000001;	// Set prescaler to 1

	EIMSK  = 0b000000001;	// Enable external interrupt (INT0)
	EICRA  = 0b000000011;	// Trigger on rising edge
	DDRD   &= (0 << 2);		// Set INT0 pin to input
	
	// LED Setup
	OUTPUT_PORT |= (1 << LED_A_PIN) || (1 << LED_B_PIN) || (1 << LED_C_PIN); // Set pins for LEDs to output
	
	sei();					// Enable interrupts
	
	while (1)
	{
		// Set "Start Conversion" bit
		ADCSRA |= 1 << ADSC;
		
		// Adjust LED brightness based off of potentiometer
		OCR1A = ADC * 64;
		// ADC = 1024*V(in)/V(aref) [0, 1023]
		// map [0, 1023] to [0, 65535]
		// -> multiply x by 64
	}
}

// Timer Compare: turn on
ISR(TIMER1_COMPA_vect)
{
	OUTPUT_PORT |= (1 << activeLEDPin);
}

// Timer Overflow: turn off
ISR(TIMER1_OVF_vect)
{
	// Turn all LEDs off such that on active LED change no LEDs are left on
	OUTPUT_PORT &= (0 << LED_A_PIN) || (0 << LED_B_PIN) || (0 << LED_C_PIN);
}


// On button press, cycle through the LEDs, setting one to be active.
ISR(INT0_vect)
{
	if (activeLEDPin == LED_A_PIN)
	{
		activeLEDPin = LED_B_PIN;
	}
	else if (activeLEDPin == LED_B_PIN)
	{
		activeLEDPin = LED_C_PIN;
	}
	else // activeLEDPin == LED_C_PIn
	{
		activeLEDPin = LED_A_PIN;
	}
}