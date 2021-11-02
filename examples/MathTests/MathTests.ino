#include <math_utils.h>

void setup()
{

    Serial.begin(115200);


    int x = minumunDistanceCircularMap(50,1,100,50,75,100);
    Serial.println(x);
    x = minumunDistanceCircularMap(50,1,100,75,50,100);
    Serial.println(x);

    Serial.println("done");
}

void loop()
{
}