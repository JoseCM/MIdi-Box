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

  //  pthread_create(&blockThread, &thread_attr, MIDI_ProcessBlock::Thread_Processing, static_cast<void*>(this));
    pthread_create(&blockThread, NULL, MIDI_ProcessBlock::Thread_Processing, static_cast<void*>(this));
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

MIDI_Monitor::MIDI_Monitor(MonitorModel *monitor): monitorModel(monitor)
{


}

MIDI_Monitor::~MIDI_Monitor(){

}

void MIDI_Monitor::ProcessMsg(MidiMessage& msg)
{
    int data;
    string message, channel, data1, data2;

    if(msg.getSize() < 1) return;

    //System Common Messages and System Real-Time Messages
    if((msg[0] & 0xF0) == 0xF0) return;

    data = msg.getChannel();
    if(data != -1)
        channel = to_string(data);
    else
        channel = " ";

    //Channel Voice Messages
    switch(msg.getCommandNibble()){
        case 0x80:
            message = "Note Off"; break;
        case 0x90:
            message = "Note On"; break;
        case 0xA0:
            message = "After Touch"; break;
        case 0xB0:
            message = "Control Change"; break;
        case 0xC0:
            message = "Patch Change"; break;
        case 0xD0:
            message = "Channel Pressure"; break;
        case 0xE0:
            message = "Pitch Bend Change"; break;
    }

    data = msg.getP1() ;
    if(data != -1)
        data1 = to_string(data);
    else
        data1 = " ";

    data = msg.getP2();
    if(data != -1)
        data2 = to_string(data);
    else
        data2 = " ";

    monitorModel->addMessageToMonitor(channel, message, data1, data2);

}


MIDI_Scale::MIDI_Scale()
{



}


void MIDI_Scale::ProcessMsg(MidiMessage& msg)
{


}
