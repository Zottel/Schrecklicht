
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <string.h>

uint32_t longest = 0;
uint32_t middle = 0;

uint32_t max_diff = 0;

uint8_t led_state = 0;

ISR(ADC_vect) {
	uint8_t measurement = ADCH;
	
	longest -= (longest >> 20);
	longest += measurement;
	
	middle -= (middle >> 12);
	middle += measurement;
	
	
	if(middle > ((longest >> 8) + 1024)) {
		PORTB = (1 << PB0) | (1 << PB1) | (1 << PB2);
	} else {
		led_state = (led_state + 1) % 15;
		if(led_state < 5) {
			PORTB = (0 << PB0) | (1 << PB1) | (1 << PB2);
		}
		else if(led_state < 10) {
			PORTB = (1 << PB0) | (0 << PB1) | (1 << PB2);
		}
		else{
			PORTB = (1 << PB0) | (1 << PB1) | (0 << PB2);
		}
	}
	
}

ISR(TIMER0_OVF_vect) {
	//PORTB ^= (1 << PB0) | (1 << PB1) | (1 << PB2);
	//PORTB ^= (0 << PB0) | (0 << PB1) | (1 << PB2);
}

void adc_setup(void) {
	// Vcc as reference voltage
	ADMUX = (0 << REFS2) | (0 << REFS1) | (0 << REFS0)
	// Left adjust result
	      | (1 << ADLAR)
	// PB4 as single ended input
	      | (0 << MUX3) | (0 << MUX2) | (1 << MUX1) | (0 << MUX0);
	
	// Enable ADC
	ADCSRA = (1 << ADEN)
	// Start first conversion
	       | (1 << ADSC)
	// Enable auto trigger
	       | (1 << ADATE)
	// Enable ADC interrupt
	       | (1 << ADIE)
	// ADC clock = system clock / 2    (fastest setting)
	       | (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);
}

void timer_setup(void) {
	// Fast PWM, inverting mode
	TCCR0A = (1 << WGM00) | (1 << WGM01);
	//TCCR0A = (1 << COM0A1) | (1 << COM0A0) | (1 << WGM00) | (1 << WGM01);
	// Fast PWM and no prescaler
	TCCR0B = (0 << WGM02)  | (0 << CS02)   | (0 << CS01)  | (1 << CS00);
	// Enable interrupt on overflow
	TIMSK |= (1 << TOIE0);
}

int main(void) {
	DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2);
	
	// Pull LEDs high -> off
	PORTB = (1 << PB0) | (1 << PB1) | (1 << PB2);
	
	//timer_setup();
	adc_setup();
	
	set_sleep_mode(SLEEP_MODE_IDLE);
	
	sei();
	
	while(1) {
		asm volatile("nop");
		sleep_mode();
	}
}

