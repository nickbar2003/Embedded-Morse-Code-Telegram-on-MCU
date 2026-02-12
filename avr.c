#include "avr.h"
// My microcontroller board has an ATmega328P microcontroller //
// For all w/r operations I am referencing the ATmega328P data sheet // 

// Documentation //
// ATmega328P datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
// Arduion UNO R3 pinout diagram: https://docs.arduino.cc/resources/pinouts/A000066-full-pinout.pdf


int main(void)
{
    /*
    Port to hardware correspondance:
    (See Adrunio Uno R3 pinout diagram linked above)

    NAME   | PCB Pin NAME   | Hardware Function
    --------------------------------------------
    PORTB4 | Digital Pin 12 | Powering a Red LED
    ---------------------------------------------
    PORTB3 | Digital Pin 11 | Powering a Blue LED
    ---------------------------------------------
    PORTD4 | Digital Pin 4 | Recieving input from button
    
    */


    uint8_t fresh_click = 1; // 1 for fresh, 0 for stale

    enum morse_translation MCDB = 0b00000000; // Morse Code Data Buffer
    uint8_t data_size_mask = 0b00000000; // Used in tandem with MCDB
    char letter = ' ';


    // // //  USART0 CONFIGURATION // // //
    USART_config();


    // // // PORT DIRECTION CONFIGURATION // // //
    pin_direction_config();


    start_up_tune();

    while(1)
    {

        if(PIND & (1 << PD2)) // Button A clicked
        {
            _delay_ms(200); // Pause to discern length of input



            if(PIND & (1 << PD2)) // Long input
            {
                while(!(UCSR0A & (1 << UDRE0))); // Wait for UDR empty

                PORTB = PORTB | (1 << PORTB0); 

                _delay_ms(100); // let LED shine

                PORTB = PORTB & ~(1 << PORTB0); 

                _delay_ms(100); // let LED shine

                PORTB = PORTB | (1 << PORTB0); 

                _delay_ms(100); // let LED shine

                PORTB = PORTB & ~(1 << PORTB0); 
            }
            else if (~PIND & (1 << PD2)) // Short input
            {
                PORTB = PORTB | (1 << PORTB0); 

                for(int i = 0; i < 400; i++)
                {
                    PORTB = PORTB | (1 << PORTB1);
                    _delay_us(250);
                    PORTB = PORTB & ~(1 << PORTB1);
                    _delay_us(250);
                }

                PORTB = PORTB & ~(1 << PORTB0); 


                letter = translate_letter(MCDB);


                while(!(UCSR0A & (1 << UDRE0))); // Wait for UDR empty
                UDR0 = letter; // Transmit English char

                MCDB = 0b00000000; // Wipe buffer
                data_size_mask = 0b00000000; // Wipe mask

            }




        }




        if(PIND & (1 << PD4) && fresh_click) // button B clicked 
        {
            fresh_click = 0; // no longer a new click

            _delay_ms(200); // Wait after inital click to discern length


            if(~PIND & (1 << PD4)) // button B no longer depressed == Short button press
            { 


                data_size_mask = MCDB; 
                data_size_mask = data_size_mask >> 5; 

                MCDB = MCDB & ~(1 << data_size_mask); 
                MCDB += 0b00100000;

                while(!(UCSR0A & (1 << UDRE0))); // Wait for UDR empty
                UDR0 = '.'; 

                PORTB = PORTB | (1 << PORTB4); // Turn on red LED

                for(int i = 0; i < 200; i++)
                {
                    PORTB = PORTB | (1 << PORTB1);
                    _delay_us(600);
                    PORTB = PORTB & ~(1 << PORTB1);
                    _delay_us(400);
                }

                PORTB = PORTB & ~(1 << PORTB4); // turn LED off

            }
            else if(PIND & (1 << PD4)) // button still pressed == long button press
            {


                data_size_mask = MCDB; 
                data_size_mask = data_size_mask >> 5; 

                MCDB = MCDB | (1 << data_size_mask); 
                MCDB += 0b00100000;


                while(!(UCSR0A & (1 << UDRE0))); // Wait for UDR empty
                UDR0 = '-'; 

                PORTB = PORTB | (1 << PORTB3); // Turn on blue LED

                for(int i = 0; i < 400; i++)
                {
                    PORTB = PORTB | (1 << PORTB1);
                    _delay_us(600);
                    PORTB = PORTB & ~(1 << PORTB1);
                    _delay_us(400);
                }

                PORTB = PORTB & ~(1 << PORTB3); // turn LED off
            }
        }


        // If here then:
        // Button has either not been pressed, 
        // or input has already been serviced
        if(~PIND & (1 << PD4)) // Button is unpressed
        {
            _delay_ms(10);
            fresh_click = 1; // Ready to service new click
        }
    }

    return 0;
    
}



