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
#include "MIDI_Chain.h"
#include "MIDI_IOBlock.h"
#include "MIDI_ProcessBlock.h"


enum MIDI_IO_TYPE {MIDI, USB, Interface, File };
enum MIDI_PROCESS_TYPE {Monitor, Scale};

class MidiBox: public QObject
{

    Q_OBJECT

private:
    QQuickWindow *window;

    Physical_IO	*phys_io;
    UART_IO *uart_io;
    UART_IO *usb_io;

    list<MIDI_Chain*> chainList;


public:
    MidiBox(QQuickWindow *win);
    ~MidiBox();

public slots:
    void addNewChain(int input, int channel_in , int output, int channel_out);
    void removeChain(int index);
    void addBlockToChain(int chain, int index, int processblock);
    void removeBlockFromChain(int chain, int index);

};

#endif // MIDIBOX_H
