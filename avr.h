#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#define BAUD 9600 // Bits per second for Tx and Rx


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


// Enum for defining Morse Code Data Buffer Translation values
// e.i. What is the equivilant english letter of every MCDB value
enum morse_translation
{
    EMPTY = 0b00000000,

    // 1 data bits
    E = 0b00100000,
    T = 0b00100001,
                 
    // 2 data bit,s
    A = 0b01000010,
    I = 0b01000000,
    M = 0b01000011,
    N = 0b01000001,
                 
    // 3 data bit,s
    D = 0b01100001,
    G = 0b01100011,
    K = 0b01100101,
    O = 0b01100111,
    R = 0b01100010,
    S = 0b01100000,
    U = 0b01100100,
    W = 0b01100110,
                 
    // 4 data bit,s
    B = 0b10000001,
    C = 0b10000101,
    F = 0b10000100,
    H = 0b10000000,
    J = 0b10001110,
    L = 0b10000010,
    P = 0b10000110,
    Q = 0b10001011,
    V = 0b10001000,
    X = 0b10001001,
    Y = 0b10001101,
    Z = 0b10000011,
    SPACE = 0b10001010,
    BACKSPACE = 0b10001100

    // 5 data bits


};

void USART_config(void);
void pin_direction_config(void);
char translate_letter(enum morse_translation);
void start_up_tune(void);
