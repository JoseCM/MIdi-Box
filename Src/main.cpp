#include <pthread.h>
#include <unistd.h>

#include "MIDI_IO.h"
#include "Physical_IO.h"
#include "UART_IO.h"
#include "MIDI_Chain.h"
#include "MIDI_IOBlock.h"


int main(int argc, char **argv){

	//Physical_IO	*phys_io = new Physical_IO(I2CDEVICE, SPIDEVICE);
    UART_IO *uart_io = new UART_IO(UARTDEVICE);
    UART_IO *usb_io = new UART_IO(USBDEVICE);

    //phys_io->run();
    uart_io->run();
    usb_io->run();

    MIDI_InBlock *inblock = new MIDI_InBlock(0, usb_io);
    MIDI_OutBlock *outblock = new MIDI_OutBlock(0, uart_io);

    MIDI_Chain *testchain = new MIDI_Chain(inblock, outblock);
    inblock->run();
    outblock->run();

	pthread_exit(NULL);

	return 0;
}
