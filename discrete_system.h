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

    static bool tick();
    static unsigned long systemSample();
    static unsigned long systemLoopSample();

    static double systemCycleValue();

    static unsigned long loopSample(unsigned long sampleOffset);

    static double calcCycleValue(unsigned long sampleIndex);

    static double calcPhase(unsigned long sampleIndex);

   
    static double loopCycleValue(unsigned long sampleOffset);
    static double loopPhase(unsigned long sampleOffset);
    static void setSampleRate(double sampleRate);

    static void setSamplePeriod(double samplePeriod);

    static void start();

    static void start(double sampleRate);
};

#endif