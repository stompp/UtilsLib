#ifndef _MATH_UTILS_H_
#define _MATH_UTILS_H_

// #include <stdint.h>
#include <ArduinoPlatforms.h>

// #include <math.h>

long valueInCircularRange(long value, long valueMax);
float fvalueInCircularRange(float value, float valueMax);
/** Get the result of rasing \c base to \c exponent */
long powLong(long base, long exponent);
/** Get the result of rasing \c base to \c exponent */
unsigned long powULong(unsigned long base, unsigned long exponent);

/** Check if \c value is power of \c base */
bool isPowerOf(long value, long base);

/** Check if \c value is power of 2 */
bool isPowerOf2(long value);

/** Check if \c x is even */
bool isEven(long x);

/** Check if \c x is odd */
bool isOdd(long x);
/** Unsigned long and long safe sum */
unsigned long ul_plus_l(unsigned long ul, long l);

/** Sign function, returns the sign -1, 1 of the value **/
float sign(float v);
/** Returns true if the value is positive **/
bool isPositive(float f);

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
 * if @param autoUpdate is true @param v will be modified as returned
 * otherwhise just the result will be returned.
 * TODO check if type salad is needed
 */
template <typename T, typename U>
T dimValue(T &value, U dif, T minV, T maxV, bool autoUpdate)
{
    T v = constrain(value, minV, maxV);

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

    if (autoUpdate)
        value = out;
    return out;
}

template <typename T, typename U>
T dimValueInCircle(T &value, U dif, T minV, T maxV, bool autoUpdate)
{
    T temp = value;
    T minD = value - minV;
    T maxD = maxV - value;

    if ((dif < 0 && (abs(dif) > (minD))))
    {
        temp = dimValue(maxV, dif + minD, minV, maxV, false);
        if (autoUpdate)
            value = temp;
    }
    else if ((dif > 0 && maxD < dif))
    {
        temp = dimValue(minV, dif - maxD, minV, maxV, false);
        if (autoUpdate)
            value = temp;
    }
    else
    {
        temp = dimValue(value, dif, minV, maxV, autoUpdate);
    }
    return temp;
}

