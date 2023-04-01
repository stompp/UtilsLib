// #include <avr/pgmspace.h>
#include "arduino_utils.h"



/**Get the number of elements in \c s delimited by \c delimiter */
int getCSVElementsLength(const char* s, char delimiter){

	char c;
	int p,n;
	n = 1;
	while((c = s[p]) != '\0') if(s[p++] == delimiter) n++;
	return n;
}

int charsFor(unsigned long value){

	int length = 1;
	unsigned long max = 10;
	while((max < value) && (length < 10)){
		length++;
		max *= 10;
	}

	return length;
}
int charsFor(long value){

	unsigned long x = (unsigned long)abs(value);
	int length = charsFor(x);
	if(value < 0) length++;

	return length;


}
/** Get integer and decimal part of float \c value with a decimal resolution as indicated on \c digits */
FloatParts getFloatParts(float value, int digits){

	FloatParts p;
	float x = value;
	// integer/ part
	p.integer = x;
	// decimal part
	x = abs(x - (float)p.integer);
	p.decimal = ((pow(10,1 + digits)*x) + 5)/10;
	while(p.decimal > 0 && (p.decimal%10 == 0)) p.decimal /=  10;
	return p;
}

#ifdef ARDUINO

/**
 * Get free RAM. Taken from the net
 */
// this function will return the number of bytes currently free in RAM
int memoryTest() {
  int byteCounter = 0; // initialize a counter
  byte *byteArray; // create a pointer to a byte array
  // More on pointers here: http://en.wikipedia.org/wiki/Pointer#C_pointers

  // use the malloc function to repeatedly attempt allocating a certain number of bytes to memory
  // More on malloc here: http://en.wikipedia.org/wiki/Malloc
  while ( (byteArray = (byte*) malloc (byteCounter * sizeof(byte))) != 0 ) {
    byteCounter++; // if allocation was successful, then up the count for the next try
    free(byteArray); // free memory after allocating it
  }

  free(byteArray); // also free memory after the function finishes
  return byteCounter; // send back the highest number of bytes successfully allocated
}


/**
 * Get free RAM. Taken from the net
 */
int freeRam  () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

bool freeRamAvailable(int bytes){

	return (freeRam() >= bytes);

}



/** Concat \c x float number string representation into \s string with a decimal resolution as indicated on \c digits */
void concatFloat(String &s,float x,int digits){
	FloatParts p = getFloatParts(x,digits);
	s.concat(p.integer);
	s.concat(".");
	s.concat(p.decimal);
}
/** Get \c x float number string representation with a decimal resolution as indicated on \c digits */
String float2String(float x,int digits){

	String s;
	concatFloat(s,x,digits);
	return s;

}


///** Print basic GET petition to \c host \c path through \c p stream */
//void printGETPetition(const char* host, char* path,Stream *p){
//	p->print(F("GET "));
//	p->print(path);
//	p->println(F(" HTTP/1.1"));
//	p->print(F("Host: "));
//	p->println(host);
//	p->println(F("Connection: close"));
//	p->println();
//
//}

///** Print basic GET petition to \c host \c path through \c p stream */
//void printGETPetition(const char* host, const char* path,Stream *p){
//	p->print(F("GET "));
//	p->print(path);
//	p->println(F(" HTTP/1.1"));
//	p->print(F("Host: "));
//	p->println(host);
//	p->println(F("Connection: close"));
//	p->println();
//
//}

///** Print basic GET petition to \c host \c path through \c p stream */
//void printGETPetition(const char* host, String path,Stream *p){
//	printGETPetition(host,path.c_str(),p);
//
//
//}

/** Reads available bytes from \c source and writes them through \c dest  */
void serialsThrough(Stream* source, Stream *dest){
	int c;
	while((c = source->read()) != -1) dest->write((char)c);
}




///** Print basic GET petition to \c host \c path through \c p stream */
void printGETPetition(const char* host, const char* path,Stream *p){
	p->print(F("GET "));
	p->print(path);
	p->println(F(" HTTP/1.1"));
	p->print(F("Host: "));
	p->println(host);
	p->println(F("Connection: close"));
	p->println();

}

/** Get \c p stream input as string while available \c timeout ms ellapsed */
String getInputString(Stream *p, unsigned long timeout){
	int c;
	String s;
	unsigned long lastRead = millis();
	while (((millis() - lastRead) < timeout)) {
		if((c = p->read())!= -1)  {

			do	s += char(c);
			while((c = p->read())!= -1);
			lastRead = millis();
		}

	}
	return s;

}

