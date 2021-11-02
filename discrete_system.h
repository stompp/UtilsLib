#ifndef _DISCRETE_SYSTEM_H_
#define _DISCRETE_SYSTEM_H_

#include <Arduino.h>
#include <limits.h>

#define DISCRETE_SYSTEM_CALC_MODE 0

class DiscreteSystem
{
private:

    static unsigned long _last_micros;
    static unsigned long _remaining_micros;
    static unsigned long _micros_period;

    static unsigned long _sampleRate;

    static unsigned long _n;

    static unsigned long _seconds;


    static double _cycle_value;
    static double _phase;

    /* data */
public:
    DiscreteSystem(/* args */);
    ~DiscreteSystem();

    static bool tick()
    {

        unsigned long t = micros();
        unsigned long ellapsed = t - _last_micros;

        bool newSample = false;
        unsigned long _prev_n = _n;

        if (ellapsed >= _remaining_micros)
        {

#if DISCRETE_SYSTEM_CALC_MODE == 1
            unsigned long delay = ellapsed - _remaining_micros;
            unsigned long overSamples = ellapsed / _micros_period;
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
            if(_n < _prev_n){
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

    static unsigned long systemSample()
    {
        return _n + _seconds*_sampleRate;
    }
    static unsigned long systemLoopSample()
    {

        // return (_n % ((unsigned long)_sampleRate));
        return _n;
    }

    static double systemCycleValue()
    {
        return calcCycleValue(systemLoopSample());
    }

    static unsigned long loopSample(unsigned long sampleOffset)
    {

        return (systemLoopSample() + sampleOffset) % (_sampleRate);
    }

    static double calcCycleValue(unsigned long sampleIndex)
    {

        return ((double)sampleIndex) / ((double)_sampleRate);
    }

    static double calcPhase(unsigned long sampleIndex)
    {
        return TWO_PI * calcCycleValue(sampleIndex);
    }

   
    static double loopCycleValue(unsigned long sampleOffset)
    {
        return calcCycleValue(loopSample(sampleOffset));
    }
    static double loopPhase(unsigned long sampleOffset)
    {
        return calcPhase(loopSample(sampleOffset));
    }
    static void setSampleRate(double sampleRate)
    {

        _sampleRate = sampleRate;
        _micros_period = 1000000.0 / _sampleRate;
    }

    static void setSamplePeriod(double samplePeriod)
    {
        _sampleRate = 1.0 / samplePeriod;
        _micros_period = 1000000.0 * samplePeriod;
    }

    static void start()
    {
        _n = 0;
        _remaining_micros = (unsigned long)_micros_period;
        _last_micros = micros();
    }

    static void start(double sampleRate)
    {
        setSampleRate(sampleRate);
        start();
    }
};

DiscreteSystem::DiscreteSystem(/* args */)
{
}

DiscreteSystem::~DiscreteSystem()
{
}

unsigned long DiscreteSystem::_last_micros;
unsigned long DiscreteSystem::_remaining_micros;
unsigned long DiscreteSystem::_micros_period;
unsigned long DiscreteSystem::_sampleRate;

unsigned long DiscreteSystem::_seconds = 0;
unsigned long DiscreteSystem::_n = 0;

#endif