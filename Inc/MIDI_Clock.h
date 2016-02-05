

#ifndef __MIDI_CLOCK_H__
#define __MIDI_CLOCK_H__

#include <pthread.h>
#include <signal.h>
#include <time.h>


using namespace std;


class MIDI_Clock
{
    timer_t timerId;
    int bpm;
    int tickDivision;
    int tickCount;
    sem_t timerSemaphore;









};


































#endif
