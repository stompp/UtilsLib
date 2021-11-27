#ifndef _MATH_UTILS_H_
#define _MATH_UTILS_H_

// #include <stdint.h>
#include <Arduino.h>
#include <limits.h>
#include <discrete_system.h>

// Returns decimal part of float input
double decimalPart(double x);

/** Returns the float remainder after dividing dividend by divisor
 *  e.g. floatMod(5.0, 1.5) returns 0.5
 *       floatMod(9.0, 3.5) returns 2.0
 **/
double floatMod(double dividend, double divisor);

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

/** Amplitude modulation function **/
double amplitudeModulation(double signal, double carrier, double m);

/** fake **/
double amplitudDemodulation(double modulatedSignal, double carrier, double m);

// Returns a given positive or negative phase in radians in range [0,TWO_PI)
double normalizedPhase(double radians);

// Returns a given phase in radians in range [0,1)
double phaseToCycleValue(double phase);

// Maps a wave value in range [-amplitude,amplitude] to range [0,amplitude]
double positiveWave(double rangeMinus11Value, double amplitude = 1.0);

/**
 * https://www.disca.upv.es/adomenec/IASPA/tema5/FormesdOna.html
 */

double triangularWaveCycle(double x, double k = 0.5f);

double sawtoothWaveCycle(double x);

double inverseSawtoothWaveCycle(double x);

double pulseWaveCycle(double x, double k);

double squareWaveCycle(double x, double k = 0.5f);
// doubt
double rectangularWaveCycle(double x, double k);

double rhomboidWaveCycle(double x, double k);

// PHASE BASED

double triangularWave(double phase, double k = 0.5f);

double sawtoothWave(double phase);

double inverseSawtoothWave(double phase);



double pulseWave(double phase, double k);

double rectangularWave(double phase, double k);

double squareWave(double phase, double k = 0.5);

double rhomboidWave(double phase, double k);

// mine
double sinPulseWave(double phase, double k = 0.5f);


#define USE_DISCRETE_SYSTEM 1

class Wave
{
private:

#if USE_DISCRETE_SYSTEM == 1
  unsigned long _n_offset;
#else
    static unsigned long _sys_millis;
    unsigned long _millis;
    staticbool _sys_init = false;
#endif

    void init();
    

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

    double wt();

    double phase();

    double phaseSync(Wave other);

    double value(double ph,double kk = 0.5);

    double value();


    static Wave SINE_WAVE(double frequency, double initialPhase);
};

// class ValueAnimation : public Wave
// {
// private:
//     /* data */
// public:
//     ValueAnimation : Wave(/* args */);
//     ~ValueAnimation :Wave();
// };

// ValueAnimation :Wave::ValueAnimation :Wave(/* args */)
// {
// }

// ValueAnimation :Wave::~ValueAnimation :Wave()
// {
// }

#endif