#include <discrete_system.h>
#include <math_utils.h>
Wave w;
DiscreteSystem ds;
const unsigned long baudRate = 2000000;
const unsigned long sampleRate = 1000;
unsigned long lastMicros;
float freq = 20;

uint8_t sample_n = 0;
uint8_t n_samples = 100;
uint8_t samples[100];

void addSample(uint8_t v)
{
    samples[sample_n++] = v;

    // sample_n++;
    if (sample_n == n_samples)
    {
        for (uint8_t n = 0; n < n_samples; n++)
        {
            Serial.println(samples[n]);
        }
        sample_n = 0;
    }
}
unsigned long testElapsed()
{
    unsigned long t = micros();
    unsigned long diff = t - lastMicros;
    lastMicros = t;
    return diff;
}

float wt()
{
    float t = ((float)millis()) / 1000.0;
    return TWO_PI * freq * t;
}
void setup()
{

    Serial.begin(baudRate);

    ds.start(sampleRate);
    lastMicros = micros();
    // Wave::tick();
    w = Wave(freq, Wave::SINE, 0);
    w.amp = 255.0;
}

void loop()
{
    // Serial.println(255*sin(w.freq*TWO_PI*(((float)millis())/1000.0)));
    // ds.tick();
    //  Serial.println(w.value());
    if (ds.tick())
    {
           Serial.println(w.value());
        // Serial.println(ds.t());
    }

    if (Serial.available())
    {
        int c = Serial.read();
        if (c == 'f')
        {
            // freq = Serial.parseFloat();
            w.freq = Serial.parseFloat();
        }
    }
}