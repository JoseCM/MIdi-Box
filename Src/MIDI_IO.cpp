
#include "MIDI_IO.h"

MIDI_IO::MIDI_IO()
{	
	inBuffer.assign(16, NULL);
	inChannels.assign(16, false);
	outBuffer = new MIDI_MsgBuffer();

}

MIDI_IO::~MIDI_IO()
{
	delete outBuffer;
	for(int i=0; i<16; i++){
		if(inBuffer[i] != NULL)
			delete inBuffer[i];
	}
}

MidiMessage MIDI_IO::readMidiMsg(int channel)
{
	MidiMessage message;

	if(channelOn(channel))
	{	
		message = inBuffer[channel]->readMidiMsg();	
	}

	return message;
}

void MIDI_IO::writeMidiMsg(MidiMessage &msg)
{
		outBuffer->writeMidiMsg(msg);
}

bool MIDI_IO::registerChannel(int channel)  //type: 0-in, 1-out
{
	if(channel > 15 || channel < 0)
		return false;

	if(inChannels[channel] == false){
		inBuffer[channel] = new MIDI_MsgBuffer;
		inChannels[channel] = true;
	} else
		return false;

	
	
	return true;
}

bool MIDI_IO::unregisteChannel(int channel)  //type: 0-in, 1-out
{
	if(channel > 15 || channel < 0)
		return false;


	if(inChannels[channel] == true){
		delete inBuffer[channel];
		inBuffer[channel] = NULL;
		inChannels[channel] = false;
	}
	else  
		return false;
	
	return true;
}

bool MIDI_IO::channelOn(int channel)
{	
	return inChannels[channel];
	
}

