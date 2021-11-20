#ifndef BIT_ARRAY_H_
#define BIT_ARRAY_H_

#if ARDUINO < 100
#include "WProgram.h"
#else
#include "Arduino.h"
#endif

class BitArray{

protected :

	uint8_t* _data;
	uint16_t _size;

	uint8_t chunkPosition(uint16_t bit){ return bit%8;}
	uint16_t chunk(uint16_t bit){return bit/8;}
	uint16_t chunks(){return _size/8 + (_size%8 > 0 ? 1:0);}

public :

	/** Default constructor use */
	BitArray(){}

	/**  Constructor with size setting */
	BitArray(uint16_t bits){init(bits);}

	virtual ~BitArray(){free(_data);}

	/** Init bit array setting size and all bits to low */
	void init(uint16_t bitsNumber){
		setSize(bitsNumber);
		reset();
	}

	/** Set size for bit array */
	void setSize(uint16_t bitsNumber){
		_size = bitsNumber;
		_data = (uint8_t*)realloc(_data,chunks());
	}

	/** Set all bits to low */
	void reset(){memset(_data,0,chunks());}

	/** Get size */
	uint16_t size(){return _size;}

	/** Get \c position bit */
	uint8_t read(uint16_t position){return (position < _size) ? bitRead(_data[chunk(position)],chunkPosition(position)) : 0;}

	/** Get \c position bit value */
	uint8_t operator[](uint16_t position){return read(position);}

	/** Write \c value to \c position bit */
	void write(uint16_t position, uint8_t value){if(position < _size)bitWrite(_data[chunk(position)],chunkPosition(position),value);}

	/** Set \c position bit to high */
	void set(uint16_t position){if(position < _size)bitSet(_data[chunk(position)],chunkPosition(position));}

	/** Set bit \c position bit to low */
	void clear(uint16_t position){if(position < _size) bitClear(_data[chunk(position)],chunkPosition(position));}

	/** Set from \c pins reading
	 * @param pins Array containing Arduino digital.
	 */
	void setFromPins(uint8_t* pins){
		for(uint16_t n = 0; n < _size ; n++){bitWrite(_data[chunk(n)],chunkPosition(n),digitalRead(pins[n]));}
	}

	/** Write bit array to digital \c pins
	 * @param pins Array containing digital pin numbers
	 */
	void writeToPins(uint8_t* pins){
		for(uint16_t n = 0 ; n < _size ; n++) digitalWrite(pins[n],bitRead(_data[chunk(n)],chunkPosition(n)));
	}

	/** Convert bit mapping into a readeable string. Output example : 010111011 */
	String toString(){
		String s;
		for(uint16_t n = 0 ; n < _size ; n++) s.concat(char(read(n) + '0'));
		return s;
	}

	/** Print bit mapping as a readeble string through \c p stream. Output example : 010111011 */
	void toString(Stream *p){
		for(uint16_t n = 0 ; n < _size ; n++) p->print(char(read(n) + '0'));
	}

	/** Convert bit mapping into a readeable csv string
	 * @param delimiter Character, by default ','
	 * String examples :
	 * 0,1,0,1,1,1,0,1,1
	 * 0;1;0;1;1;1;0;1;1
	 */
	String toCSVString(char delimiter = ','){
		String s = "";
		for(uint16_t n = 0 ; n < _size ; n++) {
			s.concat(char(read(n) + '0'));
			if(n != (_size-1)) s += delimiter;
		}
		return s;
	}
	/** Parse \c s string. String example : 010111011 */
	void fromString(String s){
		for(uint16_t n = 0 ; n < s.length() ; n++) {
			write(n,s[n]-'0');
		}

	}
	/** Parse \c s csv string.Implemented just reading one of each to characters.
	 * Expected-like string examples :
	 * 0,1,0,1,1,1,0,1,1
	 * 0;1;0;1;1;1;0;1;1
	 */
	void fromCSVString(String s){
		uint16_t position;
		for(uint16_t n = 0 ; n < s.length() ; n+=2) {
			write(position++,((uint8_t)s[n])-'0');
		}
	}

	// DEBUGGING

	/** Print Bits info through stream. Default is Serial */
	void printBits(Stream* p = &Serial){
		for(uint16_t n = 0 ; n < _size ; n++){
			p->print(n);
			p->print(" : ");
			p->println(read(n));
		}
	}
	void printInfo(Stream* p = &Serial){
		p->print("Size : ");
		p->println(_size);
		p->print("Chunks : ");
		p->println(chunks());
		p->print("Content : ");
		p->println(toString());

	}







};


#endif /* BIT_ARRAY_H_ */
