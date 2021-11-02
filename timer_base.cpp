#include "timer_base.h"

TimerBase::TimerBase(/* args */)
{
    _remaining = _timeSet = _before = _status = _loop = 0;
}

TimerBase::TimerBase(unsigned long t, bool loop, bool start, bool triggerOnFirstCheck)
{
    set(t, loop, start, triggerOnFirstCheck);
}

TimerBase::~TimerBase()
{
}

bool TimerBase::check()
{

    if (_status < TIMER_PAUSED)
        return false;

    unsigned long n = t_now();

    if (_status == TIMER_ACTIVE)
    {

        unsigned long l = n - _before;
        if (l >= _remaining)
        {
            if (_loop)
            {
                _remaining = _timeSet;
            }
            else
            {
                _remaining = 0;
                _status = TIMER_STOPPED;
            }
            return true;
        }
        else
        {
            _remaining -= l;
        }
    }

    _before = n;
    return false;
}

void TimerBase::set(unsigned long t, bool loop, bool start, bool triggerOnFirstCheck)
{
    _timeSet = _remaining = t;
    _loop = loop;
    if (start)
    {
        _before = t_now();
        _status = TIMER_ACTIVE;
    }
    else
    {
        _status = TIMER_PAUSED;
    }
    if (triggerOnFirstCheck)
        _remaining = 0;
}

void TimerBase::setFrequency(double hertzs, bool start, bool triggerOnFristCheck)
{

    // turn freq into period and loop
    // millis resolution is 1 so max frequency is 1000Hz
    // double h = constrain(abs(hertzs), 0.0, 1000.0);
    // good idea?

    unsigned long t = frequency_to_period(hertzs);

    set(t, true, start, triggerOnFristCheck);
}

void TimerBase::start(unsigned long t, bool loop, bool triggerOnFirstCheck)
{
    set(t, loop, true, triggerOnFirstCheck);
}

// void TimerBase::start(bool triggerOnFirstCheck)
// {
// 	set(_timeSet, _loop, true, triggerOnFirstCheck);
// }
void TimerBase::start()
{
    set(_timeSet, _loop, true, false);

    // _before = t_now();
    // _status = TIMER_ACTIVE;

    // if (_status == TIMER_STOPPED)
    // {
    //     _remaining = _timeSet;
    // }
    // if (_status < TIMER_ACTIVE)
    // {
    //     _before = t_now();
    //     _status = TIMER_ACTIVE;
    // }
}

void TimerBase::startIn(unsigned long t)
{

    start();
    _remaining += t;
}
void TimerBase::stop()
{

    if ((_status == TIMER_ACTIVE) || (_status == TIMER_PAUSED))
    {

        check();
        _status = TIMER_STOPPED;
    }

    // _remaining = 0;
}

void TimerBase::pause(bool doPause)
{

    if (_status < TIMER_PAUSED)
    {
        return;
    }

    if (doPause)
    {
        if (_status == TIMER_ACTIVE)
        {
            check();
            _status = TIMER_PAUSED;
        }
    }
    else
    {
        if (_status == TIMER_PAUSED)
        {
            _before = t_now();
            _status = TIMER_ACTIVE;
        }
    }
}

void TimerBase::resume()
{
    pause(false);
}

void TimerBase::restart(bool triggerOnFirstCheck)
{
    set(_timeSet, _loop, true, triggerOnFirstCheck);
}

// void TimerBase::delay(unsigned long ms){
// 	if(_status == TIMER_ACTIVE || _status == TIMER_PAUSED){

// 	}
// }
uint8_t TimerBase::status() { return _status; }

unsigned long TimerBase::remaining()
{
    return _remaining;
}

TimerBase::operator bool()
{

    return check();
}

void TimerBase::loopMode(bool loop)
{
    _loop = loop;
}

double TimerBase::progress()
{
    return 1.0 - (double)_remaining / (double)_timeSet;
}

unsigned long TimerBase::progress100()
{
    return (unsigned long)(100.0 * progress());
    // return 100UL - 100UL * _remaining / _timeSet;
}

bool TimerBase::isActive()
{
    return _status == TIMER_ACTIVE;
}

void TimerBase::setRemaining(unsigned long remaining)
{
    _remaining = remaining;
}

void TimerBase::delay(long t)
{

    if (t >= 0)
    {
        _remaining += (unsigned long)t;
    }
    else if (abs(t) >= _remaining)
    {
        _remaining = 0;
    }
    else
    {
        _remaining -= (unsigned long)(abs(t));
    }
}
