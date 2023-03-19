#include <avr/io.h>
#define F_CPU 16000000UL // Define CPU clock as 16MHz
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
	DDRB = 0xFF;
	DDRD = 0xFF;
	
	PORTB = 0xFF;
	
	// Toggle and FAST PWM
	TCCR1A = (0 << COM1A1)|(1 << COM1A0)|(0 << WGM11)|(0 << WGM10);
	TCCR1B = (1 << WGM12)|(1<< CS12)|(0 << CS10);
	
	TIMSK1 = (1 << OCIE1A); // Set pin OC1A(B9) to 1
	
	TCNT1 = 0;
	OCR1A = 32000;
	OCR1B = 6400;
	sei();
	while(1)
	{
		
	}
}

ISR(TIMER1_COMPA_vect)
{
	PORTD ^= 0XFF;
	
	if (PORTD == 0xff)
	{
		OCR1A = 1000;
	}
	else{
		OCR1A = 43000;
	}
}