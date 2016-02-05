#ifndef MIDI_PROCESSBLOCK_H
#define MIDI_PROCESSBLOCK_H

#include "MIDI_ChainBlock.h"


class MIDI_ProcessBlock: public MIDI_ChainBlock
{
public:
    MIDI_ProcessBlock();
    ~MIDI_ProcessBlock();

    virtual void run();
    virtual void cancel();
    static void* Thread_Processing(void*);

};



class MIDI_Monitor: public MIDI_ProcessBlock
{
public:
    MIDI_Monitor();
    virtual void ProcessMsg(MidiMessage& msg);



};

class MIDI_Scale: public MIDI_ProcessBlock
{

public:
    MIDI_Scale();
    virtual void ProcessMsg(MidiMessage& msg);


};



#endif // MIDI_PROCESSBLOCK_H
