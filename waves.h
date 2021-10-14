#ifndef _waves_h_
#define _waves_h_
#include <math_utils.h>


class WaveBase
{
private:
public:
    static const uint8_t SINE = 1;
    static const uint8_t TRIANGULAR = 2;
    static const uint8_t SQUARE = 3;
    static const uint8_t SAWTOOTH = 4;
    static const uint8_t PULSE = 5;
    static const uint8_t RECTANGULAR = 6;
    static const uint8_t RHOMBOIDAL = 7;
    static const uint8_t WHITE_NOISE = 8;

    uint8_t form;

    double freq;
    double amp;
    double phase0;
    double k;
    double offset;

    bool positive;

    static void tick();

    WaveBase();
    WaveBase(double frequency, uint8_t form, double ph0);
    WaveBase(double frequency, uint8_t form, double param, double ph0);

    ~WaveBase();

    virtual double t() = 0;

    double wt()
    {
        return TWO_PI * freq * t();
    }

    double phase()
    {
        return normalizedPhase(wt() + phase0);
    }

    double phaseSync(double other)
    {
        
      

        phase0 = normalizedPhase(other - normalizedPhase(wt()));

        return phase();
    }

    double value()
    {

        double out = 0;

        switch (form)
        {
        case SINE:
            out = sin(phase());
            break;
        case TRIANGULAR:
            out = triangularWave(phase(), k);
            break;
        case SQUARE:
            out = squareWave(phase(), k);
            break;
        case SAWTOOTH:
            out = sawtoothWave(phase());
            break;
        case PULSE:
            out = pulseWave(phase(), k);
            break;
        case RECTANGULAR:
            out = rectangularWave(phase(), k);
            break;
        case RHOMBOIDAL:
            out = rhomboidWave(phase(), k);
        default:
            break;
        }
        out *= amp;

        if (positive)
            out = positiveWave(out, amp);
        out += offset;
        return out;
    }


};



WaveBase::WaveBase()
{
}
WaveBase::WaveBase(double frequency, uint8_t waveForm, double ph0)
{

    form = waveForm;
    freq = frequency;
    phase0 = ph0;

    k = 0.5f;

    offset = 0.0f;
    amp = 1;
    positive = false;
}
WaveBase::WaveBase(double frequency, uint8_t waveForm, double param, double ph0)
{

    form = waveForm;
    freq = frequency;
    phase0 = ph0;
    k = param;

    offset = 0.0f;
    amp = 1;
    positive = false;
}
WaveBase::~WaveBase()
{
}

#endif