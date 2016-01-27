
#include "UART_IO.h"

#include <iostream>
#include <cstdint>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <string>


UART_IO::UART_IO(string device) : MIDI_IO()
{
	device_uart = device;
	fd_uart = -1;

	open();
}

UART_IO::~UART_IO()
{
	close();
}

bool UART_IO::open(){
	//"/dev/ttyAMA0"
	fd_uart = ::open(device_uart.c_str(), O_RDWR | O_NOCTTY);// | O_SYNC | O_DSYNC );// O_DIRECT);
	if(fd_uart < 0)
		perror("Opening uart decvice: ");

	struct termios options;
	tcgetattr(fd_uart, &options);
	options.c_cflag = B38400 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(fd_uart, TCIFLUSH);

    cfmakeraw(&options);
    tcsetattr(fd_uart, TCSANOW, &options);

    //printf("MIN: %d, TIME: %d", options.c_cc[VMIN], options.c_cc[VTIME]);

    return true;
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

    uint8_t nobytes = 0;
	uint8_t byte[3], counter = 0, counter2 = 0, temp = 0,  channel, *ptr = nullptr;
	MidiMessage msg;

	printf("-------------------\n");

	if((temp = read(fd_uart, byte, 1)) != 1){
        //printf("bytes received = %d (0x%x)\n", temp, byte[0]);
        return;
    }

   printf("bytes received = %d\n", temp);

    if(byte[0] == 0xf8 || byte[0] == 0xfe)
        return;

	if((counter2 = counter = MIDI_GetNoParam(byte[0]))){

        ptr = &byte[1];
        while(1){
            temp = read(fd_uart, ptr, counter);
            printf("read result = %d\n", temp);
            if(temp == counter)
                break;

            ptr += temp;
            counter -= temp;

        }

    }

	msg.setSize(counter2 + 1);
	for(int i = 0; i < counter2 + 1; i++){
		msg[i] = byte[i];
		printf(" %x", msg[i]);
    } printf("\n");

    channel = msg.getChannel();

    writeInMidiMsg(channel, msg);
}

void UART_IO::sendNextMidiMsg(){

	uint8_t byte[3], size;
	MidiMessage msg = readOutMIdiMsg();

	size = msg.getSize();

	for(int i = 0; i < size; i++)
		byte[i] = msg[i];

	write(fd_uart, byte, size);

}

void* UART_IO::Thread_InMidi(void* arg)
{
    UART_IO *io = static_cast<UART_IO*>(arg);

	while(1)
        io->getNextMidiMsg();
}

void* UART_IO::Thread_OutMidi(void* arg)
{
    UART_IO *io = static_cast<UART_IO*>(arg);

	while(1)
        io->sendNextMidiMsg();
}

void UART_IO::run()
{
    pthread_create(&handle1, NULL, UART_IO::Thread_InMidi, static_cast<void*>(this));
	pthread_create(&handle2, NULL, UART_IO::Thread_OutMidi, static_cast<void*>(this));
}


