// #pragma once
#ifndef _millis_utils_h_
#define _millis_utils_h_

// #include <Arduino.h>
#include "timer_base.h"
// #if ARDUINO < 100
// #include "WProgram.h"
// #else
// #include "Arduino.h"
// #endif

#include <limits.h>

#define MILLIS_PER_SECOND 1000UL
#define MILLIS_PER_MINUTE 60000UL
#define MILLIS_PER_HOUR 3600000UL
#define MILLIS_PER_DAY 86400000UL

// #define MU_MILLIS 0x00
// #define MU_SECONDS 0x01
// #define MU_MINUTES 0x02
// #define MU_HOURS 0x03
// #define MU_DAYS 0x04

static const unsigned long MILLIS_PER[] = {1, MILLIS_PER_SECOND, MILLIS_PER_MINUTE, MILLIS_PER_HOUR, MILLIS_PER_DAY};

unsigned long toMillis(float timeValue, uint8_t scale);

/** Get full seconds in rawMillis*/
unsigned long millis2seconds(unsigned long rawMillis);
/** Get full minutes in rawMillis */
unsigned long millis2minutes(unsigned long rawMillis);
/** Get full hours in rawMillis */
unsigned long millis2hours(unsigned long rawMillis);
/** Get full days in rawMillis */
unsigned long millis2days(unsigned long rawMillis);

/** Get chrono millis in rawMillis @return 0-999*/
uint16_t chronoMillisForMS(unsigned long rawMillis);
/** Get chrono seconds in rawMillis @return 0-59*/
uint16_t chronoSecondsForMS(unsigned long rawMillis);
/** Gets chrono minutes in rawMillis @return 0-59*/
uint16_t chronoMinutesForMS(unsigned long rawMillis);
/** Get chrono hours in rawMillis @return 0-23*/
uint16_t chronoHoursForMS(unsigned long rawMillis);
/** Get chrono days in rawMillis @return �0-54?*/
uint16_t chronoDaysForMS(unsigned long rawMillis);

/** Get millis remaining to \c nextMillis from \c nowMillis */
unsigned long millisRemaining(unsigned long nextMillis, unsigned long nowMillis = millis());
/** Check if \c nextMillis moment reached or exceeded \c nowMillis moment. \c nowMillis is by default millis() */
bool millisReached(unsigned long nextMillis, unsigned long nowMillis = millis());
/** Check if \c intervalMillis ellapsed since \c lastRecordMillis */
bool millisOver(unsigned long lastRecordMillis, unsigned long intervalMillis);

/** Gets elapsed millis from sinceMillis to indicated nowMillis  */
unsigned long elapsedMillis(unsigned long sinceMillis, unsigned long nowMillis);
/** Gets elapsed millis since sinceMillis  */
unsigned long elapsedMillis(unsigned long sinceMillis);

/** Gets full millis since sinceMillis indicated */
unsigned long millisSinceMS(unsigned long sinceMillis);
/** Gets full seconds since sinceMillis indicated */
unsigned long secondsSinceMS(unsigned long sinceMillis);
/** Gets full minutes since sinceMillis indicated */
unsigned long minutesSinceMS(unsigned long sinceMillis);
/** Gets full hours since sinceMillis indicated */
unsigned long hoursSinceMS(unsigned long sinceMillis);
/** Gets full days since sinceMillis indicated */
unsigned long daysSinceMS(unsigned long sinceMillis);

/** Gets chrono millis @return 0-999*/
uint16_t chronoMillisSinceMS(unsigned long sinceMillis);
/** Gets chrono seconds @return 0-59*/
uint16_t chronoSecondsSinceMS(unsigned long sinceMillis);
/** Gets chrono minutes @return 0-59*/
uint16_t chronoMinutesSinceMS(unsigned long sinceMillis);
/** Gets chrono hours @return 0-23*/
uint16_t chronoHoursSinceMS(unsigned long sinceMillis);
/** Gets chrono days @return 0-54?*/
uint16_t chronoDaysSinceMS(unsigned long sinceMillis);

void printChronoTimeMSMsFor(unsigned long rawMillis, Stream *stream);
void printChronoTimeFor(unsigned long rawMillis, Stream *stream);


class MillisTimer : public virtual TimerBase
{
protected:
    /* data */

    virtual unsigned long t_now();

    virtual unsigned long frequency_to_period(double hertzs);

public:
    MillisTimer(/* args */);
    MillisTimer(unsigned long t, bool loop = false, bool start = false, bool triggerOnFirstCheck = false);
    ~MillisTimer();
};

// class MillisTimer
// {
// protected:
//     unsigned long _before;
//     unsigned long _remaining;
//     unsigned long _timeSet;
//     bool _loop;
//     byte _status;

// public:
//     MillisTimer();
//     MillisTimer(unsigned long ms, bool loop = false, bool start = false, bool triggerOnFirstCheck = false);
//     ~MillisTimer();

//     bool check();

//     void set(unsigned long ms, bool loop = false, bool start = false, bool triggerOnFirstCheck = false);

//     void setFrequency(double hertzs, bool start = false, bool triggerOnFristCheck = false);

//     void start(unsigned long ms, bool loop = false, bool triggerOnFirstCheck = false);

//     // void start(bool triggerOnFirstCheck);

//     void start();
//     void startIn(unsigned long ms);

//     void stop();
//     void pause(bool doPause = true);
//     void resume();

//     // void delay(unsigned long ms);

//     unsigned long remaining();

//     byte status();
//     operator bool();

//     void loopMode(bool loop);

//     float progress();

//     unsigned long progress100();

//     bool isActive();

//     void setRemaining(unsigned long remaining);
// };

class MillisChronometer
{
private:
    unsigned long _before;
    unsigned long _millis;
    byte _status;

public:
    MillisChronometer(/* args */);
    ~MillisChronometer();

    void start();
    void restart();
    void stop();
    void pause(bool doPause = true);
    void resume();
    void reset();

    byte status();

    unsigned long update();

    unsigned long value();
};

#endif