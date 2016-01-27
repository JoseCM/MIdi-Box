#include <pthread.h>
#include <unistd.h>

#include "MIDI_IO.h"
#include "Physical_IO.h"
#include "UART_IO.h"
#include "MIDI_Chain.h"
#include "MIDI_IOBlock.h"


int main(int argc, char **argv){

	Physical_IO	*phys_io = new Physical_IO(I2CDEVICE, SPIDEVICE);
    UART_IO *uart_io = new UART_IO(UARTDEVICE);
    UART_IO *usb_io = new UART_IO(USBDEVICE);

    MIDI_InBlock *inblock = new MIDI_InBlock(0, phys_io);
    MIDI_OutBlock *outblock = new MIDI_OutBlock(0, usb_io);
    MIDI_Chain *testchain = new MIDI_Chain(inblock, outblock);

//    MIDI_InBlock *inblock2 = new MIDI_InBlock(0, uart_io);
//    MIDI_OutBlock *outblock2 = new MIDI_OutBlock(0, usb_io);
//    MIDI_Chain *testchain2 = new MIDI_Chain(inblock2, outblock2);

    phys_io->run();
    uart_io->run();
    usb_io->run();
    inblock->run();
    outblock->run();
//    inblock2->run();
//    outblock2->run();

	pthread_exit(NULL);

	return 0;
}
