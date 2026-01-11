#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL

unsigned char variable;
void Config_USART0();

int main(void)
{
    UCSR0C = (2 << UPM00) | (3 << UCSZ00);
    UBRR0L = 103;
    UCSR0B = (1 << TXEN0);

    // Config_USART0();
    uint8_t variable = 65;

    while(1)
    {
        while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = variable;
	variable = variable + 1;
	_delay_ms(1000);



    }

    return 0;
}

//void Config_USART0()
//{
//}



// int main(void)
// {
//     // Port Direction Register 0 = input; 1 = output
    
//     // uint16_t = 0;

//     // Set PORTB4 to output
//     DDRB = DDRB | (1 << DDB4);

//     // Set PORTB3 to output
//     D
//     DRB = DDRB | (1 << DDB3);

//     // Set PORTD4 is input by default
//     DDRD = DDRD & ~(1 << DDD4);

//     uint8_t fresh_click = 1; // 1 for fresh, 0 for stale

//     while(1)
//     {




//         if(PIND & (1 << PD4) && fresh_click) // button depressed
//         {
//             fresh_click = 0;
//             _delay_ms(250); // This delay controls how long short vs long intepretation
            

//             if(~PIND & (1 << PD4)) // button no longer depressed
//             {
//                 // Set PortB4
//                 PORTB = PORTB | (1 << PORTB4); // RED
//                 _delay_ms(200);
//                 // unset PORTB4
//                 PORTB = PORTB & ~(1 << PORTB4); // RED
//             }

//             if(PIND & (1 << PD4)) // button still pressed
//             {
//                 // Set PortB3
//                 PORTB = PORTB | (1 << PORTB3); // BLUE
//                 _delay_ms(200);
//                 // unSet PortB3
//                 PORTB = PORTB & ~(1 << PORTB3); // BLUE
//             }
//         }




//         if(~PIND & (1 << PD4)) // button no longer depressed
//         {
//             _delay_ms(50);
//             fresh_click = 1;
//         }




//     }



//     return 0;
// }
