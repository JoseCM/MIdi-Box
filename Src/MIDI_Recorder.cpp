
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
        (*it).second->close(); //write midifile ("xxx.mid")
        delete (*it).second;
        filesToRecord.erase(it);
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

    struct messageToRecord{
        MIDI_Chain *chain;
        MidiMessage msg;
    };

    messageToRecord message_s;
    char *message_v;
    ssize_t size;
    int tick;

    while(1){
        size = mq_receive(midiRecorder->getQueueRecorder(), message_v, sizeof(messageToRecord), 0);

        if(midiRecorder->isRecording){
            tick = midiRecorder->getTickCount();

            if(size == sizeof(messageToRecord)){
                memcpy(&message_s, message_v, size);
                midiRecorder->addEventToFile(MidiEvent(tick, 1, message_s.msg), message_s.chain);
            }
        }
    }
}

void MIDI_Recorder::run(){
    pthread_create(&handle, NULL, MIDI_Recorder::Thread_Record, static_cast<void*>(this));
}


