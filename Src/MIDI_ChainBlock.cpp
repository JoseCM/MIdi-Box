#include "MIDI_ChainBlock.h"
#include "MIDI_Chain.h"

MIDI_ChainBlock::MIDI_ChainBlock(MIDI_Chain* mother) : nextBlock(nullptr), motherChain(mother)
{

}

MIDI_ChainBlock::~MIDI_ChainBlock()
{

}

MIDI_ChainBlock* MIDI_ChainBlock::getNextBlock(){
    return nextBlock;
}

void MIDI_ChainBlock::setNextBlock(MIDI_ChainBlock* next)
{
    this->nextBlock  = next;
}

void MIDI_ChainBlock::setMotherChain(MIDI_Chain *chain)
{
    motherChain = chain;
}

MIDI_Chain* MIDI_ChainBlock::getMotherChain()
{
    return motherChain;
}


void MIDI_ChainBlock::passMidiMsg(MidiMessage &msg)
{
    //motherChain->lockChain();
    nextBlock->inBuffer.writeMidiMsg(msg);
    //motherChain->unlockChain();
}




