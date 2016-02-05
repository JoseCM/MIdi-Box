#include "MIDI_IOBlock.h"

#include <iostream>
#include <stdio.h>
#include "MIDI_IO.h"

MIDI_IOBlock::MIDI_IOBlock(uint8_t channel, MIDI_IO* io) : midiChannel(channel), io_stream(io)
{

}

MIDI_IOBlock::~MIDI_IOBlock()
{
    //dtor
}

void MIDI_IOBlock::setIOStream(MIDI_IO* io)
{
    io_stream = io;
}

MIDI_IO* MIDI_IOBlock::getIOStream()
{
    return io_stream;
}

uint8_t MIDI_IOBlock::getChannel()
{
    return midiChannel;
}

void MIDI_IOBlock::setChannel(uint8_t channel)
{
    midiChannel = channel;
}

MIDI_InBlock::MIDI_InBlock(uint8_t channel, MIDI_IO* in) : MIDI_IOBlock(channel, in)
{
    in->registerChannel(channel);
}

MIDI_InBlock::~MIDI_InBlock()
{

}

void* MIDI_InBlock::Thread_In(void *argument)
{
    MIDI_InBlock *io_block = static_cast<MIDI_InBlock*>(argument);
    MIDI_IO *io_stream = nullptr;
    MIDI_Chain *chain = nullptr;
    MidiMessage msg;

    if(io_block == nullptr)
        pthread_exit(nullptr);

    if((chain = io_block->getMotherChain()) == nullptr)
        pthread_exit(nullptr);

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);


    while(1){

        if((io_stream = io_block->getIOStream()) == nullptr)
            pthread_exit(nullptr);

        msg = io_stream->readInMidiMsg(io_block->getChannel());
        //std::cout << "received message" << std::endl;

        io_block->passMidiMsg(msg);

    }

}

void MIDI_InBlock::run()
{
    pthread_create(&blockThread, NULL, MIDI_InBlock::Thread_In, static_cast<void*>(this));
}

void MIDI_InBlock::cancel(){
    pthread_cancel(blockThread);
    pthread_testcancel();
}

MIDI_OutBlock::MIDI_OutBlock(uint8_t channel, MIDI_IO* out) : MIDI_IOBlock(channel, out)
{

}

MIDI_OutBlock::~MIDI_OutBlock()
{

}

void MIDI_OutBlock::run()
{
    pthread_create(&blockThread, NULL, MIDI_OutBlock::Thread_Out, static_cast<void*>(this));
}


void MIDI_OutBlock::cancel(){
    pthread_cancel(blockThread);
    pthread_testcancel();
}

void* MIDI_OutBlock::Thread_Out(void *argument)
{
    MIDI_OutBlock *io_block = static_cast<MIDI_OutBlock*>(argument);
    MIDI_IO *io_stream = nullptr;
    MIDI_Chain *chain = nullptr;
    MidiMessage msg;

    if(!io_block)
        pthread_exit(nullptr);

    if(!(chain = io_block->getMotherChain()))
        pthread_exit(nullptr);

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    while(1){

        msg = io_block->inBuffer.readMidiMsg();

        if((io_stream = io_block->getIOStream()) == nullptr)
            pthread_exit(nullptr);

        //change channel
        if ((msg[0] & 0xF0) != 0xF0)
            msg[0] = (msg[0] & 0xF0) | (io_block->getChannel() & 0x0F);


        io_stream->writeOutMidiMsg(msg);
        //std::cout << "sent message" << std::endl;

    }
}





