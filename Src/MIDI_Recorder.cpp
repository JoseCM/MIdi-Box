
#include "MIDI_Recorder.h"

MIDI_Recorder::MIDI_Recorder(MIDI_Clock* clk, mqd_t queue): clock(clk), isRecording(false), queue_recorder(queue)
{


}

MIDI_Recorder::~MIDI_Recorder(){

}


void MIDI_Recorder::addChainToRecord(MIDI_Chain* chain, string fileName){

    FILE_IO *newfile = new FILE_IO(fileName);
    newfile->setTicksPerQuarterNote(96);

    std::pair<MIDI_Chain*, FILE_IO*> newpair(chain, newfile);

    filesToRecord.insert(newpair);
}

void MIDI_Recorder::removeChainFromRecord(MIDI_Chain* chain){
    map<MIDI_Chain*, FILE_IO*>::iterator it;

    it = filesToRecord.find(chain);

    if(it != filesToRecord.end()){
        //(*it).second->close(); //write midifile ("xxx.mid")
        delete (*it).second;
        filesToRecord.erase(it);
    }
}

void MIDI_Recorder::clearFilesToRecord(){
    map<MIDI_Chain*, FILE_IO*>::iterator it;

    it = filesToRecord.begin();

    if(it != filesToRecord.end()){
        (*it).second->truncate();
    }
}

void MIDI_Recorder::closeFilesToRecord(){
    map<MIDI_Chain*, FILE_IO*>::iterator it;

    it = filesToRecord.begin();

    if(it != filesToRecord.end()){
        (*it).second->close(); //write midifile ("xxx.mid")
    }
}

void MIDI_Recorder::addEventToFile(MidiEvent event, MIDI_Chain *chain){
    map<MIDI_Chain*, FILE_IO*>::iterator it;

    it = filesToRecord.find(chain);
    if(it != filesToRecord.end()){
        (*it).second->addMidiEvent(event);
    }
}

int MIDI_Recorder::getTickCount(){
    return clock->getTickCount();
}

void MIDI_Recorder::setRecordState(bool state){
    isRecording = state;
}

bool MIDI_Recorder::getRecordState(){
    return isRecording;
}

mqd_t MIDI_Recorder::getQueueRecorder(){
    return queue_recorder;
}

void* MIDI_Recorder::Thread_Record(void *arg){
    MIDI_Recorder *midiRecorder = static_cast<MIDI_Recorder*>(arg);

    messageToRecord *message_s;
    ssize_t size;
    int tick;

    while(1){
        message_s = new messageToRecord();
        size = mq_receive(midiRecorder->getQueueRecorder(), reinterpret_cast<char*>(message_s), sizeof(messageToRecord), 0);

        if( midiRecorder->getRecordState()){
            tick = midiRecorder->getTickCount();
            //printf("tick:%x\n", tick);
            if(size == sizeof(messageToRecord)){
                //printf("queue_received\n");
                midiRecorder->addEventToFile(MidiEvent(tick, 0, message_s->msg), message_s->chain);
            }
        }
    }
}

void MIDI_Recorder::run(){
    struct sched_param pthread_param;
    pthread_attr_t thread_attr;

    pthread_param.sched_priority = 3;

    pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);
    pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedparam(&thread_attr, &pthread_param);

    //pthread_create(&handle, &thread_attr, MIDI_Recorder::Thread_Record, static_cast<void*>(this));
    pthread_create(&handle, NULL, MIDI_Recorder::Thread_Record, static_cast<void*>(this));
}


