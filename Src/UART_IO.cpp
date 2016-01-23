
#include "UART_IO.h"

#include <cstdint>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <string>


UART_IO::UART_IO(string device) : MIDI_IO()
{
	device_uart = device;
	fd_uart = -1;
}

UART_IO::~UART_IO()
{
	close();
}

bool UART_IO::open(){
	//"/dev/ttyAMA0"
	fd_uart = ::open("/dev/ttyAMA0", O_RDWR | O_NOCTTY);
	if(fd_uart < 0)
		perror("Opening uart decvice: ");

	struct termios options;
	tcgetattr(fd_uart, &options);
	options.c_cflag = B38400 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(fd_uart, TCIFLUSH);
	tcsetattr(fd_uart, TCSANOW, &options);

}

bool UART_IO::close(){

	if(::close(fd_uart) < 0)
		return false;
	return true;
}



uint8_t UART_IO::MIDI_GetNoParam(uint8_t status){

	status &= 0xF0;

	if(status == 0xF0){
		return 0;
	} else if(status == 0xD0){
		return 1;
	} else {
		return 2;
	}

}

void UART_IO::getNextMidiMsg(){

	uint8_t byte[3], counter;
	MidiMessage msg;

	read(fd_uart, byte, 1);
	if(counter = MIDI_GetNoParam(byte[0]))
		read(fd_uart, &byte[1], counter);

	msg.setSize(counter + 1);
	for(int i = 0; i < counter + 1; i++)
		msg[i] = byte[i];

}

void UART_IO::sendNextMidiMsg(MidiMessage &msg){

	uint8_t byte[3], size;

	size = msg.getSize();

	for(int i = 0; i < size; i++)
		byte[i] = msg[i];

	write(fd_uart, byte, size);

}
