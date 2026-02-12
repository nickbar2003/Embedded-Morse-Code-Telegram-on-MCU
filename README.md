# EMBEDDED MORSE CODE TELEGRAM ON MCU

## Project Intro

For a long time now, I have had an UNO R3 style microcontroller board. I have had really good ideas for projects and a real desire to delve into this subject.
Giving time to the hobby and study of AVR C programming on microcontrollers has paid off very quickly.

It has been very enjoyable to study and take notes on the ATmega320 MCU, USART protocol, and AVR programming concepts in general.

I wanted to start a project that would require me to explore each of these concepts in greater depth and give me a practical skill.
In seeking that end, I decided that trying to design my own Morse code telegram from scratch would be very fun.

I thought about all the little peripheral devices and how I could start stacking them up to create something that felt, sounded, and looked exciting. This endeavor has not only already greatly expanded my knowledge, but it has honestly been so fun. It is rewarding to think of a new feature, examine the task of implementation, try, iterate, and eventually succeed!

### Project Description

This project consists primarily of two parts: the firmware/software internals, and the hardware externals. These two parts integrate to create a working telegram.

#### | Hardware Solution: (To see diagrams of hardware implementation see /project_documents)

The hardware implementation is lightweight and practical. It has been designed with iteration in mind. The early conception takes form on the breadboard. Two buttons, three LEDs, a passive buzzer, resistors, and jumper cables are all that are needed.

Button B:

Receives input from the user as button depressions. The length of the depression is discerned as either short or long. These signals are converted to Morse code.

RED LED:
Shines when a short button B press is detected.

BLUE LED:
Shines when a long button B press is detected.

Button A:
When pressed, this button tells the program to translate the Morse buffer to English, then it initiates transmission of the English message.

Green LED
Shines when a long button A press is detected.

Passive Buzzer:
This buzzer emits different tones at different lengths for each button.
For button B, different lengths of tone are played depending on whether the button signal was short or long.

#### | Firmware and Software Solutions:

Software (main.cpp)
This cpp program, when launched, serves two primary functions. The first function is to compile the firmware program (avr.c) and then to flash this program to the ATmega320 microcontroller. The second is to service the telegram by monitoring the serial port. The MCU program will be transmitting both the Morse and English message. This cpp program will separate the streams and display them in a legible manner. It also provides nice aids, such as delimiters between words in the Morse stream.

Firmware (avr.c)
This is the largest portion of the project. This program is responsible for discerning the length of user input, translating said signals to Morse, and converting Morse to English. Utilizing the USART protocol, this program transmits the messages to the host PC. Translation from Morse to English is achieved using a single byte buffer!
