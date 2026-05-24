#define F_CPU 16000000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

volatile uint32_t millis;

ISR(TIMER0_COMPA_vect)
{
	millis++;
}

void timer0_init(void)
{
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS00) | (1 << CS01);
	OCR0A = 249;
	TIMSK0 = (1 << OCIE0A);
	TCNT0 = 0;
}

uint32_t millis_1(void)
{
	uint32_t m;
	uint8_t sreg = SREG;
	cli();
	m = millis;
	SREG = sreg;
	return m;
}

int main(void)
{
	timer0_init();
	sei();
	DDRD &= ~(1 << PD7);
	PORTD |= (1 << PD7);
	DDRB |= (1 << PB5);
	uint32_t last_time_debounce = 0;
	
	// ***************************************
	
	uint8_t state1 = 0;
	uint8_t state2 = 0;
	while (1)
	
	{
		uint32_t value = millis_1();
		uint8_t reading = (!(PIND & (1 << PD7)));
		
		if(reading != state1)
		{
			last_time_debounce = value;
		}
		if((reading != state2) && (value - last_time_debounce) >= 20)
		{
			state2 = reading;
		}
		state1 = reading;
		
		// *************************************************************
		
		if(state2)
		{
			PORTB |= (1 << PB5);
		}
		else
		{
			PORTB &= ~(1 << PB5);
		}
	}
}

