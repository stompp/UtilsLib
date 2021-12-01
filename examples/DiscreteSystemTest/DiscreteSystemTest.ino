#include <discrete_system.h>


DiscreteSystem ds;

const unsigned long baudRate = 2000000;
const unsigned long sampleRate = 2000;
unsigned long lastMicros;



unsigned long testElapsed()
{
    unsigned long t = ds.lastMicros();
    unsigned long diff = t - lastMicros;
    lastMicros = t;
    return diff;
}
unsigned long testSampleRate()
{
    return 1000000/testElapsed();
}

void setup()
{

    Serial.begin(baudRate);
    ds.start(sampleRate);
    lastMicros = ds.lastMicros();
   
}

void loop()
{
    if (ds.tick())
    {
       Serial.println(testSampleRate());
    }

   
}