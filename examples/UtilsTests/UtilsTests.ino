#include <arduino_utils.h>

float  n = 0.5;
void setup(){

    Serial.begin(115200);

}


void loop(){

    printValue(F("Hola"),n);
    delay(1000);
    n++;

}