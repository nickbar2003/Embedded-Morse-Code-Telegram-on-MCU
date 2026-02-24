default:
	g++ main.cpp -lCppLinuxSerial -o program && ./program
clean:
	rm avr.o avr.bin avr.hex program
