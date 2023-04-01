#include "millis_utils.h"
/** TODO */
unsigned long toMillis(float timeValue, uint8_t scale)
{
	float millisPer = MILLIS_PER[scale];
	return timeValue * millisPer;
}

/** Get full seconds in rawMillis*/
unsigned long millis2seconds(unsigned long rawMillis) { return rawMillis / MILLIS_PER_SECOND; }
/** Get full minutes in rawMillis */
unsigned long millis2minutes(unsigned long rawMillis) { return rawMillis / MILLIS_PER_MINUTE; }
/** Get full hours in rawMillis */
unsigned long millis2hours(unsigned long rawMillis) { return rawMillis / MILLIS_PER_HOUR; }
/** Get full days in rawMillis */
unsigned long millis2days(unsigned long rawMillis) { return rawMillis / MILLIS_PER_DAY; }

/** Get chrono millis in rawMillis @return 0-999*/
uint16_t chronoMillisForMS(unsigned long rawMillis) { return rawMillis % MILLIS_PER_SECOND; }
/** Get chrono seconds in rawMillis @return 0-59*/
uint16_t chronoSecondsForMS(unsigned long rawMillis) { return (rawMillis % MILLIS_PER_MINUTE) / MILLIS_PER_SECOND; }
/** Gets chrono minutes in rawMillis @return 0-59*/
uint16_t chronoMinutesForMS(unsigned long rawMillis) { return (rawMillis % MILLIS_PER_HOUR) / MILLIS_PER_MINUTE; }
/** Get chrono hours in rawMillis @return 0-23*/
uint16_t chronoHoursForMS(unsigned long rawMillis) { return (rawMillis % MILLIS_PER_DAY) / MILLIS_PER_HOUR; }
/** Get chrono days in rawMillis @return �0-54?*/
uint16_t chronoDaysForMS(unsigned long rawMillis) { return millis2days(rawMillis); }

/** Get millis remaining to \c nextMillis from \c nowMillis */
unsigned long millisRemaining(unsigned long nextMillis, unsigned long nowMillis) { return (nowMillis < nextMillis) ? (nextMillis - nowMillis) : 0; }
/** Check if \c nextMillis moment reached or exceeded \c nowMillis moment. \c nowMillis is by default millis() */
bool millisReached(unsigned long nextMillis, unsigned long nowMillis)
{
	return (nowMillis >= nextMillis);
}
/** Check if \c intervalMillis ellapsed since \c lastRecordMillis */
bool millisOver(unsigned long lastRecordMillis, unsigned long intervalMillis) { 
	// return millisReached(lastRecordMillis + intervalMillis); 
	return ((millis() -  lastRecordMillis) >= intervalMillis);
	}

/** Gets elapsed millis from sinceMillis to indicated nowMillis  */
unsigned long elapsedMillis(unsigned long sinceMillis, unsigned long nowMillis)
{
	return nowMillis - sinceMillis;
	// unsigned long elapsed = 0;

	// if (nowMillis >= sinceMillis)
	// {

	// 	elapsed = nowMillis - sinceMillis;
	// }
	// else
	// {

	// 	elapsed = ULONG_MAX - sinceMillis + nowMillis;
	// }

	// return elapsed;
}

/** Gets  millis since sinceMillis to now   */
unsigned long millisSinceMS(unsigned long sinceMillis)
{
	return elapsedMillis(sinceMillis, millis());
}
/** Gets elapsed millis since sinceMillis  */
unsigned long elapsedMillis(unsigned long sinceMillis)
{
	return elapsedMillis(sinceMillis, millis());
}

/** Gets full seconds since sinceMillis indicated */
unsigned long secondsSinceMS(unsigned long sinceMillis) { return millis2seconds(millisSinceMS(sinceMillis)); }
/** Gets full minutes since sinceMillis indicated */
unsigned long minutesSinceMS(unsigned long sinceMillis) { return millis2minutes(millisSinceMS(sinceMillis)); }
/** Gets full hours since sinceMillis indicated */
unsigned long hoursSinceMS(unsigned long sinceMillis) { return millis2hours(millisSinceMS(sinceMillis)); }
/** Gets full days since sinceMillis indicated */
unsigned long daysSinceMS(unsigned long sinceMillis) { return millis2days(millisSinceMS(sinceMillis)); }

