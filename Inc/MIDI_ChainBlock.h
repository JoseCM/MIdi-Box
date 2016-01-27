#ifndef MIDI_CHAINBLOCK_H
#define MIDI_CHAINBLOCK_H

#include "MidiMessage.h"
#include "MIDI_MsgBuffer.h"


class MIDI_Chain;

class MIDI_ChainBlock
{
    protected:
        MIDI_MsgBuffer inBuffer;
        MIDI_ChainBlock *nextBlock;
        MIDI_Chain* motherChain;
        pthread_t blockThread;

    public:
        MIDI_ChainBlock(MIDI_Chain *mother = nullptr);

        MIDI_ChainBlock* getNextBlock();
        MIDI_Chain* getMotherChain();
        void setMotherChain(MIDI_Chain *);
        void setNextBlock(MIDI_ChainBlock *next);

        void passMidiMsg(MidiMessage &msg);
        virtual void ProcessMsg(MidiMessage&) { }

        virtual void run() = 0;

        virtual ~MIDI_ChainBlock();
};

#endif // MIDI_CHAINBLOCK_H
