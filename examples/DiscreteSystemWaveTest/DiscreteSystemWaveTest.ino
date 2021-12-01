#ifdef DISCRETE_SYSTEM_CALC_MODE
#undef DISCRETE_SYSTEM_CALC_MODE
#endif
#define DISCRETE_SYSTEM_CALC_MODE 1
#ifdef USE_DISCRETE_SYSTEM
#undef USE_DISCRETE_SYSTEM
#endif
#define USE_DISCRETE_SYSTEM 1

#include <Wave.h>

DiscreteSystem ds;

const unsigned long baudRate = 2000000;
const unsigned long sampleRate = 500;
unsigned long lastMicros;

Wave wave;
Wave wave2;
Wave wave3;
double waveValue;

unsigned long testElapsed()
{
    unsigned long t = ds.lastMicros();
    unsigned long diff = t - lastMicros;
    lastMicros = t;
    return diff;
}
unsigned long testSampleRate()
{
    return 1000000 / testElapsed();
}

void setup()
{

    Serial.begin(baudRate);
    ds.start(sampleRate);
    lastMicros = ds.lastMicros();
    wave = Wave(50.0, Wave::SINE, 0);
    wave.amp = 1000.0;
    wave.positive = true;

    wave2 = Wave(5.0, Wave::SINE, 0);
    wave2.amp = 1000.0;
    wave2.positive = true;
    wave3 = Wave(100.0, Wave::SAWTOOTH, 0);
    wave3.amp = 1000.0;
    wave3.positive = true;
}

void loop()
{
    if (ds.tick())
    {
        // waveValue = wave.value();
        //   Serial.println(wave.value());
        Serial.print(wave.value());
        Serial.print(',');
        Serial.print(wave2.value());
        Serial.print(',');
        Serial.print(wave3.value());
        Serial.print(',');
        Serial.println(testSampleRate());
    }
}