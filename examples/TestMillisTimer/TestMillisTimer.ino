#include <millis_utils.h>

MillisTimer t1, printTimer;

char str[100];
void setup()
{

    Serial.begin(115200);

    printTimer.setFrequency(4, true, true);
    t1.start(2000, true);
    
}

void loop()
{
    readInput();
    if (printTimer)
    {

        t1.check();
        int status = t1.status();

        Serial.print("Reamining (ms) : ");
        Serial.print(t1.remaining());
        Serial.print(" Status : ");
        Serial.println(t1.status());

        // sprintf(str,"Remaining: %lu  Status: %u ", t1.remaining(),t1.status());
        // Serial.println(str);
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