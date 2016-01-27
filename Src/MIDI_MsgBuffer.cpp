
#include "MIDI_MsgBuffer.h"
#include <pthread.h>


MIDI_MsgBuffer::MIDI_MsgBuffer() : mutex(PTHREAD_MUTEX_INITIALIZER), cond_var(PTHREAD_COND_INITIALIZER)
{

}

MIDI_MsgBuffer::~MIDI_MsgBuffer()
{

}

MidiMessage MIDI_MsgBuffer::readMidiMsg(void)
{
	MidiMessage message;

	pthread_mutex_lock(&mutex);

	while(!buffer.size())
	{
		pthread_cond_wait(&cond_var, &mutex);
	}

	message = buffer.back();
	buffer.pop_back();

	pthread_mutex_unlock(&mutex);

	return message;
}

void MIDI_MsgBuffer::writeMidiMsg(MidiMessage &message)
{
	pthread_mutex_lock(&mutex);

	buffer.push_front(message);

	pthread_mutex_unlock(&mutex);

	pthread_cond_signal(&cond_var);
}
