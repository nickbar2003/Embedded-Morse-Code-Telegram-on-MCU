#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL

// USART video: https://www.youtube.com/watch?v=KnyyQujvcBo&t=1423s
// I copied this code for serial communcation from the video linked above

void config_USART0();
void serial_write(uint8_t output);

int main(void)
{

    uint8_t fresh_click = 1; // 1 for fresh, 0 for stale
    

    // Port Direction Register 0 = input; 1 = output
    // Set PORTB4 to output
    DDRB = DDRB | (1 << DDB4);

    // Set PORTB3 to output
    DDRB = DDRB | (1 << DDB3);

    // Set PORTD4 is input by default
    DDRD = DDRD & ~(1 << DDD4);


    UCSR0C = (2 << UPM00) | (3 << UCSZ00); // Even parity, 8 data bits, 1 stop bit
    UBRR0L = 103; // sets baud rate to about 9600
    UCSR0B = (1 << TXEN0); // Enables Transmission


    while(1)
    {
        if(PIND & (1 << PD4) && fresh_click) // button depressed
        {
            fresh_click = 0;
            _delay_ms(250); // This delay controls how long short vs long intepretation
            

            if(~PIND & (1 << PD4)) // button no longer depressed
            {
                // Set PortB4
                PORTB = PORTB | (1 << PORTB4); // RED
                while(!(UCSR0A & (1 << UDRE0))); // Wait until USART Data Register is empty
                UDR0 = 83; // Send 'S'
                _delay_ms(200);
                // unset PORTB4
                PORTB = PORTB & ~(1 << PORTB4); // RED
            }

            if(PIND & (1 << PD4)) // button still pressed
            {
                // Set PortB3
                PORTB = PORTB | (1 << PORTB3); // BLUE
                while(!(UCSR0A & (1 << UDRE0))); // Wait until USART Data Register is empty
                UDR0 = 76; // Send 'L'
                _delay_ms(200);
                // unSet PortB3
                PORTB = PORTB & ~(1 << PORTB3); // BLUE
            }
        }

        if(~PIND & (1 << PD4)) // button no longer depressed
        {
            _delay_ms(50);
            fresh_click = 1;
        }
    }

    return 0;
    
}

// void serial_write(uint8_t var)
// {

//     while(!(UCSR0A & (1 << UDRE0))); // Wait until USART Data Register is empty
//     UDR0 = var;
//     _delay_ms(50);
// }




