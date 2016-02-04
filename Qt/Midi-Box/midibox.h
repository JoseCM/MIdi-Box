#ifndef MIDIBOX_H
#define MIDIBOX_H


#include <QList>
#include <QQuickWindow>
#include <QQmlComponent>
#include <QObject>
#include <QDebug>
#include <QQuickItem>
#include <QQmlEngine>


class MidiBox
{

private:
    QList<QObject*> chainList;
    QQuickWindow *window;


public:
    MidiBox(QQuickWindow *win, QQmlEngine * engine);
    ~MidiBox();
    void addNewChain();

};

#endif // MIDIBOX_H
