#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>



// Controlling an LED with a button A
//
// Remember the LED circuit is actually seperate from the button circuit.
//
// The button circut will be used to determine whether we want to close the LED circuit.
//


int main(void)
{
    // Port Direction Register 0 = input; 1 = output



    // Set PORTB4 to output
    DDRB = DDRB | (1 << DDB4);

    // Set PORTB3 to output
    DDRB = DDRB | (1 << DDB3);

    // Set PORTD4 is input by default
    DDRD = DDRD & ~(1 << DDD4);

    while(1)
    {



        //_delay_ms(500);

        if(PIND & (1 << PD4))
        {
                // unset PORTB3
                PORTB = PORTB & ~(1 << PORTB3);

                // Set PortB4
                PORTB = PORTB | (1 << PORTB4);
        }
        else
        {
                // unset PORTB4
                PORTB = PORTB & ~(1 << PORTB4);

                // Set PortB3
                PORTB = PORTB | (1 << PORTB3);
        }


    }



    return 0;
}