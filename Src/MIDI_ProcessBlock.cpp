#include "MIDI_ProcessBlock.h"

MIDI_ProcessBlock::MIDI_ProcessBlock()
{

}

MIDI_ProcessBlock::~MIDI_ProcessBlock()
{

}

void MIDI_ProcessBlock::run()
{ 
    struct sched_param pthread_param;
    pthread_attr_t thread_attr;

    pthread_param.sched_priority = 2;

    pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);
    pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedparam(&thread_attr, &pthread_param);

    pthread_create(&blockThread, &thread_attr, MIDI_ProcessBlock::Thread_Processing, static_cast<void*>(this));
}

void MIDI_ProcessBlock::cancel()
{
    pthread_cancel(blockThread);
    pthread_testcancel();
}

void* MIDI_ProcessBlock::Thread_Processing(void* arg)
{
    MIDI_ProcessBlock *process_block = static_cast<MIDI_ProcessBlock*>(arg);
    MIDI_Chain *chain = nullptr;
    MidiMessage msg;

    if(process_block == nullptr)
        pthread_exit(nullptr);

    if((chain = process_block->getMotherChain()) == nullptr)
        pthread_exit(nullptr);

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);


    while(1){

        //Read MIDI message from input MidiMsgBuffer
        msg = process_block->getMidiMsg();

        //Process MIDI message
        process_block->ProcessMsg(msg);

        //Write MIDI message in MIDIMsgBuffer of next MIDI_ChainBlock
        process_block->passMidiMsg(msg);
    }
}



MIDI_Monitor::MIDI_Monitor()
{

}



void MIDI_Monitor::ProcessMsg(MidiMessage& msg)
{



}

MIDI_Scale::MIDI_Scale()
{



}

void MIDI_Scale::ProcessMsg(MidiMessage& msg)
{


}
