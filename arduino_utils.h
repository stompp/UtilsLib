/*
 * arduino_utils.h
 *
 *  Created on: 18/09/2013
 *      Author: J.M Tomé
 */

#ifndef arduino_utils_h_
//#pragma once
#define arduino_utils_h_

#if ARDUINO < 100
#ifdef WPROGAM_H
#include "WProgram.h"
#endif
#else
#include "Arduino.h"
#endif
#include <avr/pgmspace.h>
#include <limits.h>
#include <math_utils.h>

#define containsBit(value, bitvalue) ((value & bitvalue) == bitvalue);

typedef struct
{
	long integer;
	long decimal;
} FloatParts;

/**
 * Get free RAM. Taken from the net
 */
// this function will return the number of bytes currently free in RAM
int memoryTest();

/**
 * Get free RAM. Taken from the net
 */
int freeRam();

bool freeRamAvailable(size_t bytes);

int charsFor(unsigned long value);
int charsFor(long value);
/** Get integer and decimal part of \c x with a decimal resolution as indicated on \c digits */
FloatParts getFloatParts(float x, int digits = 6);
/** Concat \c x float number string representation into \s string with a decimal resolution as indicated on \c digits */
void concatFloat(String &s, float x, int digits = 6);
/** Get \c x float number string representation with a decimal resolution as indicated on \c digits */
String float2String(float x, int digits = 6);

/** Reads available bytes from \c source and writes them through \c dest  */
void serialsThrough(Stream *source, Stream *dest);

/** Print basic GET petition to \c host \c path through \c p stream */
//void printGETPetition(const char* host, const char* path,Stream *p);

/** Print basic GET petition to \c host \c path through \c p stream */
//void printGETPetition(char* host, char* path,Stream *p);

/** Print basic GET petition to \c host \c path through \c p stream */
void printGETPetition(const char *host, const char *path, Stream *p);

/** Get \c p stream input as string while available \c timeout ms ellapsed */
String getInputString(Stream *p, unsigned long timeout);

/** Get \c p stream input as string while available \c timeout ms ellapsed */
String getInputString(Stream *p, unsigned int size, unsigned long timeout);

/** Get \c p stream input as string */
String getInputString(Stream *p);
String getResponseBodyContent_T(Stream *p, unsigned long timeout = 1000);

bool goToResponseBodyContent(Stream *p);
String getResponseBodyContent(Stream *p);
String getResponseBodyContent(const char *host, const char *path, Stream *p, unsigned long waitDelay = 500);
String getResponseContent(const char *host, const char *path, Stream *p, unsigned long waitDelay = 500);
String getResponseBodyContent(Stream *p, String &s);

void sendMegunoPlotData(String seriesName, float data);

template <typename T>
void printValue(const __FlashStringHelper *s, T value, Print *p = &Serial)
{
	//prog_char pv_delimiter[] = " -> ";
	p->print(s);
	p->print(F(" -> "));
	p->println(value);
}

template <typename K, typename T>
void printValue(K s, T value, Print *p = &Serial)
{
	//prog_char pv_delimiter[] = " -> ";
	p->print(s);
	p->print(F(" -> "));
	//p->print(pv_delimiter);
	p->println(value);
}

template <typename T>
void debugValue(String n, T v,Print *p = &Serial)
{
    p->print(n);
    p->print(' ');
    p->println(v);
}



template <typename T>
void print(T s, Stream &stream = Serial) { stream.print(s); }
template <typename T>
void println(T s,Stream &stream = Serial) { stream.println(s); }

template <typename T>
void debug(T s,Stream &stream = Serial) { stream.println(s); }

void debugFreeRam(Stream *p = &Serial);




/**Get the number of elements in \c s delimited by \c delimiter */
int getCSVElementsLength(const char *s, char delimiter = ',');
/**Get the number of elements in \c s delimited by \c delimiter */
int getCSVElementsLength(String &s, char delimiter = ',');

//String extractNextCSV(char* input, char delimiter = ',');
//String extractNextCSV(String &s, char delimiter = ',');

// STRING APPENDING

/** Concat \c digit into \s padding with zeros
 * if representation string length is less than \c size
 * TODO What about sprintf??
 */
void appendLong(String &s, long digit, int fixedLength);

/** Concat byte \c digit into \s padding with zeros
 * if representation string length is less than 3
 * TODO What about sprintf??
 */
void appendByte(String &s, byte digit);

/** Appends a formated float number into input string */
void appendFloat(String &s, float number, int intSize, int floatSize);
/** Appends a formated float number in e mode into input string */
void appendEFloat(String &s, float number);

// TO STREAM PRINT METHODS

void printFormattedLong(long digit, int size, Stream *stream = &Serial);

void printFormattedByte(byte digit, Stream *stream = &Serial);
void printFormattedFloat(float number, int intSize, int floatSize, Stream *stream = &Serial);

void printFormattedEFloat(float number, Stream *stream = &Serial);

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

void beep(uint8_t pin, unsigned long delayms, int nivel);

void beeps(uint8_t pin, int total, unsigned long delayms, int nivel);

void siren(uint8_t pin, unsigned long delayms);

int readAverage(int analogPin, int N);
int readAverage(int analogPin, unsigned int nSamples, unsigned long period);

unsigned long makeWordUL(unsigned long highW, unsigned long lowW);
unsigned long highWordUL(unsigned long mValue);
unsigned long lowWordUL(unsigned long mValue);

int findOcurrencesUntil(char *str, char target, char term);

template <typename T>
void swap(T &a, T &b)
{
	T c;
	c = a;
	a = b;
	b = c;
}
template <typename T>
void sortAsc(T *data, uint8_t size)
{

	T aux;
	for (uint8_t n = 0; n < size; n++)
	{
		for (uint8_t m = n; m < size; m++)
		{
			if (data[m] < data[n])
			{
				//				aux = data[n];
				//				data[n] = data[m];
				//				data[m] = aux;
				swap(data[m], data[n]);
			}
		}
	}
}
template <typename T>
void printArray(T *data, uint8_t size, char delimiter = ',', Stream *p = &Serial)
{
	uint8_t endP = size - 1;
	for (uint8_t n = 0; n < endP; n++)
	{
		p->print(data[n]);
		p->print(delimiter);
	}
	p->print(data[endP]);
}
template <typename T>
void printArrayln(T *data, uint8_t size, char delimiter = ',', Stream *p = &Serial)
{
	uint8_t endP = size - 1;
	for (uint8_t n = 0; n < endP; n++)
	{
		p->print(data[n]);
		p->print(delimiter);
	}
	p->println(data[endP]);
}

void concatPGMString(PGM_P progString, String &st);
void getPGMString(PGM_P progString, String &st);

String getPGMString(PGM_P progString);
uint32_t print_pgm_str(PGM_P progString, Print *p = &Serial);

uint32_t print_pgm_strln(PGM_P progString, Print *p = &Serial);

#endif /* ARDUINO_UTILS_H_ */
