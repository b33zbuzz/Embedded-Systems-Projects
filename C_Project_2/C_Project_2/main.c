#include <avr/io.h>
#define F_CPU 16000000UL    // Define CPU clock as 16Mhz
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
	DDRB = 0xFF;
	PORTB = 0x00;
	
	TCNT1= 0x00;   // set timer to 0 initially
	
	TIMSK1 = (1<<TOIE1);  // ENABLE the internal interrupt
	
	TCCR1B = (0<<CS12) | (1<<CS11) | (1<<CS10);   // used this for spec freq, PRESCALER(64)
	sei();
	
	while(1)
	{}
}

ISR(TIMER1_OVF_vect)
{
	PORTB ^= 0xFF; // load high value to the LED
}