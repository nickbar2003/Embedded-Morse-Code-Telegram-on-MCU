#include "avr.h"
// My microcontroller board has an ATmega328P microcontroller //
// For all w/r operations I am referencing the ATmega328P data sheet // 

// Reference Documentation //

// ATmega328P datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
// Arduion UNO R3 pinout diagram: https://docs.arduino.cc/resources/pinouts/A000066-full-pinout.pdf

/*

PORT HARDWARE CORRESPONDANCE TABLE (See Adrunio Uno R3 pinout diagram linked above)

| Register Name | PCB Pin Name | Hardware Piece  | Function                                   |
| ------------- | ------------ | --------------- | ------------------------------------------ |
| PORTB5        | Pin 13       | Red LED         | Visual feedback for short (dot) input      |
| PORTB3        | Pin 11       | Blue LED        | Visual feedback for long (dash) input      |
| PORTB1        | Pin 9        | Morse Button    | Button for inputting morse                 |
| PORTD7        | Pin 7        | Green Led       | Visual feedback for transmission or delete |
| PORTD5        | Pin 5        | Transmit Button | Button for transmitting and deleting       |
| PORTD2        | Pin 2        | Passive Buzzer  | Varying Audio feedback for all inputs      |


*/

int main(void)
{




  uint8_t fresh_click = 1; // 1 for fresh, 0 for stale

  enum morse_translation MCDB = 0b00000000; // Morse Code Data Buffer
  uint8_t data_size_mask = 0b00000000; // Used in tandem with MCDB
  char letter = ' ';

  // uint8_t short_signal = 0b00000000; 
  // uint8_t long_signal = 0b00000000; 


  // // //  USART0 CONFIGURATION // // //
  USART_config();


  // // // PORT DIRECTION CONFIGURATION // // //
  pin_direction_config();


  start_up_tune();

  while(1)
  {

      // Service for transmission button input //

      if(PIND & (1 << PD5) && fresh_click) // Tx button clicked
      {
          fresh_click = 0; // no longer a new click

          _delay_ms(200); // Wait after inital click to discern length

          if(PIND & (1 << PD5)) // Long input
          {

              PORTD = PORTD | (1 << PORTD7); // Turn on green LED

              // Play buzzer tone
              for(int i = 0; i < 500; i++) 
              {
                  PORTD = PORTD | (1 << PORTD2);
                  _delay_us(200);
                  PORTD = PORTD & ~(1 << PORTD2);
                  _delay_us(200);
              }

              PORTD = PORTD & ~(1 << PORTD7); // Turn off green LED

              while(!(UCSR0A & (1 << UDRE0))); // Wait for UDR empty
              UDR0 = '<'; // Transmit backspace

              MCDB = 0b00000000; // Wipe buffer
              data_size_mask = 0b00000000; // Wipe mask


          }
          else if (~PIND & (1 << PD5)) // Short input
          {
              PORTD = PORTD | (1 << PORTD7); // Turn on green LED

              // Play buzzer tone
              for(int i = 0; i < 200; i++)
              {
                  PORTD = PORTD | (1 << PORTD2);
                  _delay_us(500);
                  PORTD = PORTD & ~(1 << PORTD2);
                  _delay_us(500);
              }

              PORTD = PORTD & ~(1 << PORTD7); // Turn off green LED


              letter = translate_letter(MCDB);


              while(!(UCSR0A & (1 << UDRE0))); // Wait for UDR empty
              UDR0 = letter; // Transmit English char

              MCDB = 0b00000000; // Wipe buffer
              data_size_mask = 0b00000000; // Wipe mask

          }

      }




      // Service for morse button inputs //

      if(PINB & (1 << PB1) && fresh_click) // Morse button clicked
      {
          fresh_click = 0; // no longer a new click

          _delay_ms(200); // Wait after inital click to discern length


          if(~PINB & (1 << PB1)) // Short (dot) input
          { 


              data_size_mask = MCDB; 
              data_size_mask = data_size_mask >> 5; 

              MCDB = MCDB & ~(1 << data_size_mask); 
              MCDB += 0b00100000;

              while(!(UCSR0A & (1 << UDRE0))); // Wait for UDR empty
              UDR0 = '.'; 

              PORTB = PORTB | (1 << PORTB5); // Turn on red LED

              // Play buzzer tone
              for(int i = 0; i < 200; i++)
              {
                  PORTD = PORTD | (1 << PORTD2);
                  _delay_us(600);
                  PORTD = PORTD & ~(1 << PORTD2);
                  _delay_us(400);
              }

              PORTB = PORTB & ~(1 << PORTB5); // Turn off red LED

          }
          else if(PINB & (1 << PB1)) // Long (dash) input
          {

              data_size_mask = MCDB; 
              data_size_mask = data_size_mask >> 5; 

              MCDB = MCDB | (1 << data_size_mask); 
              MCDB += 0b00100000;


              while(!(UCSR0A & (1 << UDRE0))); // Wait for UDR empty
              UDR0 = '-'; 

              PORTB = PORTB | (1 << PORTB3); // Turn on blue LED

              // Play buzzer tone
              for(int i = 0; i < 400; i++)
              {
                  PORTD = PORTD | (1 << PORTD2);
                  _delay_us(600);
                  PORTD = PORTD & ~(1 << PORTD2);
                  _delay_us(400);
              }

              PORTB = PORTB & ~(1 << PORTB3); // turn LED off
          }
      }


      // If here then either:
      // No button is pressed
      // or button input has been serviced
      if(~PINB & (1 << PB1) && ~PIND & (1 << PD5)) // Both buttons have to become unpressed to be renabled
      {
          _delay_ms(10);
          fresh_click = 1; // Renable service for new clicks
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

    // PORTB5 to output
    DDRB = DDRB | (1 << DDB5); // Red LED

    // PORTB3 to output
    DDRB = DDRB | (1 << DDB3); // Blue LED

    // PORTB1 to input
    DDRB = DDRB & ~(1 << DDB1); // Morse Button

    // PORTD7 to output
    DDRD = DDRD | (1 << DDD7); // Green LED

    // PORTB5 to input
    DDRD = DDRD & ~(1 << DDD5); // Transmit Button

    // PORTD2 to output
    DDRD = DDRD | (1 << DDD2); // Passive Buzzer


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
        case BACKSPACE:
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
        PORTD |=  (1 << PORTD2);
        _delay_us(100);
        PORTD &= ~(1 << PORTD2);
        _delay_us(100);
    }
    for (int i = 0; i < 300; i++)
    {
        PORTD |=  (1 << PORTD2);
        _delay_us(200);
        PORTD &= ~(1 << PORTD2);
        _delay_us(200);
    }
    for (int i = 0; i < 500; i++)
    {
        PORTD |=  (1 << PORTD2);
        _delay_us(400);
        PORTD &= ~(1 << PORTD2);
        _delay_us(400);
    }
    for (int i = 0; i < 600; i++)
    {
        PORTD |=  (1 << PORTD2);
        _delay_us(600);
        PORTD &= ~(1 << PORTD2);
        _delay_us(600);
    }
}

// Don't need to do it this way
            // short_signal = 0b00000000; 
            // long_signal = 0b00000001; 

            // // _delay_ms(200); // Pause to discern length of input

            // for (uint8_t i = 0; i < 4; i++) 
            // {
            //     _delay_ms(50); // Pause to discern length of input

            //     if(~PIND & (1 << PD2)) // Short input
            //     {
            //         long_signal = long_signal && 0; // Set long false
            //         short_signal = short_signal || 1; // Set short to true
            //         break;
            //     }
            // }