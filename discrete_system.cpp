#include "discrete_system.h"

DiscreteSystem::DiscreteSystem(/* args */)
{
}

DiscreteSystem::~DiscreteSystem()
{
}

bool DiscreteSystem::tick()
{

    unsigned long t = micros();
    unsigned long ellapsed = t - _last_micros;

    bool newSample = false;
    unsigned long _prev_n = _n;

    if (ellapsed >= _remaining_micros)
    {

#if DISCRETE_SYSTEM_CALC_MODE == 1
        unsigned long delay = ellapsed - _remaining_micros;
        unsigned long overSamples = delay / _micros_period;
        // unsigned long overLapDistance = ULONG_MAX - _n;
        unsigned long sampleInc = 1 + overSamples;

        // if (overLapDistance <= sampleInc)
        // {
        //     _n = systemLoopSample();
        // }

        _n += sampleInc;
        delay -= (overSamples * _micros_period);
        _remaining_micros = _micros_period - delay;

#elif DISCRETE_SYSTEM_CALC_MODE == 0

        // if (_n == ULONG_MAX)
        //     _n = systemLoopSample();
        _n++;

        // if (_n == _sampleRate)
        // {
        //     _n = 0;
        //     _seconds++;
        // }

        _remaining_micros = _micros_period;
#endif

        newSample = true;

        _n %= _sampleRate;
        if (_n < _prev_n)
        {
            _seconds++;
        }
    }
    else
    {
        _remaining_micros -= ellapsed;
    }

    _last_micros = t;

    return newSample;
}

unsigned long DiscreteSystem::systemSample()
{
    return _n + (_seconds * _sampleRate);
}
unsigned long DiscreteSystem::systemLoopSample()
{

    // return (_n % ((unsigned long)_sampleRate));
    return _n;
}

double DiscreteSystem::t()
{
    return double(systemSample())/(double(_sampleRate));
}
double DiscreteSystem::t(unsigned long sampleOffset)
{
     return double(systemSample() +sampleOffset)/(double(_sampleRate));
}

double DiscreteSystem::systemCycleValue()
{
    return calcCycleValue(systemLoopSample());
}

unsigned long DiscreteSystem::loopSample(unsigned long sampleOffset)
{

    return (systemLoopSample() + sampleOffset) % (_sampleRate);
}

double DiscreteSystem::calcCycleValue(unsigned long sampleIndex)
{

    return ((double)sampleIndex) / ((double)_sampleRate);
}

double DiscreteSystem::calcPhase(unsigned long sampleIndex)
{
    return TWO_PI * calcCycleValue(sampleIndex);
}

double DiscreteSystem::loopCycleValue(unsigned long sampleOffset)
{
    return calcCycleValue(loopSample(sampleOffset));
}
double DiscreteSystem::loopPhase(unsigned long sampleOffset)
{
    return calcPhase(loopSample(sampleOffset));
}
void DiscreteSystem::setSampleRate(unsigned long sampleRate)
{

    _sampleRate = sampleRate;
    _micros_period = 1000000.0 / _sampleRate;
}

// void DiscreteSystem::setSamplePeriod(double samplePeriod)
// {
//     _sampleRate = 1.0 / samplePeriod;
//     _micros_period = 1000000.0 * samplePeriod;
// }

void DiscreteSystem::start()
{
    _n = 0;
    _remaining_micros = _micros_period;
    _last_micros = micros();
}

void DiscreteSystem::start(unsigned long sampleRate)
{
    setSampleRate(sampleRate);
    start();
}
unsigned long DiscreteSystem::_last_micros;
unsigned long DiscreteSystem::_remaining_micros;
unsigned long DiscreteSystem::_micros_period;
unsigned long DiscreteSystem::_sampleRate;

unsigned long DiscreteSystem::_seconds = 0;
unsigned long DiscreteSystem::_n = 0;
