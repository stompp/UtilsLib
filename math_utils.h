#ifndef _MATH_UTILS_H_
#define _MATH_UTILS_H_

// #include <stdint.h>
#include <Arduino.h>
#include <limits.h>
#include <discrete_system.h>

// Returns decimal part of float input
double decimalPart(double x)
{
    return x - floor(x);
}

/** Returns the float remainder after dividing dividend by divisor
 *  e.g. floatMod(5.0, 1.5) returns 0.5
 *       floatMod(9.0, 3.5) returns 2.0
 **/
double floatMod(double dividend, double divisor)
{

    long n = dividend / divisor;
    return dividend - ((double)n) * divisor;
}

/**
 * Modify a value @param v in @param dif quantity
 * but always keeping in range @param min @param max.
 * if @param autoUpdate is true @param v will be modified and returned
 * otherwhise just the result will be returned.
 * /TODO check if type salad is needed
 */
template <typename T, typename U>
T dimValue(T &value, U dif, T minV, T maxV, bool autoUpdate = true)
{
    T v = constrain(value,minV,maxV);

    T out = v;
    if (dif < 0 && (abs(dif) > (v - minV)))
        out = minV;
    else if (dif < 0)
    {
        out = v - (T)(abs(dif));
    }
    else if (((T)dif) >= (maxV - v))
        out = maxV;
    else
        out = v + (T)dif;
        // out = constrain(v + (T)dif, minV, maxV);

    if (autoUpdate)
        value = out;
    return out;
}

// template <typename T, typename U, typename V, typename W>
// T dimValue(T &v, U dif, V minV, W maxV, bool autoUpdate = true)
// {
//     T out;

//     if (dif < 0 && (abs(dif) > (v - minV)))
//         out = minV;
//     else
//         out = constrain(v + dif, minV, maxV);

//     if (autoUpdate)
//         v = out;
//     return out;
// }

/**
 * Template map function. Same as map but for any type.
 *
 */
