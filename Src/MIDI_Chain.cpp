#include "MIDI_Chain.h"
#include "MIDI_IOBlock.h"

MIDI_Chain::MIDI_Chain(MIDI_IOBlock *in, MIDI_IOBlock *out) :  inputBlock(in), outputBlock(out), chainMutex(PTHREAD_MUTEX_INITIALIZER)
{
    processBlockList.push_back(static_cast<MIDI_ChainBlock*>(in));
    processBlockList.push_back(static_cast<MIDI_ChainBlock*>(out));
    in->setNextBlock(out);
    in->setMotherChain(this);
    out->setMotherChain(this);
}

MIDI_Chain::~MIDI_Chain()
{
    //dtor
}

void MIDI_Chain::insertBlock(int pos, MIDI_ChainBlock *block)
{
    if(!block)
        return;

    if(pos <= 0 || pos >= processBlockList.size())
        return;

    lockChain();

    std::list<MIDI_ChainBlock*>::iterator it = processBlockList.begin();
    std::advance(it, pos - 1);
    (*it)->setNextBlock(block);
    it++;
    block->setNextBlock(*it);
    processBlockList.insert(it, block);
    block->setMotherChain(this);

    unlockChain();
}

void MIDI_Chain::removeBlock(int pos)
{
    lockChain();

    if(pos <= 0 || pos >= processBlockList.size() - 1){
        unlockChain();
        return;
    }

    MIDI_ChainBlock *temp;
    std::list<MIDI_ChainBlock*>::iterator it = processBlockList.begin();
    std::advance(it, pos - 1);
    temp = *it;
    it++;
    temp->setNextBlock((*it)->getNextBlock());
    processBlockList.erase(it);

    unlockChain();
}

void MIDI_Chain::setInputBlock(MIDI_IOBlock *in)
{
    if(!in)
        return;

    lockChain();
    MIDI_ChainBlock *temp = inputBlock;
    inputBlock = in;
    in->setMotherChain(this);
    unlockChain();

    delete temp;
}

void MIDI_Chain::setOutputBlock(MIDI_IOBlock *out)
{
    if(!out)
        return;

    lockChain();
    MIDI_ChainBlock *temp = outputBlock;
    outputBlock = out;
    out->setMotherChain(this);
    unlockChain();

    delete temp;
}

void MIDI_Chain::lockChain()
{
    pthread_mutex_lock(&chainMutex);
}

void MIDI_Chain::unlockChain()
{
    pthread_mutex_unlock(&chainMutex);
}
