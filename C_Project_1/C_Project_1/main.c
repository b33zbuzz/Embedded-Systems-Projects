#include <avr/io.h>
#define F_CPU 16000000UL    //Define CPU clock as 16Mhz
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
	DDRB = 0xFF; // PORT B for LED's
	DDRD = 0x00; // PORT D for interrupt
	
	// Setting EICRA values for ISC01 and ISC00 as interrupt INT0(D2) is used
	EICRA = (0<<ISC01) | (1<<ISC00);
	
	// setting EIMSK's position 1 element on
	EIMSK = 0b00000001;
	sei(); // sei always after EIMSK to enable interrupt

	
	// Turning LED on one after another after delay
	while(1)
	{
		PORTB = 0b00000100;
		_delay_ms(1000);
		PORTB = 0b00000010;
		_delay_ms(1000);
		PORTB = 0b00000001;
		_delay_ms(1000);
	}
}

// When interrupt sensed LED's blink simultaneously 3 times and then go back to normal function
ISR(INT0_vect)
{
	PORTB = 0b00000111;
	_delay_ms(100);
	PORTB = 0b00000000;
	_delay_ms(100);
	PORTB = 0b00000111;
	_delay_ms(100);
	PORTB = 0b00000000;
	_delay_ms(100);
	PORTB = 0b00000111;
	_delay_ms(100);
	PORTB = 0b00000000;
	_delay_ms(1000);
	cli();  // Disable interrupt
}