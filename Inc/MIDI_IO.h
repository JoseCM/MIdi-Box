
#ifndef __MIDI_IO_H__
#define __MIDI_IO_H__

#include <pthread.h>
#include "MIDI_MsgBuffer.h"
#include <vector>



using namespace std;

class MIDI_IO
{
protected:
	vector<MIDI_MsgBuffer*> inBuffer;
	MIDI_MsgBuffer* outBuffer;
	vector<bool> inChannels;
	pthread_t handle1, handle2;

public:
	MIDI_IO();
	~MIDI_IO();
	virtual bool open() = 0;
	virtual bool close() = 0;

	MidiMessage readInMidiMsg(int);
	MidiMessage readOutMIdiMsg();
	void writeInMidiMsg(int, MidiMessage&);
	void writeOutMidiMsg(MidiMessage&);

	bool registerChannel(int);
	bool unregisteChannel(int);
	bool channelOn(int);

	virtual void run() = 0;
};


#endif

