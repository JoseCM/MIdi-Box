
#include "MIDI_Clock.h"


MIDI_Clock::MIDI_Clock() :
    bpm(120), tickDivision(96), tickCount(-1)

{
    struct sigaction sa;
    struct sigevent sev;

    sem_init(&timerSemaphore, 0, 0);

    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sa.sa_sigaction = MIDI_Clock::Handler_ClockTick;

    sigemptyset(&sa.sa_mask);
    sigaction(TIMER_SIG, &sa, NULL);

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = TIMER_SIG;
    sev.sigev_value.sival_ptr = static_cast<void*>(this);

    timer_create(CLOCK_REALTIME, &sev, &timerId);
}



MIDI_Clock::~MIDI_Clock()
{
    sem_destroy(&timerSemaphore);
    timer_delete(timerId);
}

void MIDI_Clock::incTickCount(int tickInc){
    tickCount += tickInc;
}

int MIDI_Clock::getTickCount() const{
    return tickCount;
}


void MIDI_Clock::setBMP(int bpm){
    this->bpm = bpm;
}

timer_t MIDI_Clock::getTimerId() const{
    return timerId;
}


void MIDI_Clock::reset(){
    tickCount = -1;
}

void MIDI_Clock::enable(bool en){

    struct itimerspec ts;
    long clock_time = 60000000000/(bpm*tickDivision);

    if(en == true){
        ts.it_interval = {0, clock_time};
        ts.it_value = {0, clock_time};
        timer_settime(timerId, 0, &ts, NULL);
    }else{
        ts.it_interval = {0, 0};
        ts.it_value = {0, 0};
        timer_settime(timerId, 0, &ts, NULL);
    }
}

bool MIDI_Clock::lock(){
    if(sem_wait(&timerSemaphore) == -1)
        return false;
    return true;
}

void MIDI_Clock::unlock(){
    sem_post(&timerSemaphore);
}


void MIDI_Clock::Handler_ClockTick(int sig, siginfo_t *si, void *uc){

    MIDI_Clock *midiClock;
    //struct timespec spec;

    if(sig == TIMER_SIG){

        midiClock = static_cast<MIDI_Clock*>(si->si_value.sival_ptr);
        midiClock->incTickCount( 1 + timer_getoverrun(midiClock->getTimerId()));
        midiClock->unlock();

        //clock_gettime(CLOCK_REALTIME, &spec);
        //qDebug() << spec.tv_nsec / 1000000.0 << endl;
    }

}
