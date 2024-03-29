#ifndef BIT_ARRAY_H_
#define BIT_ARRAY_H_
#include <ArduinoPlatforms.h>
#if defined(RPI)
#include <iostream>
#include <cstring>
using namespace std;
#endif
// unsigned long bit_endian_swap(unsigned long in, uint8_t nBytes = 1)
// {
// 	unsigned long out = 0;
// 	uint16_t lastBit = (8 * nBytes) - 1;
// 	for (uint8_t n = 0; n <= lastBit; n++)
// 	{
// 		uint8_t b = bitRead(in, lastBit - n);
// 		if (b)
// 		{
// 			bitSet(out, n);
// 		}
// 	}

// 	return out;
// }

template <typename T>
T bit_endian_swap(T in)
{

	T out = 0;
	long lastBit = (8 * sizeof(T)) - 1;

	for (long n = lastBit; n > -1; n--)
	{
		uint8_t b = bitRead(in, n);

		if (b)
		{
			bitSet(out, lastBit - n);
		}
	}

	return out;
}

class BitArray
{

protected:
	uint8_t *_data;
	uint16_t _size;

	uint8_t chunkPosition(uint16_t bit) { return bit % 8; }
	uint16_t chunk(uint16_t bit) { return bit / 8; }
	uint16_t chunks() { return _size / 8 + (_size % 8 > 0 ? 1 : 0); }

public:
	/** Default constructor use */
	BitArray() {}

	/**  Constructor with size setting */
	BitArray(uint16_t bits) { init(bits); }

	virtual ~BitArray() { free(_data); }

	/** Init bit array setting size and all bits to low */
	void init(uint16_t bitsNumber)
	{
		setSize(bitsNumber);
		reset();
	}

	/** Set size for bit array */
	void setSize(uint16_t bitsNumber)
	{
		_size = bitsNumber;
		_data = (uint8_t *)realloc(_data, chunks());
	}

	/** Set all bits to low */
	void reset() { memset(_data, 0, chunks()); }

	/** Get size */
	uint16_t size() { return _size; }

	/** Get data */
	uint8_t *data() { return _data; }

	/** Get \c position bit */
	uint8_t read(uint16_t position) { return (position < _size) ? bitRead(_data[chunk(position)], chunkPosition(position)) : 0; }

	/** Get \c position bit value */
	uint8_t operator[](uint16_t position) { return read(position); }

	/** Write \c value to \c position bit */
	void write(uint16_t position, uint8_t value)
	{
		if (position < _size)
			bitWrite(_data[chunk(position)], chunkPosition(position), value);
	}

	/** Set \c position bit to high */
	void set(uint16_t position)
	{
		if (position < _size)
			bitSet(_data[chunk(position)], chunkPosition(position));
	}

	/** Set bit \c position bit to low */
	void clear(uint16_t position)
	{
		if (position < _size)
			bitClear(_data[chunk(position)], chunkPosition(position));
	}

	/** Set all bits to high */
	void set()
	{
		uint16_t ch = chunks();
		for (uint16_t n = 0; n < ch; n++)
		{
			_data[n] = 0xff;
		}
	}
	/** Set all bits to low */
	void clear()
	{
		uint16_t ch = chunks();
		for (uint16_t n = 0; n < ch; n++)
		{
			_data[n] = 0x00;
		}
	}
	/** Set from \c pins reading
	 * @param pins Array containing Arduino digital.
	 */
	void setFromPins(uint8_t *pins)
	{
		for (uint16_t n = 0; n < _size; n++)
		{
			bitWrite(_data[chunk(n)], chunkPosition(n), digitalRead(pins[n]));
		}
	}

	/** Write bit array to digital \c pins
	 * @param pins Array containing digital pin numbers
	 */
	void writeToPins(uint8_t *pins)
	{
		for (uint16_t n = 0; n < _size; n++)
			digitalWrite(pins[n], bitRead(_data[chunk(n)], chunkPosition(n)));
	}
#ifdef RPI
	/** Convert bit mapping into a readeable string. Output example : 010111011 */
	string toString()
	{
		string s;
		for (uint16_t n = 0; n < _size; n++)
			s += (char(read(n) + '0'));
		return s;
	}

