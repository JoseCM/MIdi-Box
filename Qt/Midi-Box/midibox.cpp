#include "midibox.h"

MidiBox::MidiBox(QQuickWindow *win){

    window = win;

    phys_io = new Physical_IO(I2CDEVICE, SPIDEVICE);
    uart_io = new UART_IO(UARTDEVICE);
    usb_io = new UART_IO(USBDEVICE);

    QObject::connect(window, SIGNAL(addChainSignal(int, int, int, int)), this, SLOT(addNewChain(int , int, int , int)));
    QObject::connect(window, SIGNAL(removeChainSignal(int)), this, SLOT(removeChain(int)));
    QObject::connect(window, SIGNAL(addBlockSignal(int, int, int)), this, SLOT(addBlockToChain(int, int, int)));
    QObject::connect(window, SIGNAL(removeBlockSignal(int, int)), this, SLOT(removeBlockFromChain(int, int)));

    phys_io->run();
    uart_io->run();
    usb_io->run();

}

MidiBox::~MidiBox() { }

void MidiBox::addNewChain(int input, int channel_in , int output, int channel_out){

    MIDI_InBlock *in;

    qDebug() << "input " << input << " channel " << channel_in;
    qDebug() << "output" << output << " channel" << channel_out;
    channel_in--;
    channel_out--;

    switch (input){
        case Interface:
            in = new MIDI_InBlock(channel_in, phys_io);
            break;
        case MIDI:
            in = new MIDI_InBlock(channel_in, uart_io);
            break;
        case USB:
            in = new MIDI_InBlock(channel_in, usb_io);
            break;
        case File:
            break;
    };

    MIDI_OutBlock *out;

    switch (output){
        case MIDI:
            out = new MIDI_OutBlock(channel_out, uart_io);
            break;
        case USB:
            out = new MIDI_OutBlock(channel_out, usb_io);
            break;
        case File:
            break;
    };

    MIDI_Chain *chain = new MIDI_Chain(in, out);

    chainList.push_back(chain);

    in->run();
    out->run();

    qDebug() << "Adding Chain...";
}

void MidiBox::removeChain(int index){

    std::list<MIDI_Chain*>::iterator it1 = chainList.begin();
    std::advance(it1, index);

    delete (*it1);

    chainList.erase(it1);

    qDebug() << "Removing Chain...";
}


void MidiBox::addBlockToChain(int chain, int index, int processblock){

    MIDI_ProcessBlock *block;

    std::list<MIDI_Chain*>::iterator it1 = chainList.begin();
    std::advance(it1, chain);


    switch (processblock) {
        case Monitor:
            block = new MIDI_Monitor();
            break;
        case Scale:
            block = new MIDI_Scale();
            break;
    };


    if(block){
        (*it1)->insertBlock(index, block);
        block->run();
    }

    qDebug() << "Adding block to chain...";
}


void MidiBox::removeBlockFromChain(int chain, int index){

    std::list<MIDI_Chain*>::iterator it1 = chainList.begin();
    std::advance(it1, chain);

    (*it1)->removeBlock(index);
    //thread cancelled in removeBlock

    qDebug() << "Removing block to chain...";
}