void USART_config(void)
{
    // // //  USART0 CONFIGURATION // // //
    
    /* 
    
    Desired Settings for USART0 :

    In USCR0C (register):
        - USART mode = Asynchronous
        - Parity mode = disabled
        - Stop bit select = 1 stop bit
        - Data bit size = 8 bits
        - Baud rate = 9600

    In USCR0B (Register):
        Tx Enable = 1

    */

    // USART mode = 00 = Async mode
    UCSR0C = UCSR0C & ~(1 << UMSEL00); // Set bit 6 = 0
    UCSR0C = UCSR0C & ~(1 << UMSEL01); // Set bit 7 = 0


    // Parity mode = 00 = disabled
    UCSR0C = UCSR0C & ~(1 << UPM00); // Set bit 4 = 0
    UCSR0C = UCSR0C & ~(1 << UPM01); // Set bit 5 = 0
    
    // Stop bit select = 0 = one stop bit
    UCSR0C = UCSR0C & ~(1 << USBS0); // Set bit 3 = 0

    // For data size setting 11 = 8 data bits
    UCSR0C = UCSR0C | (1 << UCSZ01); // Set bit 2 = 1
    UCSR0C = UCSR0C | (1 << UCSZ00); // Set bit 1 = 1

    // Set USART Baud Rate Register for desired baud rate of 9600 to 103
    // For equation info about this calculation see ATmega328P datasheet pg. 146
    UBRR0L = 103; // UBBR0n = 103 = (fosc) / (16 * BAUD)

    // Enable transmission 
    UCSR0B = UCSR0B | (1 << TXEN0);

    return;
}


void pin_direction_config(void)
{
    // // // PORT DIRECTION CONFIGURATION // // //
    
    // Port Direction Register 0 = input pin; 1 = output pin

    // Set PORTB4 to output
    DDRB = DDRB | (1 << DDB1);

    // Set PORTB4 to output
    DDRB = DDRB | (1 << DDB4);

    // Set PORTB3 to output
    DDRB = DDRB | (1 << DDB3);

    // Set PORTB3 to output
    DDRB = DDRB | (1 << DDB0);

    // Set PORTD2 is input by default
    DDRD = DDRD & ~(1 << DDD2);

    // Set PORTD4 is input by default
    DDRD = DDRD & ~(1 << DDD4);

}

char translate_letter(enum morse_translation buffer)
{

    switch(buffer)
    {
        
        case A:
            return 'A';
            break;
        case B:
            return 'B';
            break;
        case C:
            return 'C';
            break;
        case D:
            return 'D';
            break;
        case E:
            return 'E';
            break;
        case F:
            return 'F';
            break;
        case G:
            return 'G';
            break;
        case H:
            return 'H';
            break;
        case I:
            return 'I';
            break;
        case J:
            return 'J';
            break;
        case K:
            return 'K';
            break;
        case L:
            return 'L';
            break;
        case M:
            return 'M';
            break;
        case N:
            return 'N';
            break;
        case O:
            return 'O';
            break;
        case P:
            return 'P';
            break;
        case EMPTY:
            return '#';
            break;
    }
    switch(buffer)
    {
        case Q:
            return 'Q';
            break;
        case R:
            return 'R';
            break;
        case S:
            return 'S';
            break;
        case T:
            return 'T';
            break;
        case U:
            return 'U';
            break;
        case V:
            return 'V';
            break;
        case W:
            return 'W';
            break;
        case X:
            return 'X';
            break;
        case Y:
            return 'Y';
            break;
        case Z:
            return 'Z';
            break;
        case SPACE:
            return ' ';
            break;
        case CLEAR:
            return '<';
            break;
        case EMPTY:
            return '#';
            break;
    }
}

void start_up_tune(void)
{
    for (int i = 0; i < 200; i++)
    {
        PORTB |=  (1 << PORTB1);
        _delay_us(100);
        PORTB &= ~(1 << PORTB1);
        _delay_us(100);
    }
    for (int i = 0; i < 300; i++)
    {
        PORTB |=  (1 << PORTB1);
        _delay_us(200);
        PORTB &= ~(1 << PORTB1);
        _delay_us(200);
    }
    for (int i = 0; i < 500; i++)
    {
        PORTB |=  (1 << PORTB1);
        _delay_us(400);
        PORTB &= ~(1 << PORTB1);
        _delay_us(400);
    }
    for (int i = 0; i < 600; i++)
    {
        PORTB |=  (1 << PORTB1);
        _delay_us(600);
        PORTB &= ~(1 << PORTB1);
        _delay_us(600);
    }
}