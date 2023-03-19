#include <avr/io.h>
#define F_CPU 16000000UL // Setting CPU Frequency 16MHz
#include <util/delay.h>  // Delay library
#include<avr/interrupt.h>

int clear;

void setup_adc()
{
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // Enabling ADEN and Setting prescalar to 128
	ADMUX = (1<<REFS0) | (0<<REFS1); // Setting ADMUX for AVCC with external capacitor
}

int main(void)
{
	DDRD = 0xFF;
	setup_adc();
	
	while (1)
	{
		ADCSRA |= (1<<ADSC);		// Enabling ADSC
		while(ADCSRA & (1<<ADSC));	// Checking loop until the value is zero
		PORTD = ADCL;				// Giving output acc to change
		clear = ADCH;				// Clearing other value and deleting it
	}
}