/** Get \c p stream input as string while available \c timeout ms ellapsed */
String getInputString(Stream *p, unsigned int size,unsigned long timeout){
	int c;
	String s;
	unsigned long lastRead = millis();
	while ((s.length() != size) && (((millis() - lastRead) < timeout))) {
		if((c = p->read())!= -1)  {
			lastRead = millis();
			s += char(c);
		}


	}
	return s;

}

/** Get \c p stream input as string */
String getInputString(Stream *p){
	int c;
	String s;
	while((c = p->read()) != -1) s.concat(char(c));
	return s;

}


bool goToResponseBodyContent(Stream *p){
	int c;
	uint8_t spaces = 0;
	String s = "";
	while((c = p->read()) != -1) {
		if(isspace(c)) ++spaces;
		else spaces = 0;
		if (spaces == 4) return true;
	}

	return false;

}
String getResponseBodyContent(Stream *p){
	int c;
	uint8_t spaces = 0;
	String s = "";
	while((c = p->read()) != -1) {
		if (spaces == 4) s += char(c);
		else if(isspace(c)) ++spaces;
		else spaces = 0;
	}

	return s;

}
String getResponseBodyContent (const char* host, const char* path,Stream *p, unsigned long waitDelay){
	String s;
	printGETPetition(host,path,p);
	// wait untill data is available
	byte m = 0;
	while((!p->available()) && m++ < 10) delay(waitDelay);
	getResponseBodyContent(p,s);
	return s;
}


String getResponseContent (const char* host, const char* path,Stream *p, unsigned long waitDelay){
	String s ="";
	printGETPetition(host,path,p);
	// wait untill data is available
	byte m = 0;
	while((!p->available()) && m++ < 10) delay(waitDelay);
	int c;
	while(((c = p->read()) != -1)) {s+=(char)c;}
	return s;
}
String getResponseBodyContent(Stream *p,String &s){
	int c;
	uint8_t spaces = 0;
	while(((c = p->read()) != -1)) {
		if (spaces == 4) s += char(c);
		else if(isspace(c)) ++spaces;
		else spaces = 0;
	}

	return s;

}



void sendMegunoPlotData(String seriesName, float data)
{
  Serial.print("{");
  Serial.print(seriesName);
  Serial.print(",T,");
  Serial.print(data);
  Serial.println("}");
}


void debugFreeRam(Stream* p){
	p->print(F("Free Ram : "));
	p->println(freeRam());

}

/**Get the number of elements in \c s delimited by \c delimiter */
int getCSVElementsLength(String &s, char delimiter){
	return getCSVElementsLength(s.c_str(),delimiter);

}

///**Get the number of elements in \c s delimited by \c delimiter */
//String extractNextCSV(String &s, char delimiter){
//
//	String out;
//	int index = s.indexOf(delimiter);
//	if(index>-1){
//		out = s.substring(0,index);
//		s = s.substring(++index);
//	}
//
//	return out;
//
//}



// STRING APPENDING

/** Concat \c digit into \s padding with zeros
 * if representation string length is less than \c fixedLength
 * TODO What about sprintf??
 */
void appendLong(String &s,long digit, int fixedLength){

	long size = (fixedLength > 11 ? 11 : fixedLength);

	if(digit < 0){
		digit *=(-1);
		s.concat('-');
		size--;
	}

	if(size == 11){
		s.concat('0');
		size--;
	}

	long maximum = powLong(10,--size);
	while (maximum > digit){
		s.concat('0');
		maximum/=10;
	}
	s.concat(digit);

}

/** Concat byte \c digit into \s padding with zeros
 * if representation string length is less than 3
 * TODO What about sprintf??
 */
void appendByte(String &s, byte digit){appendLong(s,digit,3);}

 /** Appends a formated float number into input string */
void appendFloat(String &s,float number, int intSize,int floatSize){

	FloatParts p = getFloatParts(number,floatSize);

	appendLong(s,p.integer,intSize);
	s.concat('.');
	appendLong(s,p.decimal,floatSize);
//	if(number < 0){number*=(-1.f);s.concat('-');intSize--;}
//
//	// int part
//	long part = number;
//	appendLong(s,part,intSize);
//	s.concat('.');
//	// real part
//	part = ((number - part)*pow(10,floatSize));
//	appendLong(s,part,floatSize);
}

/** Appends a formated float number in e mode into input string */
void appendEFloat(String &s,float number){

	float sign = (number < 0.0f) ? -1.f : 1.f;
	float base = abs(number);
	int e = 0;

	if((long)base < 1){
		while((long)base < 1) {
			base *= 10.0f;
			e++;
		}
		e *= -1;
	}else{
		while((long)base >= 10) {
			base /= 10.0f;
			e++;
		}
	}
	appendFloat(s,sign*base,2,8);
	s.concat('e');
	appendLong(s,e,3);

}

