#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLibraryInfo>
#include <midibox.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    MidiBox *midi_box;

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QQuickWindow *window = qobject_cast<QQuickWindow *>(engine.rootObjects().at(0));
    window->show();

    midi_box = new MidiBox(window);

    return app.exec();
}
