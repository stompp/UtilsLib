#include <rgb_utils.h>

void setup(){

    Serial.begin(115200);

    HSVOutput o;

    Serial.println("RED");
    o.debugConversion(255,0,0);
    o.debugConversion(127,0,0);
    Serial.println("GREEN");
    o.debugConversion(0,255,0);
    o.debugConversion(0,127,0);
    Serial.println("BLUE");
    o.debugConversion(0,0,255);
    o.debugConversion(0,0,127);
    Serial.println("YELLOW");
    o.debugConversion(YELLOW);
     Serial.println("CYAN");
    o.debugConversion(CYAN);
    Serial.println("MAGENTA");
    o.debugConversion(MAGENTA);
    Serial.println("WHITE");
    o.debugConversion(WHITE);
   
   
    
}

void loop(){
    
}