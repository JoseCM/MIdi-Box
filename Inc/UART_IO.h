#ifndef __UART_IO_H__
#define __UART_IO_H__

#include "MIDI_IO.h"
#include <string>
#include <cstdint>

#define UARTDEVICE "/dev/ttyAMA0"
#define USBDEVICE  "/dev/ttyUSB0"

using namespace std;


class UART_IO : public MIDI_IO
{
	int fd_uart;
	string device_uart;
public:
	UART_IO(string);
	~UART_IO();
	void getNextMidiMsg();
	void sendNextMidiMsg();
	uint8_t MIDI_GetNoParam(uint8_t status);
	bool open();
	bool close();

    static void* Thread_InMidi(void *arg);
    static void* Thread_OutMidi(void *arg);
	virtual void run();

};


#endif