/** Gets chrono millis @return 0-999*/
uint16_t chronoMillisSinceMS(unsigned long sinceMillis) { return chronoMillisForMS(millisSinceMS(sinceMillis)); }
/** Gets chrono seconds @return 0-59*/
uint16_t chronoSecondsSinceMS(unsigned long sinceMillis) { return chronoSecondsForMS(millisSinceMS(sinceMillis)); }
/** Gets chrono minutes @return 0-59*/
uint16_t chronoMinutesSinceMS(unsigned long sinceMillis) { return chronoMinutesForMS(millisSinceMS(sinceMillis)); }
/** Gets chrono hours @return 0-23*/
uint16_t chronoHoursSinceMS(unsigned long sinceMillis) { return chronoHoursForMS(millisSinceMS(sinceMillis)); }
/** Gets chrono days @return �0-54?*/
uint16_t chronoDaysSinceMS(unsigned long sinceMillis) { return chronoDaysForMS(millisSinceMS(sinceMillis)); }

#ifdef ARDUINO
#define PRINT_CHRONO_MODE 0

/** prints Minutes:Seconds:Milliseconds through stream. Format MM:SS:mmm */
void printChronoTimeMSMsFor(unsigned long rawMillis, Stream *stream)
{

#if PRINT_CHRONO_MODE == 0
	char str[10];
	sprintf_P(str, PSTR("%02d:%02d:%03d"), chronoMinutesForMS(rawMillis), chronoSecondsForMS(rawMillis), chronoMillisForMS(rawMillis));
	stream->print(str);
#elif PRINT_CHRONO_MODE == 1
	char str[10];
	sprintf(str, "%02d:%02d:%03d", chronoMinutesForMS(rawMillis), chronoSecondsForMS(rawMillis), chronoMillisForMS(rawMillis));
	stream->print(str);
#elif PRINT_CHRONO_MODE == 2
	if (chronoMinutesForMS(rawMillis) < 10)
		stream->print('0');
	stream->print(chronoMinutesForMS(rawMillis));
	stream->print(':');
	if (chronoSecondsForMS(rawMillis) < 10)
		stream->print('0');
	stream->print(chronoSecondsForMS(rawMillis));
	stream->print(':');
	if (chronoMillisForMS(rawMillis) < 100)
		stream->print('0');
	if (chronoMillisForMS(rawMillis) < 10)
		stream->print('0');
	stream->print(chronoMillisForMS(rawMillis));
#endif
}
/** HH:MM:SS:mmm */
void printChronoTimeFor(unsigned long rawMillis, Stream *stream)
{

#if PRINT_CHRONO_MODE == 0
	char str[4];
	sprintf_P(str, PSTR("%02d:"), chronoHoursForMS(rawMillis));
	stream->print(str);
	printChronoTimeMSMsFor(rawMillis, stream);

#elif PRINT_CHRONO_MODE == 1
	char str[4];
	sprintf(str, "%02d:", chronoHoursForMS(rawMillis));
	stream->print(str);
	printChronoTimeMSMsFor(rawMillis, stream);

#elif PRINT_CHRONO_MODE == 2
	if (chronoHoursForMS(rawMillis) < 10)
		stream->print('0');
	stream->print(chronoHoursForMS(rawMillis));
	stream->print(':');
	printChronoTimeMSMsFor(rawMillis, stream);

#endif
}
#endif
unsigned long MillisTimer::t_now()
{
	return millis();
}

MillisTimer::MillisTimer() : TimerBase()
{
}

MillisTimer::~MillisTimer()
{
}

MillisTimer::MillisTimer(unsigned long t, bool loop, bool start, bool triggerOnFirstCheck) 
{
	 set(t, loop, start, triggerOnFirstCheck);
}

unsigned long MillisTimer::frequency_to_period(double hertzs)
{

	return (unsigned long)(1000.0 / constrain(abs(hertzs), 0, 1000.0));
}

// MillisTimer::MillisTimer(/* args */)
// {
// 	_remaining = _timeSet = _before = _status = _loop = 0;
// }

// MillisTimer::MillisTimer(unsigned long ms, bool loop, bool start, bool triggerOnFirstCheck)
// {
// 	set(ms, loop, start, triggerOnFirstCheck);
// }

// MillisTimer::~MillisTimer()
// {
// }

// bool MillisTimer::check()
// {

// 	if (_status < TIMER_PAUSED)
// 		return false;

// 	unsigned long n = millis();

// 	if (_status == TIMER_ACTIVE)
// 	{

// 		unsigned long l = elapsedMillis(_before, n);
// 		if (l >= _remaining)
// 		{
// 			if (_loop)
// 			{
// 				_remaining = _timeSet;
// 			}
// 			else
// 			{
// 				_remaining = 0;
// 				_status = TIMER_STOPPED;
// 			}
// 			return true;
// 		}
// 		else
// 		{
// 			_remaining -= l;
// 		}
// 	}

// 	_before = n;
// 	return false;
// }

