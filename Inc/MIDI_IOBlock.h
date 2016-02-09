#ifndef MIDI_IOBLOCK_H
#define MIDI_IOBLOCK_H

#include <cstdint>
#include "MIDI_ChainBlock.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

class MIDI_IO;

class MIDI_IOBlock : public MIDI_ChainBlock
{
    protected:
        uint8_t midiChannel;
        MIDI_IO *io_stream;

    public:
        MIDI_IOBlock(uint8_t channel = 0, MIDI_IO *io = nullptr);
        void setIOStream(MIDI_IO *io);
        MIDI_IO* getIOStream();
        uint8_t getChannel();
        void setChannel(uint8_t);
        virtual ~MIDI_IOBlock();
};

class MIDI_InBlock : public MIDI_IOBlock
{

    public:
        MIDI_InBlock(uint8_t channel = 0, MIDI_IO *in = nullptr);
        virtual ~MIDI_InBlock();

        virtual void run();
        virtual void cancel();
        static void* Thread_In(void*);

};

class MIDI_OutBlock : public MIDI_IOBlock
{
    mqd_t queue_recorder;

    public:
        MIDI_OutBlock(uint8_t channel = 0, MIDI_IO *out = nullptr, mqd_t queue = 0);
        virtual ~MIDI_OutBlock();
        mqd_t getQueueRecorder();

        virtual void run();
        virtual void cancel();
        static void* Thread_Out(void*);

};

#endif // MIDI_IOBLOCK_H
