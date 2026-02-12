/*
Author: Nicholas Barinaga
Date: 1/17/2026
File Description:
Notes: 
- For reading data on the serial port I am using gbmhunter's "CppLinuxSerial Project" on github. 
Full transparency: I only use a couple serial commands in the program and I pretty much used the example he post's on his github page.
Link to his serial port project: https://github.com/gbmhunter/CppLinuxSerial



*/

#include <charconv>
#include <iostream>
#include <CppLinuxSerial/SerialPort.hpp>
#include <cstdlib>
#include <chrono>
#include <string>
#include <thread>

#define LOOP_DELAY_MS 15

using namespace std::chrono;
using namespace std::this_thread;
using namespace mn::CppLinuxSerial;


void print_message(std::string);
void print_context();

std::string  curr_morse_string = "";
std::string morse_message = "";
std::string english_message = "";
std::string serial_data = "";

int main(void)
{



    


    system("./start.sh");
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    print_message("LAUNCHING PROGRAM [===========================] DONE\n");

    

    SerialPort serialPort("/dev/ttyACM0", BaudRate::B_9600, NumDataBits::EIGHT, Parity::NONE, NumStopBits::ONE);

    serialPort.SetTimeout(2000); 
	serialPort.Open();
    
    sleep_for(milliseconds(500));





    print_message("INCOMING TRANSMISSION");
    for(int i = 0; i < 2; i++)
    {
        print_message(" ..... ");
        print_message("\b\b\b\b\b\b\b");
        print_message("       ");
        print_message("\b\b\b\b\b\b\b");
    }

    print_context();

    while(1)
    {

        serial_data = "";
        serialPort.Read(serial_data);

        if(serial_data == "-" || serial_data == ".")
        {
            morse_message += serial_data;
        }
        else if( serial_data >= "A" && serial_data <= "Z" )
        {
            morse_message += " ";
            english_message += serial_data;
        }
        else if(serial_data == " ")
        {
            morse_message += " | ";
            english_message += serial_data;
        }
        else if(serial_data == "<")
        {
            morse_message += serial_data;
        }

        print_context();


    }

    serialPort.Close();
    return 0;
}

void print_context()
{


    uint8_t space_after_message = 0;
    uint8_t num_rows = 25;
    uint8_t num_columns = 90;
    uint8_t morse_pos = num_rows / 6;
    uint8_t english_pos = morse_pos * 4;

    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";


    for(int r = 0; r <= num_rows; r++)
    {

        if(r == morse_pos)
        {
            space_after_message = num_columns - 10 - morse_message.size();


            std::cout << "| Morse: " << morse_message;

            for (int i = 0; i <= space_after_message; i++ ) 
            {
                std::cout << " ";
            
            }
            std::cout << "|";

        }
        else if(r == english_pos)
        {
            space_after_message = num_columns - 12 - english_message.size();

            std::cout << "| English: " << english_message;

            for (int i = 0; i <= space_after_message; i++ ) 
            {
                std::cout << " ";
            
            }
            std::cout << "|";


        }
        else if(r == 0 || r == num_rows)
        {
            std::cout << "|-----------------------------------------------------------------------------------------|";

        }
        else if(r == num_rows / 2)
        {
            std::cout << "|.........................................................................................|";

        }
        else 
        {
            std::cout << "|                                                                                         |";

        }
        

        std::cout << "\n";
    }


}


void print_message(std::string message)
{
    for(int index = 0; index < message.size(); index++)
    {
        std::cout.flush();
        sleep_for(milliseconds(LOOP_DELAY_MS));

        std::cout << message[index];
    }
}