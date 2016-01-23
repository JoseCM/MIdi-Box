
#ifndef __MIDI_IO_H__
#define __MIDI_IO_H__

#include "MIDI_MsgBuffer.h"
#include <vector>



using namespace std;


class MIDI_IO
{
protected:
	vector<MIDI_MsgBuffer*> inBuffer;
	MIDI_MsgBuffer* outBuffer;
	vector<bool> inChannels; 
public:
	MIDI_IO();
	~MIDI_IO();
	virtual bool open() = 0;
	virtual bool close() = 0;
	MidiMessage readMidiMsg(int);
	void writeMidiMsg(MidiMessage&);
	bool registerChannel(int);
	bool unregisteChannel(int);
	bool channelOn(int);
};


#endif

