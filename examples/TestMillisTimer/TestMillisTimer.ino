#include <millis_utils.h>

MillisTimer t1, delayTimer;

char str[40];
void setup()
{

    Serial.begin(9600);

    // t1.set(20000, true);
    delayTimer.start(1000, true);

    t1.start(2000,true);
}

void loop()
{
    readInput();
    if (delayTimer)
    {

            t1.check();
            int status = t1.status();
            snprintf(str, 40, "T: %lu  S: %u ", t1.remaining(),t1.status());
            Serial.println(str);
        

      
    }
}

void readInput()
{

    if (Serial.available())
    {

        char c = (char)Serial.read();

        if (c == 's')
        {
            t1.start();
        }
        else if (c == 't')
        {
            t1.start((unsigned long)Serial.parseInt());
        }
        else if (c == 'p')
        {
            t1.pause();
        }
        else if (c == 'r')
        {
           

            t1.resume();
        }
        else if (c == 'o')
        {
            t1.stop();
        }
    }
}