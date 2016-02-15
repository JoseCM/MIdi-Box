#include "MIDI_IOBlock.h"

#include <iostream>
#include <stdio.h>
#include <cstring>
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
    io_stream->unregisteChannel(getChannel());
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
       // std::cout << "received message" << std::endl;

        io_block->passMidiMsg(msg);

    }

}

void MIDI_InBlock::run()
{
    struct sched_param pthread_param;
    pthread_attr_t thread_attr;

    pthread_param.sched_priority = 2;

    pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);
    pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedparam(&thread_attr, &pthread_param);

    pthread_create(&blockThread, NULL, MIDI_InBlock::Thread_In, static_cast<void*>(this));
}

void MIDI_InBlock::cancel(){
    pthread_cancel(blockThread);
    pthread_testcancel();
}

MIDI_OutBlock::MIDI_OutBlock(uint8_t channel, MIDI_IO* out, mqd_t queue) : MIDI_IOBlock(channel, out), queue_recorder(queue)
{

}

MIDI_OutBlock::~MIDI_OutBlock()
{
    MidiMessage msg( 0xB0 | getChannel() , 123 , 0);
    io_stream->writeOutMidiMsg(msg);
}

mqd_t MIDI_OutBlock::getQueueRecorder(){
    return queue_recorder;
}


void MIDI_OutBlock::run()
{
    struct sched_param pthread_param;
    pthread_attr_t thread_attr;

    printf("max_priority: %d\n", sched_get_priority_max(SCHED_RR));

    pthread_param.sched_priority = 2;

    pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);
    pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedparam(&thread_attr, &pthread_param);

    pthread_create(&blockThread, NULL, MIDI_OutBlock::Thread_Out, static_cast<void*>(this));

    //pthread_detach(blockThread);

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

    messageToRecord message_s;
    char message_v[50];

    pthread_detach(pthread_self());

    if(!io_block)
        pthread_exit(nullptr);

    if(!(chain = io_block->getMotherChain()))
        pthread_exit(nullptr);

    message_s.chain = chain;

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

        if(chain->getRecordingState()){
            message_s.msg = msg;
            memcpy(message_v, &message_s, sizeof(message_s));

            if(!mq_send(io_block->getQueueRecorder(), message_v, sizeof(message_s), 0))
                perror("queue_sent");

        }

    }
}





