// #include "math_utils.h"

#include "math_utils.h"


/** Get the result of rasing \c base to \c exponent */
unsigned long pow2(uint8_t  exponent){
//	long result = 1;
//	while(exponent-- > 0) result*=base;
//	return result;
	return bit(exponent);
}
/** Get the result of rasing \c base to \c exponent */
long powLong(long base, long exponent){
//	long result = 1;
//	while(exponent-- > 0) result*=base;
//	return result;
	return (exponent == 0) ? 1: base*powLong(base,--exponent);
}
/** Get the result of rasing \c base to \c exponent */
unsigned long powULong(unsigned long base, unsigned long exponent){

//	unsigned long result = 1;
//	while(exponent-- > 0) result*=base;
//	return result;

	return (exponent == 0) ? 1: base*powLong(base,--exponent);
}

/** Check if \c value is power of \c base */
bool isPowerOf(long value,long base){
	// zero is power of no number
	if ((value == 0) || (base == 1)) return false;
	// one is power of every number raised to 0
	if(value == 1) return true;
	return (value%base == 0 ? isPowerOf(value/base,base) : false);
}

/** Check if \c value is power of 2 */
bool isPowerOf2(long value){return (value != 0) && ((value & (value - 1)) == 0);}

/** Check if \c x is even */
bool isEven(long x){return x%2 == 0;}

/** Check if \c x is odd */
bool isOdd(long x){return x%2 > 0;}

unsigned long ul_plus_l(unsigned long ul, long l){


	return (l < 0) ? (ul - (unsigned long)(-l)) : (ul + (unsigned long)l) ;
}

/** Sign function, returns the sign -1, 1 of the value **/
float sign(float v) { return v < 0.0 ? -1.0 : 1.0; }
/** Returns true if the value is positive **/
bool isPositive(float f) { return f >= 0.0; }
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
    return (rangeMinus11Value + amplitude) / 2.0*amplitude;
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
    double nph = normalizedPhase(phase);
    double kph = TWO_PI * k;

    if ((nph> kph) || (k == 0))
    {
        return 0;
    }

    return sin((nph / (2.0 * k)));
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
