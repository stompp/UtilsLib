// #include "math_utils.h"

#include "math_utils.h"

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
double positiveWave(double rangeMinus11Value, double amplitude)
{
    return (rangeMinus11Value + amplitude) / 2.0;
}

/**
 * https://www.disca.upv.es/adomenec/IASPA/tema5/FormesdOna.html
 */

double triangularWaveCycle(double x, double k)
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

double squareWaveCycle(double x, double k)
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

double triangularWave(double phase, double k)
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

double sinPulseWave(double phase, double k)
{

    double kph = TWO_PI * k;

    if ((phase > kph) || (k == 0))
    {
        return 0;
    }

    return sin(normalizedPhase(phase / (2.0 * k)));
}
double pulseWave(double phase, double k)
{

    return pulseWaveCycle(phaseToCycleValue(phase), k);
}

double rectangularWave(double phase, double k)
{
    return rectangularWaveCycle(phaseToCycleValue(phase), k);
}

double squareWave(double phase, double k)
{
    return squareWaveCycle(phaseToCycleValue(phase), k);
}

double rhomboidWave(double phase, double k)
{
    return rhomboidWaveCycle(phaseToCycleValue(phase), k);
}
