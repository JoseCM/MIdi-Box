#ifndef MIDIBOX_H
#define MIDIBOX_H


#include <QList>
#include <QDebug>
#include <QObject>
#include <QQuickWindow>

#include <pthread.h>
#include <unistd.h>

#include <list>

#include "MIDI_IO.h"
#include "Physical_IO.h"
#include "UART_IO.h"
#include "FILE_IO.h"
#include "MIDI_Chain.h"
#include "MIDI_IOBlock.h"
#include "MIDI_ProcessBlock.h"
#include "MIDI_Clock.h"


enum MIDI_IO_TYPE {Interface, MIDI, USB, File};
enum MIDI_PROCESS_TYPE {Monitor, Scale};

class MidiBox: public QObject
{

private:
    Physical_IO	*phys_io;
    UART_IO *uart_io;
    UART_IO *usb_io;
    MIDI_Clock *midi_clock;


    QQuickWindow *window;
    list<MIDI_Chain*> chainList;
    mqd_t queue_recorder;

public:
    MidiBox(QQuickWindow *win);
    ~MidiBox();
    void addNewChain(MIDI_IO_TYPE input, int channel_in , MIDI_IO_TYPE output, int channel_out);
    void removeChain(int index);
    void addBlockToChain(int chain, int index, MIDI_PROCESS_TYPE processblock);
    void removeBlockFromChain(int chain, int index);

    //ChainToRecord Register/Unregister
    //Play On/Off
    //set bpm


};

#endif // MIDIBOX_H