// TO STREAM PRINT METHODS

void printFormattedLong(long digit, int size,Stream *stream){
	String s = "";
	appendLong(s,digit,size);
	stream->print(s.c_str());

}

void printFormattedByte(byte digit,Stream *stream){
	printFormattedLong(digit,3,stream);

}
void printFormattedFloat(float number, int intSize,int floatSize,Stream *stream){
	String s;
	appendFloat(s,number,intSize,floatSize);
	stream->print(s);

}

void printFormattedEFloat(float number,Stream *stream){
	String s;
	appendEFloat(s,number);
	stream->print(s);

}

//float getEFloat(String &s){
//
//	 int n = s.indexOf('e');
//	 XString baseString;
//	 baseString = s.substring(0,n-1);
//	 // base
//	 float eFloat = baseString.getFloat();
//	 // exponent
//	 int exp = s.substring(n+1).toInt();
//	 n = 0;
//	 while(n < exp) eFloat*=10.0f;
//	 return eFloat;
//
//}

void concatPGMString(PGM_P progString, String &st){
	char *s = (char*)malloc(strlen_P(progString)+1);
	strcpy_P(s,progString);
	st.concat(s);
	free(s);
}
void getPGMString(PGM_P progString, String &st){
	char *s = (char*)malloc(strlen_P(progString)+1);
	strcpy_P(s,progString);
	st = s;
	free(s);
}

String getPGMString(PGM_P progString){
	char *s = (char*)malloc(strlen_P(progString)+1);
	strcpy_P(s,progString);
	String st = s;
	free(s);
	return st;

}

uint32_t print_pgm_str(PGM_P progString,Print *p){
	return p->print(getPGMString(progString));
}

uint32_t print_pgm_strln(PGM_P progString,Print *p){
	return p->println(getPGMString(progString));
}
#endif

void blink(uint8_t pin,int times,int timeOn, int timeOff){
	// pinMode(pin,OUTPUT);
	for(int n = 0 ; n < times ; n++){
		digitalWrite(pin,HIGH);
		delay(timeOn);
		digitalWrite(pin,LOW);
		delay(timeOff == -1 ? timeOn : timeOff);
	}
}
void rblink(uint8_t pin,int times,int timeOn, int timeOff){
	// pinMode(pin,OUTPUT);
	for(int n = 0 ; n < times ; n++){
		digitalWrite(pin,LOW);
		delay(timeOn);
		digitalWrite(pin,HIGH);
		delay(timeOff == -1 ? timeOn : timeOff);
	}
}
void beep(uint8_t pin,unsigned long delayms,int nivel)
/**
 * Beep en el buzz
 * @param delayms Tiempo sonando en milisegundos.
 * @param nivel PWM de salida.
 */
{
	analogWrite(pin, nivel);
	delay(delayms);
	analogWrite(pin, 0);
}

void beeps(uint8_t pin,int total,unsigned long delayms,int nivel)
	/**
	 * Beep en el buzz
	 * @param total Total de beeps.
	 * @param delayms Tiempo sonando en milisegundos.
	 * @param nivel PWM de salida.
	 */
{
	for(int n = 0 ; n < total ; n++) {
		beep(pin,delayms,nivel);
		delay(delayms);
	}
}

void siren(uint8_t pin,unsigned long delayms){
	  analogWrite(pin, 150);
	  delay(delayms);
	  analogWrite(pin, 20);
	  delay(delayms);
	  analogWrite(pin, 0);
	  delay(delayms);
}

int readAverage(int analogPin,int N)
{


	long average = 0;

	for(int n = 0 ; n<N; n++)
	{
		average += analogRead(analogPin);
		delay(10);
	}
		average /= N;
	return average;


}



int readAverage(int analogPin,unsigned int nSamples,unsigned long period){
	if(nSamples == 0) return 0;
	long value = 0;
	unsigned int n = 0;
	while(++n){
		value += (analogRead(analogPin));
		if(n < nSamples) delay(period);
		else break;
	}
		return value/nSamples;
}

unsigned long makeWordUL(unsigned long highW, unsigned long lowW ){	return lowW | (highW<<16);}
unsigned long highWordUL(unsigned long mValue){return 0xffffUL & (mValue>>16);}
unsigned long lowWordUL(unsigned long mValue){return 0xffffUL & (mValue);}



int findOcurrencesUntil(char* str, char target,char term){
	int o = 0;
	int n = 0;
	while((str[n] != term) && (str[n] != '\0')){
		if(str[n] == target) o++;
		n++;
	}

	return o;
}




