#ifndef MIDIBOX_H
#define MIDIBOX_H


#include <QList>
#include <QDebug>
#include <QObject>
#include <QQuickWindow>
#include <QQuickItem>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QDirIterator>

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
#include "MIDI_Player.h"
#include "MIDI_Recorder.h"
#include "monitormodel.h"


enum MIDI_IO_TYPE {MIDI, USB, Interface, File };
enum MIDI_PROCESS_TYPE {Monitor, Scale};

class MidiBox: public QObject
{
    Q_OBJECT

private:
    Physical_IO	*phys_io;
    UART_IO *uart_io;
    UART_IO *usb_io;

    MIDI_Clock *midi_clock;
    MIDI_Player *player;
    MIDI_Recorder *recorder;

    list<MIDI_Chain*> chainList;
    mqd_t queue_recorder;

    QQuickWindow *window;
    QQmlEngine *engine;
    QStringList fileModel;

    list<pair<pair<int, int>,  MonitorModel*>> monitorList;

public:
    MidiBox(QQuickWindow *win, QQmlEngine *eng);
    ~MidiBox();

public slots:
    void addNewChain(int input, int channel_in , int output, int channel_out, QString filename);
    void removeChain(int index);
    void addBlockToChain(int chain, int index, int processblock);
    void removeBlockFromChain(int chain, int index);
    void octaveUp();
    void octaveDown();
    void setBPM(int bpm);
    void play();
    void stop();
    void armChain(int, QString);
    void disarmChain(int);
    void updateFileModel();
};

#endif // MIDIBOX_H
