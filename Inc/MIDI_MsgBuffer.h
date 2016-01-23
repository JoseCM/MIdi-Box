#ifndef __MIDI_MESSAGEBUFFER_H_
#define __MIDI_MESSAGEBUFFER_H_

#include "MIDI_Library/MidiMessage.h"
#include <list>
#include <pthread.h>

class MIDI_MsgBuffer
{
	pthread_mutex_t mutex;
	pthread_cond_t cond_var;
	list<MidiMessage> buffer;
public:
	MIDI_MsgBuffer();
	~MIDI_MsgBuffer();
	MidiMessage readMidiMsg(void);
	void writeMidiMsg(MidiMessage&);
};



#endif
