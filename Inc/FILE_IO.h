
#ifndef __FILE_IO_H__
#define __FILE_IO_H__

#include "MIDI_IO.h"
#include "MidiFile.h"
#include "MidiEvent.h"
#include <string>


#define PATH_FILE "./MIDIFiles/"

using namespace std;

class FILE_IO: public MIDI_IO
{
    MidiFile midiFile;
    string midiFileName;
    int index;
public:
    FILE_IO(string filename);
    virtual ~FILE_IO();
	bool open();
    bool close();
    void truncate();
    void setTicksPerQuarterNote(int tick);
    void addMidiEvent(MidiEvent &event);
    int getNextMidiMsg(int channel, int tick);
    int getLastTick();
    void resetIndex();

    void run(){}

};


#endif