	/** Print bit mapping as a readeble string through \c p stream. Output example : 010111011 */
	void debugString()
	{
		for (uint16_t n = 0; n < _size; n++)
			cout << (char(read(n) + '0'));
	}

	/** Convert bit mapping into a readeable csv string
	 * @param delimiter Character, by default ','
	 * String examples :
	 * 0,1,0,1,1,1,0,1,1
	 * 0;1;0;1;1;1;0;1;1
	 */
	string toCSVString(char delimiter = ',')
	{
		string s = "";
		for (uint16_t n = 0; n < _size; n++)
		{
			s += (char(read(n) + '0'));
			if (n != (_size - 1))
				s += delimiter;
		}
		return s;
	}
	/** Parse \c s string. String example : 010111011 */
	void fromString(string s)
	{
		for (uint16_t n = 0; n < s.size(); n++)
		{
			write(n, s[n] - '0');
		}
	}
	/** Parse \c s csv string.Implemented just reading one of each to characters.
	 * Expected-like string examples :
	 * 0,1,0,1,1,1,0,1,1
	 * 0;1;0;1;1;1;0;1;1
	 */
	void fromCSVString(string s)
	{
		uint16_t position;
		for (uint16_t n = 0; n < s.size(); n += 2)
		{
			write(position++, ((uint8_t)s[n]) - '0');
		}
	}

	// DEBUGGING

	/** Print Bits info through stream. Default is Serial */
	void printBits()
	{
		for (uint16_t n = 0; n < _size; n++)
		{
			printf("%ud : %ud\n", n, read(n));
		}
	}
	void printInfo()
	{
		printf("Size : %ud Chuks : %ud\nContent : %s", _size, chunks(), toString());
	}
#else
	/** Convert bit mapping into a readeable string. Output example : 010111011 */
	String toString()
	{
		String s;
		for (uint16_t n = 0; n < _size; n++)
			s.concat(char(read(n) + '0'));
		return s;
	}

	/** Print bit mapping as a readeble string through \c p stream. Output example : 010111011 */
	void toString(Stream *p)
	{
		for (uint16_t n = 0; n < _size; n++)
			p->print(char(read(n) + '0'));
	}

	/** Convert bit mapping into a readeable csv string
	 * @param delimiter Character, by default ','
	 * String examples :
	 * 0,1,0,1,1,1,0,1,1
	 * 0;1;0;1;1;1;0;1;1
	 */
	String toCSVString(char delimiter = ',')
	{
		String s = "";
		for (uint16_t n = 0; n < _size; n++)
		{
			s.concat(char(read(n) + '0'));
			if (n != (_size - 1))
				s += delimiter;
		}
		return s;
	}
	/** Parse \c s string. String example : 010111011 */
	void fromString(String s)
	{
		for (uint16_t n = 0; n < s.length(); n++)
		{
			write(n, s[n] - '0');
		}
	}
	/** Parse \c s csv string.Implemented just reading one of each to characters.
	 * Expected-like string examples :
	 * 0,1,0,1,1,1,0,1,1
	 * 0;1;0;1;1;1;0;1;1
	 */
	void fromCSVString(String s)
	{
		uint16_t position;
		for (uint16_t n = 0; n < s.length(); n += 2)
		{
			write(position++, ((uint8_t)s[n]) - '0');
		}
	}

	// DEBUGGING

	/** Print Bits info through stream. Default is Serial */
	void printBits(Stream *p = &Serial)
	{
		for (uint16_t n = 0; n < _size; n++)
		{
			p->print(n);
			p->print(" : ");
			p->println(read(n));
		}
	}
	void printInfo(Stream *p = &Serial)
	{
		p->print("Size : ");
		p->println(_size);
		p->print("Chunks : ");
		p->println(chunks());
		p->print("Content : ");
		p->println(toString());
	}

#endif
};

#endif /* BIT_ARRAY_H_ */
