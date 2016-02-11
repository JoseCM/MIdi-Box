
#include "MIDI_Player.h"


MIDI_Player::MIDI_Player(MIDI_Clock* clk, list<MIDI_Chain*> &list): clock(clk), internalCounter(0), chainList(list)
{
    stop();
}

MIDI_Player::~MIDI_Player(){

}

void MIDI_Player::start(){
    clock->enable(false);
    clock->reset();
    internalCounter = 0;
    setPlayingState(true);

    for( auto &chain : filesToPlay ){
        chain.second.second = 0;
    }

    clock->enable(true);
    sendSysRealTimeMsg(0xFA); //system real-time message play
}

void MIDI_Player::stop(){
    clock->enable(false);
    setPlayingState(false);
    sendSysRealTimeMsg(0xFC); //system real-time message stop

    for( auto &chain : chainList ){
        MidiMessage msg( 0xB0 | chain->getOutputBlock()->getChannel() , 123 , 0);
        chain->getOutputBlock()->getIOStream()->writeOutMidiMsg(msg);

    }

    resetChainToPlay();
}

void MIDI_Player::resetChainToPlay(){
    map<MIDI_Chain*, pair<FILE_IO*, int>>::iterator it = filesToPlay.begin();

    while(it != filesToPlay.end()){
        (*it).second.first->resetIndex();
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

    map<MIDI_Chain*, pair<FILE_IO*, int>>::iterator it = filesToPlay.begin();
    int channel, tick, tickTotal = clock->getTickCount() ;

    while(it != filesToPlay.end()){
        channel = (*it).first->getInputBlock()->getChannel();
        tick = tickTotal - (*it).second.second;
        if(!(*it).second.first->getNextMidiMsg(channel, tick))
            (*it).second.second = tickTotal;
        it++;
    }
}

bool MIDI_Player::waitTickCounting(){
    internalCounter++;
    return clock->lock();
}


void MIDI_Player::addChainToPlay(MIDI_Chain* chain, FILE_IO* midiFile){
    pair<MIDI_Chain*, pair<FILE_IO*, int>> newpair(chain, pair<FILE_IO*, int>(midiFile, 0));
    filesToPlay.insert(newpair);
}

void MIDI_Player::removeChainFromPlay(MIDI_Chain* chain){
    map<MIDI_Chain*, pair<FILE_IO*, int>>::iterator it;

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
        if(midiPlayer->waitTickCounting()){

            if(midiPlayer->getPlayingState())
            {
                if((midiPlayer->getInternalCounter()%4) == 0)
                    midiPlayer->sendSysRealTimeMsg(0xF8);

                midiPlayer->sendMidiMsgFromFiles();

            }
        }
    }
}

void MIDI_Player::run(){
    pthread_create(&handle, NULL, MIDI_Player::Thread_Play, static_cast<void*>(this));
}


