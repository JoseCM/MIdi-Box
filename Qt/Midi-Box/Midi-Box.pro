TEMPLATE = app

QT += qml quick opengl
CONFIG += c++11

SOURCES += main.cpp \
    midibox.cpp \
    ../../Src/UART_IO.cpp \
    ../../Src/TrellisI2C.cpp \
    ../../Src/Physical_IO.cpp \
    ../../Src/MIDI_ProcessBlock.cpp \
    ../../Src/MIDI_MsgBuffer.cpp \
    ../../Src/MIDI_IOBlock.cpp \
    ../../Src/MIDI_IO.cpp \
    ../../Src/MIDI_Clock.cpp \
    ../../Src/MIDI_ChainBlock.cpp \
    ../../Src/MIDI_Chain.cpp \
    ../../Src/FILE_IO.cpp \
    ../../Src/gpio.c \
    ../../Src/library/Options.cpp \
    ../../Src/library/MidiMessage.cpp \
    ../../Src/library/MidiFile.cpp \
    ../../Src/library/MidiEventList.cpp \
    ../../Src/library/MidiEvent.cpp \
    ../../Src/library/Binasc.cpp

RESOURCES += qml.qrc

TARGET = Midi-Box
    target.files=Midi-Box
    target.path = /root
INSTALLS += target

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    midibox.h \
    ../../Inc/UART_IO.h \
    ../../Inc/TrellisI2C.h \
    ../../Inc/Physical_IO.h \
    ../../Inc/MIDI_ProcessBlock.h \
    ../../Inc/MIDI_MsgBuffer.h \
    ../../Inc/MIDI_IOBlock.h \
    ../../Inc/MIDI_IO.h \
    ../../Inc/MIDI_Clock.h \
    ../../Inc/MIDI_ChainBlock.h \
    ../../Inc/MIDI_Chain.h \
    ../../Inc/gpio.h \
    ../../Inc/FILE_IO.h \
    ../../Inc/library/Options.h \
    ../../Inc/library/MidiMessage.h \
    ../../Inc/library/MidiFile.h \
    ../../Inc/library/MidiEventList.h \
    ../../Inc/library/MidiEvent.h \
    ../../Inc/library/Binasc.h

DISTFILES +=

INCLUDEPATH += ../../Inc ../../Inc/library
