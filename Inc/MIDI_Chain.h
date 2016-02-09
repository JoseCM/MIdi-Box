#ifndef MIDI_CHAIN_H
#define MIDI_CHAIN_H

#include <pthread.h>
#include <list>


class MIDI_ChainBlock;
class MIDI_IOBlock;

class MIDI_Chain
{
    std::list<MIDI_ChainBlock*> processBlockList;
    MIDI_IOBlock *inputBlock;
    MIDI_IOBlock *outputBlock;
    pthread_mutex_t chainMutex;
    bool isRecording;

    public:
        MIDI_Chain(MIDI_IOBlock *in, MIDI_IOBlock *out);
        void insertBlock(int pos, MIDI_ChainBlock*);
        void removeBlock(int pos);
        void lockChain();
        void unlockChain();
        void setInputBlock(MIDI_IOBlock *);
        void setOutputBlock(MIDI_IOBlock *);
        virtual ~MIDI_Chain();
        void setRecordingState(bool state);
        bool getRecordingState();
        MIDI_IOBlock* getOutputBlock();
        MIDI_IOBlock* getInputBlock();
};

#endif // MIDI_CHAIN_H
