#include "Wave.h"


#if USE_DISCRETE_SYSTEM == 0

unsigned long Wave::_sys_time;
bool Wave::_sys_init = false;

#endif
void Wave::init()
{
#if USE_DISCRETE_SYSTEM == 1
    _n_offset = DiscreteSystem::systemLoopSample();
#else
    if (_sys_init == false)
    {
        LOOP();
        _sys_init = true;
    }
    _time_offset = _sys_time;
#endif
}
Wave::Wave()
{
    init();
}
Wave::Wave(double frequency, uint8_t waveForm, double ph0)
{

    init();

    form = waveForm;
    setFrequency(frequency);
    phase0 = ph0;

    k = 0.5f;
    
    offset = 0.0f;
    amp = 1;
    positive = false;
}
Wave::Wave(double frequency, uint8_t waveForm, double param, double ph0)
{
    init();

    form = waveForm;
    setFrequency(frequency);
    phase0 = ph0;
    k = param;

    offset = 0.0f;
    amp = 1;
    positive = false;
}
Wave::~Wave()
{
}
void Wave::LOOP()
{
#if USE_DISCRETE_SYSTEM == 1
    DiscreteSystem::tick();
#else
    #if WAVE_TIME_MODE == 0
        _sys_time = millis();
    #else
        _sys_time = micros();
    #endif
#endif
}

double Wave::_t()
{
#if USE_DISCRETE_SYSTEM == 1
    return DiscreteSystem::loopCycleValue(_n_offset);
    // return DiscreteSystem::t(_n_offset);
#else
    return ((double)(_sys_time - _time_offset)) / WAVE_TIME_SCALE;
#endif
}

double Wave::t()
{
#if USE_DISCRETE_SYSTEM == 1
    // return DiscreteSystem::loopCycleValue(_n_offset);
    return DiscreteSystem::t(_n_offset);

#else
    return ((double)(_sys_time - _time_offset)) / WAVE_TIME_SCALE;
#endif
}

void Wave::setFrequency(double frequency)
{
    w = TWO_PI * abs(frequency);
}
double Wave::getFrequency()
{
    return w / TWO_PI;
}
double Wave::_wt()
{
    return w * _t();
}

double Wave::phase()
{
    return normalizedPhase(_wt() + phase0);
}

double Wave::phaseSync(Wave other)
{
    double o_ph = other.phase();
    double ph = normalizedPhase(_wt());

    phase0 = normalizedPhase(o_ph - ph);

    return phase();
}

double Wave::value(double ph, double kk)
{

    double out = 0;

    switch (form)
    {
    case SINE:
        out = sin(ph);
        break;
    case TRIANGULAR:
        out = triangularWave(ph, kk);
        break;
    case SQUARE:
        out = squareWave(ph, kk);
        break;
    case SAWTOOTH:
        out = sawtoothWave(ph);
        break;
    case INVERSE_SAWTOOTH:
        out = inverseSawtoothWave(ph);
        break;
    case PULSE:
        out = pulseWave(ph, kk);
        break;
    case RECTANGULAR:
        out = rectangularWave(ph, kk);
        break;
    case RHOMBOIDAL:
        out = rhomboidWave(ph, kk);
        break;
    case WHITE_NOISE:
        out = ((float)random(1000)) / 1000.0;
        break;
    case SINE_PULSE:
        out = sinPulseWave(ph, kk);
        break;
    default:
        break;
    }

    out *= amp;

    if (positive)
        out = positiveWave(out, amp);
    out += offset;
    return out;
}

double Wave::value()
{

    return value(phase(), k);
}

Wave Wave::SINE_WAVE(double frequency, double initialPhase)
{
    return Wave(frequency, SINE, initialPhase);
}