template <typename T, typename U>
T dimValue(T &value, U dif, T minV, T maxV, bool inCircle, bool autoUpdate)
{

    return inCircle ? (dimValueInCircle(value, dif, minV, maxV, autoUpdate))
                    : dimValue(value, dif, minV, maxV, autoUpdate);
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
 */
template <typename T>
T mapT(T x, T in_min, T in_max, T out_min, T out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * Template map function. Same as map but for any type.
 */
template <typename T>
T mapT(T x, T in_min, T in_max, T out_max)
{
    return mapT(x, in_min, in_max, 0, out_max);
    // return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
/**
 * Template map function. Same as map but for any type.
 */
template <typename T>
T mapT(T x, T in_max, T out_max)
{
    return mapT(x, 0, in_max, 0, out_max);
    // return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
/**
 * Interpolates a value @param x whose original range starts at @param xStart and ends at @param xEnd,
 * into a new range @param yStart @param yEnd. Start values can be bigger than end values. Any direction interpolation.
 * You can interpolate a value in decreasing range into a value in increasing range or viceversa or anyway.
 * e.g x is 7 in range [10,0] output is in range [20,30] the result would be 23
 */
template <typename T>
T interpolate(T x, T xStart, T xEnd, T yStart, T yEnd)
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

        if (yStart < yEnd)
        {
            out = mapT(xStart - x, (T)0, xStart - xEnd, yStart, yEnd);
        }
        else if (yStart > yEnd)
        {
            d = yStart - yEnd;
            out = yEnd + d - mapT(xStart - x, (T)0, xStart - xEnd, (T)0, d);
        }
    }

    return out;
}

/**
 * Calculates the distance from @param startV to @param endV in an upwards circular way which circle max value is @param maxV
 * e.g startV is 20 endV is 80 maxV is 100 then output distance is 60
 * e.g startV is 80 endV is 20 maxV is 100 then output distance is 40
 */
template <typename T>
T circleUpwardsDistance(T _startV, T _endV, T _minV, T _maxV)
{
    T startV = _startV - _minV;
    T endV = _endV - _minV;
    T maxV = _maxV - _minV;
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

    // T a[] = {_startV, _endV, _minV, _maxV, startV, endV, maxV, s, e, dif};
    // for (int n = 0; n < 10; n++)
    // {
    //     Serial.print(n);
    //     Serial.print(" ");
    //     Serial.println(a[n]);
    // }

    return dif;
}

/**
 * Calculates the distance from @param startV to @param endV in an downwards circular way which circle max value is @param maxV
 * e.g startV is 20 endV is 80 maxV is 100 then output distance is 40
 * e.g startV is 80 endV is 20 maxV is 100 then output distance is 60
 */
template <typename T>
T circleDownwardsDistance(T _startV, T _endV, T _minV, T _maxV)
{
    T startV = _startV - _minV;
    T endV = _endV - _minV;
    T maxV = _maxV - _minV;

    T dif = (T)0;

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

    // T a[] = {_startV, _endV, _minV, _maxV, startV, endV, maxV, s, e, dif};
    // for (int n = 0; n < 10; n++)
    // {
    //     Serial.print(n);
    //     Serial.print(" ");
    //     Serial.println(a[n]);
    // }
    return dif;
}

// /**
//  * Maps a linear value @param linePosition in range [ @param lineStart, @param lineEnd] into a circular ranged
//  * value going from 0 to @param circleMaxValue starting at @param circleStartValue and ending at @param circleEndValue
//  * running the minumun distance between both.
//  * e.g linePosition is 50 from 0 to 100
//  * circleStartValue is 80 circleEndValue is 20 circleMaxValue is 99 then result is 0
//  *  circleStartValue is 80 circleEndValue is 20 circleMaxValue is 100 then result is 100
//  */
// template <typename T>
// T minumunDistanceCircularMap(T linePosition, T lineStart, T lineEnd, T circleStartValue, T circleEndValue, T circleMaxValue)
// {

//     T out = circleEndValue;

//     if (linePosition < lineEnd)
//     {

//         T dd = circleDownwardsDistance(circleStartValue, circleEndValue, circleMaxValue);
//         // Serial.println(dd);
//         T ud = circleUpwardsDistance(circleStartValue, circleEndValue, circleMaxValue);
//         // Serial.println(ud);
//         if (ud <= dd)
//         {

//             if (circleStartValue <= circleEndValue)
//             {
//                 out = mapT(linePosition, lineStart, lineEnd, circleStartValue, circleEndValue);
//             }
//             else
//                 out = floatMod((circleStartValue + (T)1 + mapT(linePosition, lineStart, lineEnd, (T)0, ud)), (circleMaxValue + 1));
//             // out = (circleStartValue + mapT(linePosition, lineStart, lineEnd, (T)0, ud)) % (circleMaxValue + 1);
//         }
//         else
//         {

//             if (circleStartValue <= circleEndValue)
//             {

//                 out = floatMod((circleEndValue + dd - mapT(linePosition, lineStart, lineEnd, (T)0, dd)), (circleMaxValue + 1));
//                 // out = (circleEndValue + dd - mapT(linePosition, lineStart, lineEnd, (T)0, dd)) % (circleMaxValue + 1);
//             }
//             else
//             {
//                 out = circleStartValue - mapT(linePosition, lineStart, lineEnd, (T)0, dd);
//             }
//         }
//     }

//     return out;
// }

/**
 * Maps a linear value @param linePosition in range [ @param lineStart, @param lineEnd] into a circular ranged
 * value going from 0 to @param circleMaxValue starting at @param circleStartValue and ending at @param circleEndValue
 * running the minumun distance between both.
 * e.g linePosition is 50 from 0 to 100
 * circleStartValue is 80 circleEndValue is 20 circleMaxValue is 99 then result is 0
 *  circleStartValue is 80 circleEndValue is 20 circleMaxValue is 100 then result is 100
 */
template <typename T>
T minumunDistanceCircularMap(T linePosition, T lineStart, T lineEnd, T _circleStartValue, T _circleEndValue, T _circleMinValue, T _circleMaxValue)
{

    T circleStartValue = _circleStartValue - _circleMinValue;
    T circleEndValue = _circleEndValue - _circleMinValue;
    T circleMaxValue = _circleMaxValue - _circleMinValue;

    T out = _circleEndValue;

    if (linePosition < lineEnd)
    {

        T dd = circleDownwardsDistance(_circleStartValue, _circleEndValue, _circleMinValue, _circleMaxValue);

        T ud = circleUpwardsDistance(_circleStartValue, _circleEndValue, _circleMinValue, _circleMaxValue);

        if (ud <= dd)
        {

            if (circleStartValue <= circleEndValue)
            {
                out = mapT(linePosition, lineStart, lineEnd, _circleStartValue, _circleEndValue);
            }
            else
                out = _circleMinValue + floatMod((circleStartValue + (T)1 + mapT(linePosition, lineStart, lineEnd, (T)0, ud)), (circleMaxValue + 1));
            // out = (circleStartValue + mapT(linePosition, lineStart, lineEnd, (T)0, ud)) % (circleMaxValue + 1);
        }
        else
        {

            if (circleStartValue <= circleEndValue)
            {

                out = _circleMinValue + floatMod((circleEndValue + dd - mapT(linePosition, lineStart, lineEnd, (T)0, dd)), (circleMaxValue + 1));
                // out = (circleEndValue + dd - mapT(linePosition, lineStart, lineEnd, (T)0, dd)) % (circleMaxValue + 1);
            }
            else
            {
                out = _circleStartValue - mapT(linePosition, lineStart, lineEnd, (T)0, dd);
            }
        }
    }

    return out;
}

template <typename T>
class Constrainer
{
protected:
    T _xMax;
    T _xMin;

public:
    Constrainer() : _xMax(0), _xMin(0)
    {
    }

    Constrainer(T a, T b = 0)
    {
        set(a, b);
    }
    T xMin()
    {
        return _xMin;
    }
    T xMax()
    {

        return _xMax;
    }
    T xWidth()
    {
        return _xMax - _xMin;
    }

    void set(T a, T b = 0)
    {
        _xMax = (a > b) ? a : b;
        _xMin = (a > b) ? b : a;
    }

    // T constrained(T value)
    // {
    //     return constrain(value, _xMin, _xMax);
    // }

    T constrained(float value)
    {
        return constrain(value, _xMin, _xMax);
    }
    bool isMax(T value)
    {
        return _xMax == constrained(value);
    }

    bool isMin(T value)
    {
        return _xMin == constrained(value);
    }

    bool isIn(T value)
    {
        return (value <= _xMax) && (value >= _xMin);
    }

    float progress(T value)
    {
        return (((float)value - (float)_xMin) / ((float)_xMax - (float)_xMin)) ;
    }
    float progress100(T value)
    {
        return progress(value) * 100.0f;
    }
    T distanceToMin(T value)
    {
        return value - _xMin;
    }
    T distanceToMax(T value)
    {
        return _xMax - value;
    }
    T progresstoValue(float prc)
    {
        float c = _xMin + ((float)(xWidth()) * prc);

        return constrained(c);
    }
    T progress100toValue(float prc)
    {
        float c = _xMin + ((float)(xWidth()) * prc) / 100.f;

       return constrained(c);
    }

    T circularProgresstoValue(float prc)
    {
        return constrainedInCircle(((float)_xMin) + ((float)(xWidth()) * prc));
    }
    T circularProgress100toValue(float prc)
    {
        return circularProgresstoValue(prc / 100.f);
    }

    // TODO Test
    // T constrainedInCircle(T value)
    // {
    //     return _xMin + (floatMod(value - _xMin, xWidth()));
    // }
    T constrainedInCircle(float value)
    {

        return _xMin + fvalueInCircularRange(value - _xMin, xWidth());
        // float dif = (floatMod(value - _xMin, xWidth()));
        // float o = dif + _xMin;
        // if (dif < 0)
        // {
        //     o += xWidth();
        // }

        // return o;
    }
    template <typename U>
    T dim(T &value, U dif, bool autoUpdate)
    {
        return dimValue(value, dif, xMin(), xMax(), autoUpdate);
    }
    template <typename U>
    T dim(T &value, U dif, bool inCircle, bool autoUpdate)
    {
        return dimValue(value, dif, xMin(), xMax(), inCircle, autoUpdate);
    }

    T getLevelValue(uint16_t level, uint16_t maxLevel)
    {
        uint16_t l = constrain(level, 0, maxLevel);
        if (l == maxLevel)
        {
            return xMax();
        }
        return xMin() + (l * (xWidth() / maxLevel));
    }
    bool isMinOrMax(T a)
    {
        return (isMin(a) || isMax(a));
    }

    bool isMinAndMax(T a, T b)
    {
        return ((isMin(a) && isMax(b)) || (isMin(b) && isMax(a)));
    }
};

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

    T linear(T &startV, T endV, unsigned long transitionTime, bool autoChangeStartV)
    {

        T out = startV;
        unsigned long t = millis();

        startCheck(startV, endV);

        if (canCalculate(startV, endV))
        {

            out = (T)interpolate((double)t, (double)startTime, (double)(startTime + transitionTime), (double)startV, (double)endV);
            if (t >= (startTime + transitionTime))
                out = endV;
        }

        endCheck(out, startV, endV, autoChangeStartV);

        return out;
    }

    T minimunDistanceCircular(T &startV, T endV, T minV, T maxV, unsigned long transitionTime, bool autoChangeStartV)
    {

        T out = startV;
        unsigned long t = millis();

        startCheck(startV, endV);

        if (this->canCalculate(startV, endV))
        {
            out = minV + (T)minumunDistanceCircularMap((double)t, (double)startTime, (double)(startTime + transitionTime), (double)(startV - minV), (double)(endV - minV), 0.0 /*(double)minV*/, (double)(maxV - minV));
        }

        endCheck(out, startV, endV, autoChangeStartV);

        return out;
    }
};

template <typename T>
class TimedInterpolator : public TimedInterpolationBase<T>
{
private:
    /* data */
public:
    T target;
    T prev;
    unsigned long durationMs;
    bool autoChangeSV = true;

    T linear()
    {
        return TimedInterpolationBase<T>::linear(prev, target, durationMs, autoChangeSV);
    }

    T minimunDistanceCircular(T minValue, T maxValue)
    {
        return TimedInterpolationBase<T>::minimunDistanceCircular(prev, target, minValue, maxValue, durationMs, autoChangeSV);
    }

    void start(T newPrev, T newTarget)
    {

        target = newTarget;
        prev = newPrev;
        TimedInterpolationBase<T>::reset();
    }
    void start(T value)
    {
        start(value, value);
    }
    void moveTo(T newTarget)
    {
        if (target != newTarget)
            start(target, newTarget);
    }
    void startReverse()
    {
        T aux = prev;
        prev = target;
        target = aux;
        TimedInterpolationBase<T>::reset();
    }
    void restart()
    {
        TimedInterpolationBase<T>::reset();
    }
    void reset()
    {
        prev = target;
        TimedInterpolationBase<T>::reset();
    }
    void reset(T value)
    {

        start(value);
    }
    void reset(T newPrev, T newTarget)
    {

        start(newPrev, newTarget);
    }

    void resetTarget(T newTarget)
    {

        TimedInterpolationBase<T>::reset();
        target = newTarget;
    }
    void resetPrev(T newPrev)
    {

        TimedInterpolationBase<T>::reset();
        prev = newPrev;
    }
    unsigned long getStartTime() { return TimedInterpolationBase<T>::startTime; }
    unsigned long getEndTime() { return TimedInterpolationBase<T>::startTime + durationMs; }

    void setDurationMs(unsigned long durationInMs)
    {
        durationMs = durationInMs;
    }
    unsigned long getDurationMs()
    {
        return durationMs;
    }
    float getProgress()
    {

        float r = ((float)millis() - (float)getStartTime()) / (float)durationMs;
        return (constrain(r, 0.0f, 1.0f));
    }

    float getProgress100()
    {
        return 100.0f*getProgress();
        
    }
    void init(unsigned long durationInMs, bool doAutoChangeStartValue)
    {

        durationMs = durationInMs;
        autoChangeSV = doAutoChangeStartValue;
    }

    void init(T initPrev, T initTarget, unsigned long durationInMs, bool doAutoChangeStartValue)
    {
        prev = initPrev;
        target = initTarget;
        init(durationInMs, doAutoChangeStartValue);
    }
    void init(T initValue, unsigned long durationInMs, bool doAutoChangeStartValue)
    {
        init(initValue, initValue, durationInMs, doAutoChangeStartValue);
    }
};

template <typename T>
class TimedValueInterpolator : public TimedInterpolator<T>
{

public:
    static const uint8_t CONSTRAINED = 0;
    static const uint8_t CIRCULAR = 1;

    uint8_t mode = CONSTRAINED;
    Constrainer<T> limits;
    bool animateCircularLimitToLimit = false;
    bool isConstrained() { return mode == CONSTRAINED; }
    bool isCircular() { return mode == CIRCULAR; }
    void setMode(uint8_t _mode)
    {
        mode = _mode;
    }
    void setMode(uint8_t _mode, Constrainer<T> &_limits)
    {
        setMode(_mode);

        setLimits(_limits);
    }
    void setLimits(Constrainer<T> &_limits)
    {
        limits = _limits;
        start(limits.xMax());
    }
    void setConstrained()
    {
        setMode(CONSTRAINED);
    }

    void setCircular()
    {
        setMode(CIRCULAR);
    }
    void setConstrained(Constrainer<T> &_limits)
    {
        setMode(CONSTRAINED, limits);
    }

    void setCircular(Constrainer<T> _limits)
    {
        setMode(CIRCULAR, limits);
    }

    T value()
    {
        // if(mode == CIRCULAR)
        if ((mode == CIRCULAR) && !(animateCircularLimitToLimit && limits.isMinAndMax(TimedInterpolator<T>::prev, TimedInterpolator<T>::target)))
        {
            return TimedInterpolator<T>::minimunDistanceCircular(limits.xMin(), limits.xMax());
        }
        return TimedInterpolator<T>::linear();
    }

    template <typename U>
    T getDimmedTarget(U &v, bool autoUpdate)
    {
        return dimValue(TimedInterpolator<T>::target, v, limits.xMin(), limits.xMax(), isCircular(), autoUpdate);
    }
    template <typename U>
    T filterValue(U v)
    {
        if (isCircular())
        {
            return limits.constrainedInCircle(v);
        }
        return limits.constrained(v);
    }
    T progressToValue(float progress)
    {
        return isCircular() ? limits.circularProgresstoValue(progress)
                            : limits.progresstoValue(progress);
    }
};
template <typename T>
class TimedLinearInterpolator : public TimedInterpolator<T>
{

public:
    T value()
    {
        return TimedInterpolator<T>::linear();
    }
};

// t
template <typename T>
class TimedMinimunDistanceCircularInterpolator : public TimedInterpolator<T>
{

public:
    T minValue = 0;
    T maxValue;
    T value()
    {
        return TimedInterpolator<T>::minimunDistanceCircular(minValue, maxValue);
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

float gap_calc(float maxV, float levels);

#endif