#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000 // default clock rate?


// Using internal clock and its overflow function to
// turn built in LED (pin 13) on and off every 1 second


// This function is ran when timer 1 overflow condition met
ISR(TIMER1_OVF_vect)
{
    PORTB = PORTB | (1 << PORTB5);

    TCNT1 = 65535 - (F_CPU/1024); 
}


int main(void)
{
    // Port Direction Register 0 = input; 1 = output


    // Set PORTB5 to output
    DDRB = DDRB | (1 << DDB5);

    // Timer/Counter 1
    // Overflow value
    // ISR triggers when this value is passed
    // By default this value is incremented at 16 Mhz
    // Counter has a max value of 65535 before overflow
    TCNT1 = 65535 - (F_CPU/1024); 

    // Timer normally operates at 16Mhz which is too fast,
    // Setting prescaler  now means it operates at 16Mhz / 1024 = 15625Hz
    // Essentially this command slows down the clock
    // Timer control register 1 part b
    TCCR1B = (1 << CS10) | (1 << CS12); // Set prescaler value 1024

    // This disables clock functions other than the overflow func
    TCCR1A = 0x00;

    // Enabling timer overflow interrupt enbable
    TIMSK1 = (1 << TOIE1);

    // Don't understand this yet
    sei(); // Set enable interrupts

    while(1)
    {
        ;; // Empty loop, we are utilzing ISR here
    }









    

    // // Set PORTB4 to output
    // DDRB = DDRB | (1 << DDB4);

    // // Set PORTB3 to output
    // DDRB = DDRB | (1 << DDB3);

    // // Set PORTD4 is input by default
    // DDRD = DDRD & ~(1 << DDD4);








    return 0;
}
