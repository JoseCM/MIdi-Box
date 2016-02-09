#ifndef __Physical_IO_H__
#define __Physical_IO_H__

#include "MIDI_IO.h"
#include "TrellisI2C.h"
#include <string>
#include <cstdint>

#define NOTE_ON	 1
#define NOTE_OFF 0

#define SPIDEVICE "/dev/spidev0.0"
#define I2CDEVICE "/dev/i2c-1"


using namespace std;

class Physical_IO: public MIDI_IO
{
	int fd_i2c, fd_spi;
	string device_i2c, device_spi;
	vector<MidiMessage> buttonMap;
	vector<MidiMessage> encoderMap;
	float encoder_value[4];

public:
	Physical_IO(string, string);
    virtual ~Physical_IO();
	bool open();
	bool close();
	bool addMidiMsgtoButton(int, MidiMessage&);
	bool addMidiMsgtoEncoder(int, MidiMessage&);
	MidiMessage& buttonToMidiMsg(int button, uint8_t noteonff);
	MidiMessage& encoderToMidiMsg(int);
	uint8_t updateEncoderState();
	uint8_t updateButtonState();
    void upOctave();
    void downOctave();

    static void* Thread_InButton(void *arg);
    static void* Thread_InEncoder(void *arg);
	virtual void run();
};

#endif
