

#ifndef MIDI_RECORDER_H
#define MIDI_RECORDER_H

#include "MIDI_Clock.h"
#include "MIDI_Chain.h"
#include "MIDI_IOBlock.h"
#include "FILE_IO.h"
#include <string>
#include <map>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

using namespace std;

class MIDI_Recorder{

    MIDI_Clock *clock;
    map<MIDI_Chain*, FILE_IO*> filesToRecord;
    pthread_t handle;
    bool isRecording;
    mqd_t queue_recorder;

public:
    MIDI_Recorder(MIDI_Clock*, mqd_t);
    ~MIDI_Recorder();
    void addChainToRecord(MIDI_Chain*, string fileName);
    void removeChainFromRecord(MIDI_Chain*);
    void addEventToFile(MidiEvent , MIDI_Chain *);
    void clearFilesToRecord();
    void closeFilesToRecord();
    int getTickCount();
    void setRecordState(bool);
    bool getRecordState();
    mqd_t getQueueRecorder();

    static void* Thread_Record(void *arg);
    void run();
};

#endif


