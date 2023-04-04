#ifndef _timer_base_h_
#define _timer_base_h_


#include <ArduinoPlatforms.h>




#define TIMER_NOT_SET 0
#define TIMER_STOPPED 1
#define TIMER_PAUSED 2
#define TIMER_ACTIVE 3

// static const uint8_t TIMER_NOT_SET = 0;
// static const uint8_t TIMER_STOPPED = 1;
// static const uint8_t TIMER_PAUSED = 2;
// static const uint8_t TIMER_ACTIVE = 3;

class TimerBase
{
protected:
    unsigned long _before;
    unsigned long _remaining;
    unsigned long _timeSet;
     bool _loop;
    uint8_t _status;


    virtual unsigned long t_now() = 0;
    virtual unsigned long frequency_to_period(double hertzs) = 0;

public:
    TimerBase();
    TimerBase(unsigned long t, bool loop = false, bool start = false, bool triggerOnFirstCheck = false);
    ~TimerBase();

    bool check();

    void set(unsigned long t, bool loop = false, bool start = false, bool triggerOnFirstCheck = false);

    void setFrequency(double hertzs, bool start = false, bool triggerOnFristCheck = false);

    void start(unsigned long t, bool loop = false, bool triggerOnFirstCheck = false);

    // void start(bool triggerOnFirstCheck);

    void start();
    void startIn(unsigned long t);


    void stop();
    void pause(bool doPause = true);
    void resume();

    void restart(bool triggerOnFirstCheck = false);
    
    // void delay(unsigned long ms);

    unsigned long remaining();

    uint8_t status();
    operator bool();

    void loopMode(bool loop);

    double progress();

    unsigned long progress100();

    bool isActive();

    void setRemaining(unsigned long remaining);

    void delay(long t);
};

#endif