// void MillisTimer::set(unsigned long ms, bool loop, bool start, bool triggerOnFirstCheck)
// {
// 	_timeSet = _remaining = ms;
// 	_loop = loop;
// 	if (start)
// 	{
// 		_before = millis();
// 		_status = TIMER_ACTIVE;
// 	}
// 	else
// 	{
// 		_status = TIMER_PAUSED;
// 	}
// 	if (triggerOnFirstCheck)
// 		_remaining = 0;
// }

// void MillisTimer::setFrequency(double hertzs, bool start, bool triggerOnFristCheck)
// {

// 	// turn freq into period and loop
// 	// millis resolution is 1 so max frequency is 1000Hz
// 	double h = constrain(abs(hertzs), 0.0, 1000.0);
// 	// good idea?
// 	unsigned long ms = (unsigned long)(1000.0 / h);

// 	set(ms, true, start, triggerOnFristCheck);
// }

// void MillisTimer::start(unsigned long ms, bool loop, bool triggerOnFirstCheck)
// {
// 	set(ms, loop, true, triggerOnFirstCheck);
// }

// // void MillisTimer::start(bool triggerOnFirstCheck)
// // {
// // 	set(_timeSet, _loop, true, triggerOnFirstCheck);
// // }
// void MillisTimer::start()
// {
// 	set(_timeSet, _loop, true, false);
// }

// void MillisTimer::startIn(unsigned long ms)
// {
// 	start();
// 	_remaining = ms;
// }
// void MillisTimer::stop()
// {

// 	if ((_status == TIMER_ACTIVE) || (_status == TIMER_PAUSED))
// 	{

// 		check();
// 		_status = TIMER_STOPPED;
// 	}

// 	// _remaining = 0;
// }

// void MillisTimer::pause(bool doPause)
// {

// 	if (_status < TIMER_PAUSED)
// 	{
// 		return;
// 	}

// 	if (doPause)
// 	{
// 		if (_status == TIMER_ACTIVE)
// 		{
// 			_status = TIMER_PAUSED;
// 		}
// 	}
// 	else
// 	{
// 		if (_status == TIMER_PAUSED)
// 			_status = TIMER_ACTIVE;
// 	}
// }

// void MillisTimer::resume()
// {
// 	pause(false);
// }

// // void MillisTimer::delay(unsigned long ms){
// // 	if(_status == TIMER_ACTIVE || _status == TIMER_PAUSED){

// // 	}
// // }
// byte MillisTimer::status() { return _status; }

// unsigned long MillisTimer::remaining()
// {
// 	return _remaining;
// }

// MillisTimer::operator bool()
// {

// 	return check();
// }

// void MillisTimer::loopMode(bool loop)
// {
// 	_loop = loop;
// }

// float MillisTimer::progress()
// {
// 	return 1.0f - (float)_remaining / (float)_timeSet;
// }

// unsigned long MillisTimer::progress100()
// {
// 	return 100UL - 100UL * _remaining / _timeSet;
// }

// bool MillisTimer::isActive()
// {
// 	return _status == TIMER_ACTIVE;
// }

// void MillisTimer::setRemaining(unsigned long remaining)
// {
// 	_remaining = remaining;
// }

MillisChronometer::MillisChronometer(/* args */)
{
	reset();
}

MillisChronometer::~MillisChronometer()
{
}

void MillisChronometer::start()
{

	_before = millis();
	_millis = 0;
	_status = TIMER_ACTIVE;
}
void MillisChronometer::restart()
{

	if ((_status == TIMER_ACTIVE) || (_status == TIMER_PAUSED))
	{

		start();
	}
}

void MillisChronometer::stop()
{
	if (_status > TIMER_STOPPED)
	{
		update();
		_status = TIMER_STOPPED;
	}
}

void MillisChronometer::pause(bool doPause)
{
	if ((_status == TIMER_ACTIVE) && doPause)
	{

		update();
		_status = TIMER_PAUSED;
	}
	else if ((_status == TIMER_PAUSED) && !doPause)
	{
		_before = millis();
		// update();
		_status = TIMER_ACTIVE;
	}
}
void MillisChronometer::resume()
{
	pause(false);
}

void MillisChronometer::reset()
{
	_status = TIMER_NOT_SET;
	_before = _millis = 0;
}

 uint8_t MillisChronometer::status()
{
	return _status;
}
// unsigned long MillisChronometer::value()
// {
// 	return update();
// }

unsigned long MillisChronometer::update()
{

	if (_status <= TIMER_PAUSED)
	{
		if (_status == TIMER_PAUSED)
		{
			_before = millis();
		}
		return _millis;
	}

	unsigned long n = millis();

	if (_status == TIMER_ACTIVE)
	{

		unsigned long l = elapsedMillis(_before, n);
		_millis += l;
	}

	_before = n;
	return _millis;
}

unsigned long MillisChronometer::value()
{
	return update();
}
