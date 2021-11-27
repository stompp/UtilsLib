#ifndef _WAVE_H_
#define _WAVE_H_
#include <math_utils.h>
#include <discrete_system.h>

#define USE_DISCRETE_SYSTEM 0
#define WAVE_TIME_MODE 0

#if WAVE_TIME_MODE == 0
#define WAVE_TIME_SCALE 1000.0
#elif WAVE_TIME_MODE == 1
#define WAVE_TIME_SCALE 1000000.0
#endif
class Wave
{
private:
#if USE_DISCRETE_SYSTEM == 1
    unsigned long _n_offset;
#else
    static unsigned long _sys_time;

    static bool _sys_init;
    unsigned long _time_offset;
#endif

    void init();

    double _t();

    double _wt();

public:
    static const uint8_t NONE = 0;
    static const uint8_t SINE = 1;
    static const uint8_t TRIANGULAR = 2;
    static const uint8_t SQUARE = 3;
    static const uint8_t SAWTOOTH = 4;
    static const uint8_t INVERSE_SAWTOOTH = 5;
    static const uint8_t PULSE = 6;
    static const uint8_t RECTANGULAR = 7;
    static const uint8_t RHOMBOIDAL = 8;
    static const uint8_t WHITE_NOISE = 9;
    static const uint8_t SINE_PULSE = 10;

    uint8_t form;
    // double freq;
    double amp;
    double w;
    double phase0;
    double k;
    double offset;
    bool positive;

    static void LOOP();

    Wave();
    Wave(double frequency, uint8_t form, double ph0);
    Wave(double frequency, uint8_t form, double param, double ph0);

    ~Wave();

    void setFrequency(double frequency);
    double getFrequency();
    double t();
    double phase();

    double phaseSync(Wave other);

    double value(double ph, double kk = 0.5);

    double value();

    static Wave SINE_WAVE(double frequency, double initialPhase);
};


#endif