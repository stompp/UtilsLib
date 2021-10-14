#include <millis_utils.h>

unsigned long millisToRestart = 1000000;
MillisTimer printOutputTimer;
MillisChronometer chronometer;

void setup()
{

    Serial.begin(9600);
    printOutputTimer.start(500, true);
    chronometer.start();

}

void loop()
{
    readInput();
    chronometer.update();

    if (printOutputTimer)
    {

     
        Serial.println(chronometer.value());

        
        printChronoTimeFor(chronometer.value(), &Serial);
        Serial.println();

        if (chronometer.value() > millisToRestart)
        {
            chronometer.restart();
        }
    }
}

void readInput()
{

    if (Serial.available())
    {

        char c = (char)Serial.read();

        if (c == 's')
        {
            chronometer.start();
        }
        else if (c == 'e')
        {
            chronometer.reset();
        }
        else if (c == 'p')
        {
            chronometer.pause();
        }
        else if (c == 'r')
        {

            chronometer.resume();
        }
        else if (c == 'o')
        {
            chronometer.stop();
        }
    }
}