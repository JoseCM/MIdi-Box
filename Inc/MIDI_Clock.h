

#ifndef __MIDI_CLOCK_H__
#define __MIDI_CLOCK_H__

#include <semaphore.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#include <QDebug>

using namespace std;

#define TIMER_SIG  SIGRTMAX

class MIDI_Clock
{
    timer_t timerId;
    int bpm;
    int division;
    long tickDivision;
    int tickCount;
    sem_t timerSemaphore;

public:
    MIDI_Clock();
    ~MIDI_Clock();

    void setBMP(int bmp);  //confirmar
    void incTickCount(int tickInc);
    int getTickCount() const;
    timer_t getTimerId() const;
    void reset();
    void enable(bool en);
    void lock();
    void unlock();

    static void Handler_ClockTick(int sig, siginfo_t *si, void *uc);

};


































#endif
