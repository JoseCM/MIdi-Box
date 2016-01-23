
#ifndef __FILE_IO_H__
#define __FILE_IO_H__

#include "MIDI_IO.h"
#include "MidiFile.h"


using namespace std;

class FILE_IO: public MIDI_IO
{
	MidiFile fd_midifile;
public:
	FILE_IO();
	~FILE_IO();
	bool open();
	bool close();
};


#endif

