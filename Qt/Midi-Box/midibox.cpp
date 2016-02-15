#include "midibox.h"


MidiBox::MidiBox(QQuickWindow *win, QQmlEngine *eng) : window(win), engine(eng)
{
    struct mq_attr attr;
    attr.mq_maxmsg = 1000;
    attr.mq_msgsize = sizeof(messageToRecord);

    queue_recorder = mq_open("/recorderqueue", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG , &attr);
    if(queue_recorder == (mqd_t) -1)
        perror("queue:");

    phys_io = new Physical_IO(I2CDEVICE, SPIDEVICE);
    uart_io = new UART_IO(UARTDEVICE);
    usb_io = new UART_IO(USBDEVICE);

    midi_clock = new MIDI_Clock();
    player = new MIDI_Player(midi_clock, chainList);
    recorder = new MIDI_Recorder(midi_clock, queue_recorder);

    QObject::connect(window, SIGNAL(addChainSignal(int, int, int, int, QString)), this, SLOT(addNewChain(int , int, int , int, QString)));
    QObject::connect(window, SIGNAL(removeChainSignal(int)), this, SLOT(removeChain(int)));
    QObject::connect(window, SIGNAL(addBlockSignal(int, int, int)), this, SLOT(addBlockToChain(int, int, int)));
    QObject::connect(window, SIGNAL(removeBlockSignal(int, int)), this, SLOT(removeBlockFromChain(int, int)));
    QObject::connect(window, SIGNAL(octaveUp()), this, SLOT(octaveUp()));
    QObject::connect(window, SIGNAL(octaveDown()), this, SLOT(octaveDown()));
    QObject::connect(window, SIGNAL(bpm(int)), this, SLOT(setBPM(int)));
    QObject::connect(window, SIGNAL(play()), this, SLOT(play()));
    QObject::connect(window, SIGNAL(stop()), this, SLOT(stop()));
    QObject::connect(window, SIGNAL(armChain(int, QString)), this, SLOT(armChain(int, QString)));
    QObject::connect(window, SIGNAL(disarmChain(int)), this, SLOT(disarmChain(int)));
    QObject::connect(window, SIGNAL(updateFileModel()), this, SLOT(updateFileModel()));

    phys_io->run();
    uart_io->run();
    usb_io->run();
    player->run();
    recorder->run();

}

MidiBox::~MidiBox() {
    mq_close(queue_recorder);
    mq_unlink("recorderqueue");
}

void MidiBox::addNewChain(int input, int channel_in , int output, int channel_out, QString filename){

    MIDI_InBlock *in;
    FILE_IO *file;

    qDebug() << "input " << input << " channel " << channel_in;
    qDebug() << "output" << output << " channel" << channel_out;
    qDebug() << "filename " << filename;
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
            file = new FILE_IO( filename.toStdString() );
            in = new MIDI_InBlock(channel_in, file);
            break;
    };

    MIDI_OutBlock *out;

    switch (output){
        case MIDI:
            out = new MIDI_OutBlock(channel_out, uart_io, queue_recorder);
            break;
        case USB:
            out = new MIDI_OutBlock(channel_out, usb_io, queue_recorder);
            break;
    };

    MIDI_Chain *chain = new MIDI_Chain(in, out);

    chainList.push_back(chain);

    if(input == File)
        player->addChainToPlay(chain, file);

    in->run();
    out->run();

    qDebug() << "Adding Chain...";
}

void MidiBox::removeChain(int index){

    std::list<MIDI_Chain*>::iterator it1 = chainList.begin();
    std::advance(it1, index);

        qDebug() << "Removing Chain...";
    if(dynamic_cast<FILE_IO*>((*it1)->getInputBlock()->getIOStream())){
        player->removeChainFromPlay(*it1);
    }

        qDebug() << "Removing Chain...";
    if((*it1)->getRecordingState()){
        recorder->removeChainFromRecord(*it1);
    }

        qDebug() << "Removing Chain...";
    delete (*it1);
        qDebug() << "Removing Chain...";
    chainList.erase(it1);

    qDebug() << "Removing Chain...";

}


void MidiBox::addBlockToChain(int chain, int index, int processblock){

    MIDI_ProcessBlock *block;
    //MonitorModel *new_monitor;
    //QQuickItem *parentItem, *childItem;
    //QQmlComponent *component;

    switch (processblock) {
        case Monitor:
            //parentItem = qobject_cast<QQuickItem*>(window->findChild<QObject*>("chainColumns"));
            //component = new QQmlComponent(engine, QUrl("Block.qml"));
            //childItem = qobject_cast<QQuickItem*>(component->create());
            //delete component;
            //QQmlEngine::setObjectOwnership(childItem, QQmlEngine::CppOwnership);
            //childItem->setProperty("model", )


            //new_monitor = new MonitorModel();

            block = new MIDI_Monitor();

            break;
        case Scale:
            block = new MIDI_Scale();
            break;
    };


    std::list<MIDI_Chain*>::iterator it1 = chainList.begin();
    std::advance(it1, chain);

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

    qDebug() << "Removing to chain...";

}

void MidiBox::octaveUp(){
    phys_io->upOctave();
}

void MidiBox::octaveDown(){
    phys_io->downOctave();
}

void MidiBox::setBPM(int bpm){
    midi_clock->setBMP(bpm);
}

void MidiBox::play(){
    recorder->clearFilesToRecord();
    recorder->setRecordState(true);
    player->start();
    qDebug() << "play" << endl;
}


void MidiBox::stop(){
    player->stop();
    recorder->setRecordState(false);
    recorder->closeFilesToRecord();
    qDebug() << "stop" << endl;
}

void MidiBox::armChain(int chain, QString filename){
    std::list<MIDI_Chain*>::iterator it1 = chainList.begin();
    std::advance(it1, chain);

    (*it1)->setRecordingState(true);
    recorder->addChainToRecord(*it1, filename.toStdString());
    qDebug() << "chain " << chain << "armed" << endl;
}

void MidiBox::disarmChain(int chain){
    std::list<MIDI_Chain*>::iterator it1 = chainList.begin();
    std::advance(it1, chain);

    (*it1)->setRecordingState(false);
    recorder->removeChainFromRecord(*it1);
    qDebug() << "chain " << chain << "unarmed" << endl;
}

void MidiBox::updateFileModel(){

    QDirIterator it("./MIDIFiles");

    qDebug() << "updating files..";
    fileModel.clear();

    while(it.hasNext()){
        it.next();
        if(it.fileName().at(0) != '.')
            fileModel << it.fileName();
    }

    QObject *obj = window->findChild<QObject*>("fileView");
    obj->setProperty("model", fileModel);

}

