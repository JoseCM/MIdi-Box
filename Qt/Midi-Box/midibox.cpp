#include "midibox.h"

MidiBox::MidiBox(QQuickWindow *win, QQmlEngine * engine)
{

    window = win;

    /*
    QQuickItem *qmlItem1 = qobject_cast<QQuickItem*>(window->findChild<QObject*>("chain1"));
    QQuickItem *qmlItem2 = qobject_cast<QQuickItem*>(window->findChild<QObject*>("chain2"));

    qDebug() << qmlItem1->parent() << "  " << qmlItem1->parentItem() << endl;
    qDebug() << qmlItem2->parent() << "  " << qmlItem2->parentItem() << endl;

    */

    QQuickItem *qmlItem = qobject_cast<QQuickItem*>(window->findChild<QObject*>("chainColumns"));


    QQmlComponent component(engine, QUrl("./../MIdi-Box/Qt/Midi-Box/Chain.qml"));
    QQuickItem *object = qobject_cast<QQuickItem*>(component.create());

    QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership); //object owned by c++ code (not destroyed by garbage collector - java script)

    object->setProperty("color", "black");
    object->setProperty("Layout.row", 0);

    object->setParentItem(qmlItem); //visual parent of the item
    object->setParent(qmlItem);

    //qDebug() << object->parent() << "  " << object->parentItem() << endl;


    QQmlComponent component2(engine, QUrl("./../MIdi-Box/Qt/Midi-Box/Chain.qml"));
    QQuickItem *object2 = qobject_cast<QQuickItem*>(component2.create());

    QQmlEngine::setObjectOwnership(object2, QQmlEngine::CppOwnership); //object owned by c++ code (not destroyed by garbage collector - java script)

    object2->setProperty("color", "blue");
    object2->setProperty("Layout.row", 3);

    object2->setParentItem(qmlItem); //visual parent of the item
    object2->setParent(qmlItem);

    //qDebug() << object2->parent() << "  " << object2->parentItem() << endl;

}

MidiBox::~MidiBox()
{




}
