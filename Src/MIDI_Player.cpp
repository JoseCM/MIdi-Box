
#include "MIDI_Player.h"


MIDI_Player::MIDI_Player(MIDI_Clock* clk): clock(clk), internalCounter(0)
{
}

MIDI_Player::~MIDI_Player(){

}

void MIDI_Player::start(){
    clock->reset();
    clock->enable(true);
    setPlayingState(true);
    internalCounter = 0;
    sendSysRealTimeMsg(0xFA); //system real-time message play
}

void MIDI_Player::stop(){
    clock->enable(false);
    setPlayingState(false);
    sendSysRealTimeMsg(0xFC); //system real-time message stop
    resetChainToPlay();

}

void MIDI_Player::resetChainToPlay(){
    map<MIDI_Chain*, FILE_IO*>::iterator it = filesToPlay.begin();

    while(it != filesToPlay.end()){
        (*it).second->resetIndex();
        it++;
    }

}

void MIDI_Player::sendSysRealTimeMsg(int command){

    MidiMessage msg(command);

    list<MIDI_Chain*>::iterator it = chainList.begin();

    while(it != chainList.end()){
        (*it)->getOutputBlock()->getIOStream()->writeOutMidiMsg(msg);
        it++;
    }
}

void MIDI_Player::sendMidiMsgFromFiles(){

    map<MIDI_Chain*, FILE_IO*>::iterator it = filesToPlay.begin();
    int channel, tick;

    while(it != filesToPlay.end()){
        channel = (*it).first->getInputBlock()->getChannel();
        tick = clock->getTickCount();
        (*it).second->getNextMidiMsg(channel, tick);
        it++;
    }
}

void MIDI_Player::waitTickCounting(){
    clock->lock();
    internalCounter++;
}

void MIDI_Player::addChainToList(MIDI_Chain* chain){
    chainList.push_front(chain);
}

void MIDI_Player::removeChainFromList(MIDI_Chain* chain){
    list<MIDI_Chain*>::iterator it = chainList.begin();

    while(it != chainList.end()){
        if(*it == chain){
            chainList.erase(it);
            break;
        }
        it++;
    }
}

void MIDI_Player::addChainToPlay(MIDI_Chain* chain, FILE_IO* midiFile){
    pair<MIDI_Chain*, FILE_IO*> newpair(chain, midiFile);
    filesToPlay.insert(newpair);
}

void MIDI_Player::removeChainFromPlay(MIDI_Chain* chain){
    map<MIDI_Chain*, FILE_IO*>::iterator it;

    it = filesToPlay.find(chain);

    if(it != filesToPlay.end()){
        filesToPlay.erase(it);
    }
}

void MIDI_Player::setPlayingState(bool state){
    isPlaying = state;
}

bool MIDI_Player::getPlayingState(){
    return isPlaying;
}


int MIDI_Player::getInternalCounter(void){
    return internalCounter;
}

void* MIDI_Player::Thread_Play(void *arg){
    MIDI_Player *midiPlayer = static_cast<MIDI_Player*>(arg);

    while(1)
    {
        midiPlayer->waitTickCounting();

        if(midiPlayer->getPlayingState())
        {
            if((midiPlayer->getInternalCounter()%4) == 0)
                midiPlayer->sendSysRealTimeMsg(0xF8);

            midiPlayer->sendMidiMsgFromFiles();
        }
    }
}

void MIDI_Player::run(){
    pthread_create(&handle, NULL, MIDI_Player::Thread_Play, static_cast<void*>(this));
}


