// 
// Made by Jordan Hoosman
// 

#define __JNUMBERS__

#include <string>			// ...
#include <string.h>			// ...
#include <iostream>
#include <deque>			// ...
#include <algorithm>		// min/max abs
#include <ctime>			// time
#include <vector>			// ...
#include <math.h>			// pow/floor
#include <chrono>			// ...
#include <inttypes.h>		// PRIu32

typedef std::chrono::_V2::system_clock::time_point timep;
inline void timestart(timep& time1) {
	time1 = std::chrono::high_resolution_clock::now();
}
inline void timeend(timep& time2) {
	time2 = std::chrono::high_resolution_clock::now();
}
inline void printspeed(timep time1, timep time2) {
	printf("Took: %d\n", time2 - time1);
}

// NOTES
// - Maybe put bits into stuct; Store size, value
// - Add int/string to Large

namespace JNumbers {
	typedef uint32_t bitblock;
	typedef std::deque<bitblock> bits;
	typedef std::deque<char> bitarray;
	
	const int BITSIZE = sizeof(bitblock) * 8;
	const int BITSIZEA = BITSIZE - 1;
	const int BITSIZEH = BITSIZE / 2;
	const int RMTRIALS = 20;
	
	struct basicnumbers {
		const bits ZERO = { 0 };
		const bits ONE = { 1 };
		const bits TWO = { 2 };
		const bits THREE = { 3 };
		const bits FOUR = { 4 };
		const bits FIVE = { 5 };
		const bits SIX = { 6 };
		const bits SEVEN = { 7 };
		const bits EIGHT = { 8 };
		const bits NINE = { 9 };
		const bits TEN = { 10 };
	};
	static basicnumbers NUMBERS;
	
	static std::vector<std::string> STRPOWERS;
    
	inline void switchbits(bits& A, bits& B);
	inline void setbit(bits& N, int Bit, int Value);
	inline bitarray bitstoarray(bits& N);

	class Large {
        public:
			/***** Functions *****/
			Large();
            
			void printdecimal();
            void printhex();
			void printbinary();
			
			void set(bits N);
            int add(bits N);
            int sub(bits N);
            int mul(bits N);
			int div(bits N);
			int mod(bits N);
			int pow(bits N);
			int leftshift(int Shift);
			int rightshift(int Shift);

            bits generaterand(int Length);
            bits generateprime(int Length);

			// General
			static void printdecimal(bits N);
			static void printhex(bits N);
			static void printbinary(bits N);
			static int hextonum(bitblock Hex);
            static char numtohex(bitblock Num);
			
			static void genpowers(int Length);
			static bits hexstringToLarge(std::string HexStr);

			// TODO
			// - auto int conversion
			static int compare(bits A, bits B);
			static bitblock numbits(bits N);
			// Algorithms
			static bits add(bits A, bits B);
            static bits sub(bits A, bits B);
			static bits sqr(bits N);
            static bits mul(bits A, bits B);
			static bits kmul(bits A, bits B);
            static bits div(bits A, bits B);
			static bits mod(bits A, bits B);
			static bits pow(bits A, bits B);
			static bits pow(bits A, bits B, bits C);
			static std::vector<bits> rdiv(bits A, bits B);
			// Binary operations
			static bits NOT(bits A);
			static bits OR(bits A, bits B);
			static bits AND(bits A, bits B);
			static bits leftshift(bits N, int Shift);
			static bits rightshift(bits N, int Shift);
			static bits zeroextend(bits N, int Zeros);
			static bits zeroreduce(bits N);
			
			static bits twoscomplement(bits N);
			
