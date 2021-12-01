#ifdef DISCRETE_SYSTEM_CALC_MODE
#undef DISCRETE_SYSTEM_CALC_MODE
#endif
#define DISCRETE_SYSTEM_CALC_MODE 1
#ifdef USE_DISCRETE_SYSTEM
#undef USE_DISCRETE_SYSTEM
#endif
#define USE_DISCRETE_SYSTEM 1

#include <Wave.h>



const unsigned long baudRate = 2000000;
const unsigned long sampleRate = 500;

bool doHold;
Wave* wave;


void waveSerialInput()
{

    if (Serial.available())
    {
        int c = Serial.read();
        if (c != -1)
        {

            if (c == 'f')
            {
                wave->setFrequency(Serial.parseFloat());
            }
            else if (c == 'k')
            {
                wave->k = Serial.parseFloat();
            }
            else if (c == 't')
            {
                wave->form = Serial.parseInt();
            }
              else if (c == 'a')
            {
                wave->amp = Serial.parseFloat();
            }
            else if (c == 'h')
            {
                doHold = !doHold;
              wave->hold(doHold);
            }
            // else if (mode != (char)c)
            //     mode = (char)c;
        }
    }
}

void setup()
{

    Serial.begin(baudRate);

    DiscreteSystem::setSampleRate(sampleRate);
    wave = new Wave(50.0, Wave::SINE, 0);
    wave->amp = 1000.0;
    wave->positive = true;
}

void loop()
{
    waveSerialInput();
    if (Wave::LOOP())
    {
        
        Serial.println(wave->value());
     
    }
}