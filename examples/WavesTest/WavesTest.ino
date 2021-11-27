
#include <millis_utils.h>
#include <math_utils.h>

MillisTimer waveTimer, sampleTimer;
bool doPrint = true;
int cicle = 0;
int maxCicles = 1000;
int waveForm = 0;
const unsigned long sampleRate = 500;
unsigned long lastTick = 0;
Wave waveA;
Wave waveB;

double out;
char mode = 'c';
double amp = 255.0;
MillisTimer phaseSyncTimer;

Wave carrier, signal;

double m = 1;

unsigned int n = 0;
#define MAX_SAMPLES 500
unsigned int maxSamples = MAX_SAMPLES;
double samples[MAX_SAMPLES];
MillisTimer readSerialTimer;
void waves()
{

    // if (readSerialTimer)
        wavesSerialInput();

    // if (sampleTimer)
    // {

    unsigned long lastSample = DiscreteSystem::systemSample();

    // DiscreteSystem::tick();
    if (DiscreteSystem::tick())
    {
        unsigned long now_tick = micros();
        out = 0;
        switch (mode)
        {
        case 'a':
            out = waveA.value();
            break;
        case 'b':
            out = waveB.value();
            break;
        case 's':
            out = waveA.value() + waveB.value();
            break;
        case 'p':
            out = waveA.value() * waveB.value();
            break;
        case 'd':
            out = waveA.value() - waveB.value();
            break;
        case 'f':
            out = waveA.value();
            break;
        default:
            break;
        }

        Serial.println(now_tick - lastTick);
        lastTick = now_tick;

        // if (mode == 'f')
        // {
        //     unsigned long currentSample = DiscreteSystem::systemSample();
        //     unsigned long sampleDiff = currentSample - lastSample;
        //     Serial.println(sampleDiff);

        // }
        // else 
        // {
        //     // Serial.print(amp * waveB.value(), 2);
        //     // Serial.print(',');
        //     Serial.println(amp * out, 2);
        // }
    }
}

void waveTests2()
{

    if (n == maxSamples)
    {
        int m = 0;
        for (m = 0; m < maxSamples; m++)
        {
            Serial.println(samples[m]);
        }
        n = 0;
        while (1)
            ;
    }
    else if (DiscreteSystem::tick())
    {
        // Wave::tick();
        samples[n] = waveA.value();
        n++;
    }
}

int wavesSerialInput()
{

    if(Serial.available()){
        int c = Serial.read();
        if (c != -1)
        {

            if (c == 'h')
            {
                waveA.setFrequency(Serial.parseFloat());
          
            }else if(c == 'k'){
                waveA.k = Serial.parseFloat();
            }else if(c == 't'){
                waveA.form = Serial.parseInt();
            }else if(c == 'm'){
                  mode = (char)Serial.read(); 
            }
            // else if (mode != (char)c)
            //     mode = (char)c;
        }
    }
}

void modulationTest()
{

    int c = Serial.read();
    if (c == 'm')
    {
        m = Serial.parseFloat();
    }

    if (sampleTimer)
    {
        Wave::LOOP();
        double out = amplitudeModulation(signal.value(), carrier.value(), m);

        Serial.print(amplitudDemodulation(out, carrier.value(), m));
        Serial.print(',');
        Serial.println(out);
    }
}

void discreteSystemTest()
{

    // if(DiscreteSystem::tick()){
    //     Serial.println(DiscreteSystem::systemLoopSample());
    // }

    DiscreteSystem::tick();
    Serial.println(DiscreteSystem::systemLoopSample());
}
void setup()
{

    Serial.begin(115200);

    // Serial.println("{!!}");
    // sampleTimer.start(5, true, true);
    // waveTimer.start(1000, true);
    // phaseSyncTimer.start(5000);

    DiscreteSystem::start(sampleRate);
    lastTick = micros();

    // Wave::tick();

    waveA = Wave(20, Wave::SAWTOOTH, 0);
    waveB = Wave(1, Wave::PULSE, 0);

    // carrier = Wave(100, Wave::SINE, 0);
    // signal = Wave(2, Wave::TRIANGULAR, 0);

    // delay(200);
    readSerialTimer.setFrequency(1, true, true);
}

void loop()
{
    // waveTests2();
    waves();
    // ciclesTests();
    // modulationTest();
    // discretTests();
    // discreteSystemTest();
}

void ciclesTests()
{

    if (sampleTimer)
    {

        if (doPrint)
        {
            // float out = triangularWave(waveTimer.progress(), 0.1);
            float out;

            switch (waveForm)
            {
            case 0:
                out = triangularWaveCycle(waveTimer.progress(), 0.1);
                break;
            case 1:
                out = squareWaveCycle(waveTimer.progress());
                break;
            case 2:

                out = sawtoothWaveCycle(waveTimer.progress());
                break;
            case 3:
                out = rectangularWaveCycle(waveTimer.progress(), 0.4);

                break;
            case 4:
                out = rhomboidWaveCycle(waveTimer.progress(), 0.1);
                break;

            default:
                break;
            }

            Serial.print(((float)cicle) + waveTimer.progress(), 2);
            Serial.print(',');
            Serial.println(255.0 * out);
        }
        doPrint = waveTimer.isActive();
    }

    if (waveTimer.check())
    {
        cicle++;
        if (((cicle % 2) == 0) && cicle > 0)
        {
            waveForm++;
            waveForm %= 5;
        }

        if (cicle == maxCicles)
            waveTimer.stop();
    }
}

void discretTests()
{

    // if (sampleTimer)
    // {

    if (doPrint)
    {
        // float out = triangularWave(waveTimer.progress(), 0.1);
        float out;

        double c = DiscreteSystem::systemCycleValue();

        out = triangularWaveCycle(c, 0.5);

        // switch (waveForm)
        // {
        // case 0:
        //     out = triangularWaveCycle(DiscreteSystem::systemCycleValue(), 0.1);
        //     break;
        // case 1:
        //     out = squareWaveCycle(DiscreteSystem::systemCycleValue());
        //     break;
        // case 2:

        //     out = sawtoothWaveCycle(DiscreteSystem::systemCycleValue());
        //     break;
        // case 3:
        //     out = rectangularWaveCycle(DiscreteSystem::systemCycleValue(), 0.4);

        //     break;
        // case 4:
        //     out = rhomboidWaveCycle(DiscreteSystem::systemCycleValue(), 0.1);
        //     break;

        // default:
        //     break;
        // }

        // Serial.print(((float)cicle) + waveTimer.progress(), 4);
        // Serial.print(',');
        // Serial.print(((float)cicle) + c, 4);
        Serial.print(255.0 * c, 4);
        Serial.print(',');
        Serial.println((255.0 + 255.0 * out)) / 2.0;
    }
    // doPrint = waveTimer.isActive();
    // }

    if (waveTimer)
    {
        cicle++;
        if (((cicle % 2) == 0) && cicle > 0)
        {
            waveForm++;
            waveForm %= 5;
        }

        if (cicle == maxCicles)
            waveTimer.stop();
    }

    DiscreteSystem::tick();
}