			// Helper
			static int nthbit(bits& N, int Bit);
			static void changebit(bits& N, int Bit, int Value);
			static bits subbits(bits N, int Start, int End);
			static void ksplit(bits& N, bitblock Size, bits& Low, bits& High);
		private:
			/***** Variables *****/
            const std::string HEX = "0123456789ABCDEF";
			const std::vector<bits> PRIMES = {
				{ 2 },   { 3 },   { 5 },   { 7 },   { 11 },   { 13 },   { 17 },    { 19 },    { 23 },    { 29 },  
				{ 31 },  { 37 },  { 41 },  { 43 },  { 47 },   { 53 },   { 59 },    { 61 },    { 67 },    { 71 },  
				{ 73 },  { 79 },  { 83 },  { 89 },  { 97 },   { 101 },  { 103 },   { 107 },   { 109 },   { 113 }, 
				{ 127 }, { 131 }, { 137 }, { 139 }, { 149 },  { 151 },  { 157 },   { 163 },   { 167 },   { 173 }, 
				{ 179 }, { 181 }, { 191 }, { 193 }, { 197 },  { 199 },  { 211 },   { 223 },   { 227 },   { 229 }, 
				{ 233 }, { 239 }, { 241 }, { 251 }, 

				{ 1, 1 },  { 1, 7 },  { 1, 13 }, { 1, 15 }, { 1, 21 }, { 1, 25 }, 
				{ 1, 27 }, { 1, 37 }, { 1, 51 }, { 1, 55 }, { 1, 57 }, 
				{ 1, 61 }, { 1, 75 }, { 1, 81 }, { 1, 91 }, { 1, 93 } 
			};
			/*const std::vector<bitblock> PRIMES2 = {
				4,     9,     25,    49,    121,   169,    289,    361,    529,    841, 
				961,   1369,  1681,  1849,  2209,  2809,   3481,   3721,   4489,   5041, 
				5329,  6241,  6889,  7921,  9409,  10201,  10609,  11449,  11881,  12769, 
				16129, 17161, 18769, 19321, 22201, 22801,  24649,  26569,  27889,  29929, 
				32041, 32761, 36481, 37249, 38809, 39601,  44521,  49729,  51529,  52441, 
				54289, 57121, 58081, 63001, 66049, 69169,  72361,  73441,  76729,  78961, 
				80089, 85849, 94249, 96721, 97969, 100489, 109561, 113569, 120409, 121801
			};*/

			bits value;
			
    };
}

/****** Operation overloading *****/
// Bool
bool operator==(const JNumbers::bits& num1, const JNumbers::bits& num2);
bool operator!=(const JNumbers::bits& num1, const JNumbers::bits& num2);
bool operator<(const JNumbers::bits& num1, const JNumbers::bits& num2);
bool operator>(const JNumbers::bits& num1, const JNumbers::bits& num2);
bool operator<=(const JNumbers::bits& num1, const JNumbers::bits& num2);
bool operator>=(const JNumbers::bits& num1, const JNumbers::bits& num2);
// Arithmetic
JNumbers::bits operator>>(const JNumbers::bits& num1, const int shift);
JNumbers::bits operator<<(const JNumbers::bits& num1, const int shift);
JNumbers::bits operator|(const JNumbers::bits& num1, const JNumbers::bits& num2);
JNumbers::bits operator&(const JNumbers::bits& num1, const JNumbers::bits& num2);
JNumbers::bits operator+(const JNumbers::bits& num1, const JNumbers::bits& num2);
JNumbers::bits operator-(const JNumbers::bits& num1, const JNumbers::bits& num2);
JNumbers::bits operator*(const JNumbers::bits& num1, const JNumbers::bits& num2);
JNumbers::bits operator/(const JNumbers::bits& num1, const JNumbers::bits& num2);
JNumbers::bits operator%(const JNumbers::bits& num1, const JNumbers::bits& num2);
JNumbers::bits operator~(const JNumbers::bits& num);
// Assignment
JNumbers::bits& operator>>=(JNumbers::bits& num, int shift);
JNumbers::bits& operator<<=(JNumbers::bits& num, int shift);
JNumbers::bits& operator+=(JNumbers::bits& num1, const JNumbers::bits& num2);
JNumbers::bits& operator-=(JNumbers::bits& num1, const JNumbers::bits& num2);
JNumbers::bits& operator*=(JNumbers::bits& num1, const JNumbers::bits& num2);
JNumbers::bits& operator/=(JNumbers::bits& num1, const JNumbers::bits& num2);
// Increment/decrement
JNumbers::bits& operator++(JNumbers::bits& num);
JNumbers::bits& operator--(JNumbers::bits& num);