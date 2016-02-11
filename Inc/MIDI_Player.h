

#ifndef MIDI_PLAYER_H
#define MIDI_PLAYER_H

#include "MIDI_Clock.h"
#include "MIDI_Chain.h"
#include "MIDI_IOBlock.h"
#include "FILE_IO.h"

#include <map>
#include <pthread.h>


using namespace std;

class MIDI_Player{

    MIDI_Clock *clock;
    map<MIDI_Chain*, pair<FILE_IO*, int>> filesToPlay;
    list<int> tickBase;
    list<MIDI_Chain*> &chainList;
    pthread_t handle;
    bool isPlaying;
    int internalCounter;

public:
    MIDI_Player(MIDI_Clock*, list<MIDI_Chain*> &);
    ~MIDI_Player();
    void start();
    void stop();
    void sendSysRealTimeMsg(int);
    void sendMidiMsgFromFiles();
    bool waitTickCounting();
    void resetChainToPlay();
    void addChainToPlay(MIDI_Chain*, FILE_IO*);
    void removeChainFromPlay(MIDI_Chain*);
    void setPlayingState(bool);
    bool getPlayingState();
    int getInternalCounter(void);

    static void* Thread_Play(void *arg);
    void run();
};

#endif


