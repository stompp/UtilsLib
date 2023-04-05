#include <arduino_utils.h>

#include <SoftwareSerial.h>
SoftwareSerial softwareSerial(D3,D4);

#define A_BAUDRATE 115200
#define B_BAUDRATE 9600
#define SerialA Serial
#define SerialB softwareSerial


void setup()
{
    SerialA.begin(A_BAUDRATE);
    SerialB.begin(B_BAUDRATE);
    
}


void loop()
{
    serialsThrough(&SerialA,&SerialB);
    serialsThrough(&SerialB,&SerialA);
}