template <typename T>
T mapT(T x, T in_min, T in_max, T out_min, T out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// template <typename T>
// T mapFromStartToEnd(unsigned long x, unsigned long xStart, unsigned long xEnd, T yStart, T yEnd)
// {

//     T out = yEnd;
//     T d;

//     if (xStart < xEnd)
//     {
//         if (yStart < yEnd)
//         {

//             d = yEnd - yStart;
//             out = yStart + map(x, xStart, xEnd, 0, d);
//         }
//         else if (yStart > yEnd)
//         {
//             d = yStart - yEnd;

//             out = yEnd + d - map(x, xStart, xEnd, 0, d);
//         }
//     }
//     else if (xStart > xEnd)
//     {

//         out = mapFromStartToEnd(yStart - x, 0, xStart - xEnd, yStart, yEnd);
//     }

//     return out;
// }

/**
 * Maps a value @param x whose original range starts at @param xStart and ends at @param xEnd,
 * to @param yStart @param yEnd. Start values can be bigger than end values. Any direction mapping.
 * You can map a value in decreasing range into a value in increasing range or viceversa or both increasing or decreasing.
 * e.g x is 7 in range [10,0] output is in range [20,30] the result would be 23
 * Maybe I have a countdown or decreament and I want this to be mapped to an increasement or any combination.
 */
template <typename T>
T mapFromStartToEnd(T x, T xStart, T xEnd, T yStart, T yEnd)
{

    T out = yEnd;
    T d;

    if (xStart < xEnd)
    {
        if (yStart < yEnd)
        {

            d = yEnd - yStart;
            out = yStart + mapT(x, xStart, xEnd, (T)0, d);
        }
        else if (yStart > yEnd)
        {
            d = yStart - yEnd;

            out = yEnd + d - mapT(x, xStart, xEnd, (T)0, d);
        }
    }
    else if (xStart > xEnd)
    {

        if (yStart < yEnd){
            out = mapT(xStart - x, (T)0,  xStart - xEnd ,yStart,yEnd);
        }else if(yStart > yEnd){
            d = yStart - yEnd;
            out = yEnd + d - mapT(xStart - x, (T)0,  xStart - xEnd, (T)0, d);
        }

            // out = mapFromStartToEnd(xStart - x, (T)0, xStart - xEnd, yStart, yEnd);
    }

    return out;
}
/**
 * Calculates the distance from @param startV to @param endV in an upwards circular way which circle max value is @param maxV
 * e.g startV is 20 endV is 80 maxV is 100 then output distance is 60
 * e.g startV is 80 endV is 20 maxV is 100 then output distance is 40
 */
template <typename T>
T circleUpwardsDistance(T startV, T endV, T maxV)
{
    T dif = 0;
    // T s = startV % (maxV + 1);
    // T e = endV % (maxV + 1);
    T s = (T)floatMod(startV, (maxV + 1));
    T e = (T)floatMod(endV, (maxV + 1));
    if (e >= s)
    {
        dif = e - s;
    }
    else
    {
        // dif = (maxV + (T)1) - s + e;
        dif = maxV + e - s;
    }
    return dif;
}

/**
 * Calculates the distance from @param startV to @param endV in an downwards circular way which circle max value is @param maxV
 * e.g startV is 20 endV is 80 maxV is 100 then output distance is 40
 * e.g startV is 80 endV is 20 maxV is 100 then output distance is 60
 */
template <typename T>
T circleDownwardsDistance(T startV, T endV, T maxV)
{
    T dif = 0;
    // T s = startV % (maxV + 1);
    // T e = endV % (maxV + 1);
    T s = (T)floatMod(startV, (maxV + 1));
    T e = (T)floatMod(endV, (maxV + 1));
    if (s >= e)
    {
        dif = s - e;
    }
    else
    {
        // dif = maxV + (T)1 -e + s ;
        dif = maxV + s - e;
    }
    return dif;
}

/**
 * Maps a linear value @param linePosition in range [ @param lineStart, @param lineEnd] into a circular ranged
 * value going from 0 to @param circleMaxValue starting at @param circleStartValue and ending at @param circleEndValue
 * running the minumun distance between both.
 * e.g linePosition is 50 from 0 to 100
 * circleStartValue is 80 circleEndValue is 20 circleMaxValue is 99 then result is 0
 *  circleStartValue is 80 circleEndValue is 20 circleMaxValue is 100 then result is 100
 */
template <typename T>
T minumunDistanceCircularMap(T linePosition, T lineStart, T lineEnd, T circleStartValue, T circleEndValue, T circleMaxValue)
{

    T out = circleEndValue;

    if (linePosition < lineEnd)
    {

        T dd = circleDownwardsDistance(circleStartValue, circleEndValue, circleMaxValue);
        // Serial.println(dd);
        T ud = circleUpwardsDistance(circleStartValue, circleEndValue, circleMaxValue);
        // Serial.println(ud);
        if (ud <= dd)
        {

            if (circleStartValue <= circleEndValue)
            {
                out = mapT(linePosition, lineStart, lineEnd, circleStartValue, circleEndValue);
            }
            else
                out = floatMod((circleStartValue + (T)1 + mapT(linePosition, lineStart, lineEnd, (T)0, ud)), (circleMaxValue + 1));
            // out = (circleStartValue + mapT(linePosition, lineStart, lineEnd, (T)0, ud)) % (circleMaxValue + 1);
        }
        else
        {

            if (circleStartValue <= circleEndValue)
            {

                out = floatMod((circleEndValue + dd - mapT(linePosition, lineStart, lineEnd, (T)0, dd)), (circleMaxValue + 1));
                // out = (circleEndValue + dd - mapT(linePosition, lineStart, lineEnd, (T)0, dd)) % (circleMaxValue + 1);
            }
            else
            {
                out = circleStartValue - mapT(linePosition, lineStart, lineEnd, (T)0, dd);
            }
        }
    }

    return out;
}

// uint16_t minumunDistanceCircularMap(unsigned long linePosition, unsigned long lineStart, unsigned long lineEnd, uint16_t startValue, uint16_t circleEndValue, uint16_t maxValue)
// {

//     uint16_t out = endValue;

//     if (linePosition < lineEnd)
//     {

//         uint16_t dd = circleDownwardsDistance(startValue, endValue, maxValue);
//         uint16_t ud = circleUpwardsDistance(startValue, endValue, maxValue);

//         if (ud <= dd)
//         {

//             out = (startValue + map(linePosition, lineStart, lineEnd, 0, ud)) % (maxValue + 1);
//         }
//         else
//         {

//             if (startValue <= endValue)
//             {

//                 out = (endValue + dd - map(linePosition, lineStart, lineEnd, 0, dd)) % (maxValue + 1);
//             }
//             else
//             {
//                 out = startValue - map(linePosition, lineStart, lineEnd, 0, dd);
//             }
//         }
//     }

//     return out;
// }

template <typename T>
class TimedInterpolationBase
{
private:
    /* data */
public:
    bool active = false;
    unsigned long startTime = 0;

    TimedInterpolationBase(/* args */)
    {
        active = false;
        startTime = 0;
    }

    void reset()
    {
        active = false;
    }

    void startCheck(T startV, T endV)
    {
        if (!active && startV != endV)
        {
            active = true;
            startTime = millis();
        }
    }

    void endCheck(T out, T &startV, T endV, bool autoChangeStartV)
    {
        if (out == endV)
        {
            active = false;
            if (autoChangeStartV)
                startV = endV;
        }
    }

    bool canCalculate(T startV, T endV)
    {
        return (active && (startV != endV));
    }
};

template <typename T>
class TimedLinearInterpolation : public TimedInterpolationBase<T>
{

public:
    T value(T &startV, T endV, unsigned long transitionTime, bool autoChangeStartV = true)
    {

        T out = startV;
        unsigned long t = millis();

        this->startCheck(startV, endV);

        if (this->canCalculate(startV, endV))
        {

            out = (T)mapFromStartToEnd((double)t, (double)this->startTime, (double)(this->startTime + transitionTime), (double)startV, (double)endV);
            if(t >= (this->startTime + transitionTime))
                out = endV;
        }

        this->endCheck(out, startV, endV, autoChangeStartV);

        return out;
    }
};

template <typename T>
class TimedMinimunDistanceCircularInterpolation : public TimedInterpolationBase<T>
{

public:
    T value(T &startV, T endV, T maxV, unsigned long transitionTime, bool autoChangeStartV = true)
    {

        T out = startV;
        unsigned long t = millis();

        this->startCheck(startV, endV);

        if (this->canCalculate(startV, endV))
        {
            out = (T)minumunDistanceCircularMap((double)t, (double)this->startTime, (double)(this->startTime + transitionTime), (double)startV, (double)endV, (double)maxV);
        }

        this->endCheck(out, startV, endV, autoChangeStartV);

        return out;
    }
};

double amplitudeModulation(double signal, double carrier, double m)
{

    return (1.0 + m * signal) * carrier;
}

/** fake **/
double amplitudDemodulation(double modulatedSignal, double carrier, double m)
{

    return ((modulatedSignal / carrier) - 1.0) / m;
}

// Returns a given positive or negative phase in radians in range [0,TWO_PI)
double normalizedPhase(double radians)
{

    double n = floatMod(radians, TWO_PI);

    // double n = radians / TWO_PI;
    // n -= (long)n;
    // n *= TWO_PI;

    if (n < 0)
        n += TWO_PI;

    return n;
}
// Returns a given phase in radians in range [0,1)
double phaseToCycleValue(double phase)
{
    return normalizedPhase(phase) / TWO_PI;
}
// Maps a wave value in range [-amplitude,amplitude] to range [0,amplitude]
double positiveWave(double rangeMinus11Value, double amplitude = 1.0)
{
    return (rangeMinus11Value + amplitude) / 2.0;
}

/**
 * https://www.disca.upv.es/adomenec/IASPA/tema5/FormesdOna.html
 */

double triangularWaveCycle(double x, double k = 0.5f)
{

    if (x < (1 - k) / 2)
        return 2 * x / (1 - k);
    else if (((1 - k) / 2 <= x) && (x < (1 + k) / 2))
        return (1 - 2 * x) / k;
    // else if ((1 + k) / 2 <= x)
    return 2 * (x - 1) / (1 - k);
}

double sawtoothWaveCycle(double x)
{

    return 1 - 2 * x;
}

double inverseSawtoothWaveCycle(double x)
{
    return -sawtoothWaveCycle(x);
}

double pulseWaveCycle(double x, double k)
{
    if (x < k)
        return 1.0;
    // else
    return 0;
}

double squareWaveCycle(double x, double k = 0.5f)
{

    if (x < k)
        return 1.0;
    // else
    return -1.0;
}

// doubt
double rectangularWaveCycle(double x, double k)
{
    // if (x <= z || (z == 1.0f))
    //     return 1.0;
    // else
    //     return (z / (z - 1.0));
    if (x < k)
        return 1.0;
    // else
    return (k / (k - 1.0));
}

double rhomboidWaveCycle(double x, double k)
{
    double z = 0.5;
    if (x < (z * k))
        return x / (z * k);
    else if ((z * k <= x) && (x < z * (1 - k)))
        return 1.0;
    else if ((z * (1 - k) <= x) && (x < z * (1 + k)))
        return (z * (1 + k) - x) / (z * k) - 1;
    else if ((z * (1 + k) <= x) && (x < (1 - k * z)))
        return -1.0;
    // else if ((1 - z * k) <= x)
    return (x - 1) / (z * k);
}

// PHASE BASED

double triangularWave(double phase, double k = 0.5f)
{
    return triangularWaveCycle(phaseToCycleValue(phase), k);
}

double sawtoothWave(double phase)
{

    return sawtoothWaveCycle(phaseToCycleValue(phase));
}

double inverseSawtoothWave(double phase)
{
    return -sawtoothWave(phase);
}

double pulseWave(double phase, double k)
{

    return pulseWaveCycle(phaseToCycleValue(phase), k);
}

double rectangularWave(double phase, double k)
{
    return rectangularWaveCycle(phaseToCycleValue(phase), k);
}

double squareWave(double phase, double k = 0.5)
{
    return squareWaveCycle(phaseToCycleValue(phase), k);
}

double rhomboidWave(double phase, double k)
{
    return rhomboidWaveCycle(phaseToCycleValue(phase), k);
}

// class WaveGenerator
// {
// private:
//     static unsigned long _sys_millis;

//     static double sys_time()
//     {
//         return ((double)_sys_millis) / 1000.0;
//     }
//     /* data */
// public:
//     WaveGenerator(/* args */);
//     ~WaveGenerator();
// };

// WaveGenerator::WaveGenerator(/* args */)
// {
// }

// WaveGenerator::~WaveGenerator()
// {
// }
#define USE_DISCRETE_SYSTEM 1

class Wave
{
private:
    static unsigned long _sys_millis;

    unsigned long _millis;

    unsigned long _n_offset;

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

    Wave();
    Wave(double frequency, uint8_t form, double ph0);
    Wave(double frequency, uint8_t form, double param, double ph0);

    ~Wave();

    double t()
    {
#if USE_DISCRETE_SYSTEM == 1
        return DiscreteSystem::loopCycleValue(_n_offset);
#else
        return ((double)(_sys_millis - _millis)) / 1000.0;
#endif
    }

    double wt()
    {
        return TWO_PI * freq * t();
    }

    double phase()
    {
        return normalizedPhase(wt() + phase0);
    }

    double phaseSync(Wave other)
    {
        double o_ph = other.phase();
        double ph = normalizedPhase(wt());

        phase0 = normalizedPhase(o_ph - ph);

        return phase();
    }

    double value(double ph)
    {

        double out = 0;

        switch (form)
        {
        case SINE:
            out = sin(ph);
            break;
        case TRIANGULAR:
            out = triangularWave(ph, k);
            break;
        case SQUARE:
            out = squareWave(ph, k);
            break;
        case SAWTOOTH:
            out = sawtoothWave(ph);
            break;
        case PULSE:
            out = pulseWave(ph, k);
            break;
        case RECTANGULAR:
            out = rectangularWave(ph, k);
            break;
        case RHOMBOIDAL:
            out = rhomboidWave(ph, k);
        default:
            break;
        }

        out *= amp;

        if (positive)
            out = positiveWave(out, amp);
        out += offset;
        return out;
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

    static Wave SINE_WAVE(double frequency, double initialPhase)
    {
        return Wave(frequency, SINE, initialPhase);
    }
};

unsigned long Wave::_sys_millis;

Wave::Wave()
{

    _millis = _sys_millis;

    _n_offset = DiscreteSystem::systemLoopSample();
}
Wave::Wave(double frequency, uint8_t waveForm, double ph0)
{
    _millis = _sys_millis;

    _n_offset = DiscreteSystem::systemLoopSample();

    form = waveForm;
    freq = frequency;
    phase0 = ph0;

    k = 0.5f;

    offset = 0.0f;
    amp = 1;
    positive = false;
}
Wave::Wave(double frequency, uint8_t waveForm, double param, double ph0)
{
    _millis = _sys_millis;

    _n_offset = DiscreteSystem::systemLoopSample();

    form = waveForm;
    freq = frequency;
    phase0 = ph0;
    k = param;

    offset = 0.0f;
    amp = 1;
    positive = false;
}
Wave::~Wave()
{
}
void Wave::tick()
{
#if USE_DISCRETE_SYSTEM == 1
    DiscreteSystem::tick();
#else
    _sys_millis = millis();
#endif
}
#endif