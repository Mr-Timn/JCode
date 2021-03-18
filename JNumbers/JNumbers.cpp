//
// Made by Jordan Hoosman
//

#ifndef __JNUMBERS__
	#include "JNumbers.h"
#endif

/****** Operation overloading *****/
// Bool
bool operator==(const JNumbers::bits& num1, const JNumbers::bits& num2) {
	return (JNumbers::Large::compare(num1, num2) == 0);
}
bool operator!=(const JNumbers::bits& num1, const JNumbers::bits& num2) {
	return (JNumbers::Large::compare(num1, num2) != 0);
}
bool operator<(const JNumbers::bits& num1, const JNumbers::bits& num2) {
	return (JNumbers::Large::compare(num1, num2) < 0);
}
bool operator>(const JNumbers::bits& num1, const JNumbers::bits& num2) {
	return (JNumbers::Large::compare(num1, num2) > 0);
}
bool operator<=(const JNumbers::bits& num1, const JNumbers::bits& num2) {
	return (JNumbers::Large::compare(num1, num2) <= 0);
}
bool operator>=(const JNumbers::bits& num1, const JNumbers::bits& num2) {
	return (JNumbers::Large::compare(num1, num2) >= 0);
}
// Arithmetic
JNumbers::bits operator>>(const JNumbers::bits& num1, const int shift) {
	return JNumbers::Large::rightshift(num1, shift);
}
JNumbers::bits operator<<(const JNumbers::bits& num1, const int shift) {
	return JNumbers::Large::leftshift(num1, shift);
}
JNumbers::bits operator|(const JNumbers::bits& num1, const JNumbers::bits& num2) {
	return JNumbers::Large::OR(num1, num2);
}
JNumbers::bits operator&(const JNumbers::bits& num1, const JNumbers::bits& num2) {
	return JNumbers::Large::AND(num1, num2);
}
JNumbers::bits operator+(const JNumbers::bits& num1, const JNumbers::bits& num2) {
	return JNumbers::Large::add(num1, num2);
}
JNumbers::bits operator-(const JNumbers::bits& num1, const JNumbers::bits& num2) {
	return JNumbers::Large::sub(num1, num2);
}
JNumbers::bits operator*(const JNumbers::bits& num1, const JNumbers::bits& num2) {
	return JNumbers::Large::kmul(num1, num2);
}
JNumbers::bits operator/(const JNumbers::bits& num1, const JNumbers::bits& num2) {
	return JNumbers::Large::div(num1, num2);
}
JNumbers::bits operator%(const JNumbers::bits& num1, const JNumbers::bits& num2) {
	return JNumbers::Large::mod(num1, num2);
}
JNumbers::bits operator~(const JNumbers::bits& num) {
	return JNumbers::Large::NOT(num);
}
// Assignment
JNumbers::bits& operator>>=(JNumbers::bits& num, int shift) {
	return num = JNumbers::Large::rightshift(num, shift);
}
JNumbers::bits& operator<<=(JNumbers::bits& num, int shift) {
	return num = JNumbers::Large::leftshift(num, shift);
}
JNumbers::bits& operator+=(JNumbers::bits& num1, const JNumbers::bits& num2) {
	return num1 = JNumbers::Large::add(num1, num2);
}
JNumbers::bits& operator-=(JNumbers::bits& num1, const JNumbers::bits& num2) {
	return num1 = JNumbers::Large::sub(num1, num2);
}
JNumbers::bits& operator*=(JNumbers::bits& num1, const JNumbers::bits& num2) {
	return num1 = JNumbers::Large::kmul(num1, num2);
}
JNumbers::bits& operator/=(JNumbers::bits& num1, const JNumbers::bits& num2) {
	return num1 = JNumbers::Large::div(num1, num2);
}
// Increment/decrement
JNumbers::bits& operator++(JNumbers::bits& num) {
	return num = JNumbers::Large::add(num, JNumbers::NUMBERS.ONE);
}
JNumbers::bits& operator--(JNumbers::bits& num) {
	return num = JNumbers::Large::sub(num, JNumbers::NUMBERS.ONE);
}

/***** JNumbers *****/
void JNumbers::switchbits(bits& A, bits& B) {
	bits temp = A;
	A = B;
	B = temp;
}
void JNumbers::setbit(bits& N, int Bit, int Value) {
	// Index in N
	int con = Bit / BITSIZE;
	// Bit place in N[con]
	int pl = Bit % BITSIZE;

	// Change 0 to 1
	if (Value && !N[con] >> pl & 0b1) {
		N[con] |= (1ULL << pl);
	// Change 1 to 0
	} else if (!Value && N[con] >> pl & 0b1) {
		N[con] &= ~(1ULL << pl);
	}
}
// Un-tested
JNumbers::bitarray JNumbers::bitstoarray(bits& N) {
	bitarray arr;
	bool nz = false;
	
	// MSB -> LSB
	for (int i = 0; i < N.size(); i++) { for (int j = 0; j < BITSIZE; j++) {
		if (nz) {
			if (N[i] >> j & 0b1) arr.push_front(N[i] >> j & 0b1 + '0');
		} else {
			if (N[i] >> j & 0b1) {
				nz = true;
				arr.push_front('1');
			}
		}
	}}
}


/***** Large *****/
JNumbers::Large::Large() {
	srand(time(NULL));
    value.push_back(0);
}

void JNumbers::Large::printdecimal() {
	printdecimal(value);
}
void JNumbers::Large::printhex() {
    printhex(value);
}
void JNumbers::Large::printbinary() {
	printbinary(value);
}

void JNumbers::Large::set(bits N) {
    value = N;
}
int JNumbers::Large::add(bits N) {
    value = value + N;
	return 0;
}
int JNumbers::Large::sub(bits N) {
	value = value - N;
	return 0;
}
int JNumbers::Large::mul(bits N) {
	value = value * N;
	return 0;
}
int JNumbers::Large::div(bits N) {
    value = value / N;
	return 0;
}
int JNumbers::Large::mod(bits N) {
	value = value % N;
	return 0;
}
int JNumbers::Large::pow(bits N) {
	value = pow(value, N);
	return 0;
}
int JNumbers::Large::leftshift(int Shift) {
	value = value << Shift;
	return 0;
}

JNumbers::bits JNumbers::Large::generaterand(int Length) {
	bits vrand; vrand.resize(Length / BITSIZE);

	int bitmax = std::pow(2, BITSIZE);

	for (int i = 0; i < Length / BITSIZE; i++) vrand[i] = rand() % bitmax;

	return vrand;
}
JNumbers::bits JNumbers::Large::generateprime(int Length) {
	srand(time(NULL)); 
	
	bits vrand, veven, vround;
	int maxdiv;
	
	auto trialcomposite = [](bits& nrand, bits& nround, bits& neven, int nmaxdiv) {
		// Trail composite 1
		if (pow(nround, neven, nrand) == NUMBERS.ONE) return false;
		printf("Failed 1\n");
		// Trial composite 2
		for (int j = 0; j < nmaxdiv; j++) {
			if (pow(nround, (NUMBERS.ONE << j) * neven, nrand) == nrand - NUMBERS.ONE) return false;
		}
		printf("Failed 2\n");

		return true;
	};

	while (true) {
		/*** Low level prime check ***/

		// Generate random number
		vrand = generaterand(Length);

		// Primes can't end in 0
		vrand[vrand.size() - 1] |= 0b1;

		// Test against known primes
		bool success = true;
		for (const auto p : PRIMES) {
			if (compare(vrand % p, NUMBERS.ZERO) == 0) {
				success = false;
				break;
			}
		}

		// Try again
		if (!success) continue;

		/*** Miller rabbin test ***/
		//printf("--------------------------------------------\n");
		//printf("Moving to miller rabbit test: \n");
		success = true;
		
		maxdiv = 0;
		veven = vrand - NUMBERS.ONE;

		// Count max divisions
		while (compare(veven % NUMBERS.TWO, NUMBERS.ZERO) == 0) {
			veven = veven >> 1;
			++maxdiv;
		}

		//printf("Max divisions: "); printhex(maxdiv);

		// Rabbin trials
		printf("vrand: "); printdecimal(vrand); printf("\n");
		printf("\tveven: "); printdecimal(veven); printf("\n");
		printf("\tmaxdiv: %d\n", maxdiv); //printhex(maxdiv);
		for (int i = 0; i < RMTRIALS; i++) {
			// Generate random number from 2 - vrand
			vround = generaterand(Length) % vrand + NUMBERS.TWO;

			printf("\tvround: "); printdecimal(vround); printf("\n");
			if (trialcomposite(vrand, vround, veven, maxdiv)) {
				success = false;
				break;
			}
		}

		// Try again
		if (!success) {
			//printf("Failed\n");
			continue;
		}

		// Return prime
		return vrand;
	}
}

// General
void JNumbers::Large::printdecimal(bits N) {
	std::string num = "0";

	auto stradd = [](std::string str1, std::string str2) {
		std::string str = ""; 
		int carry = 0; 

		// str2 needs to be > str1
		if (str1.length() > str2.length()) swap(str1, str2); 
	
		// Calculate length of both string 
		int n1 = str1.length();
		int n2 = str2.length(); 
		int diff = n2 - n1;
	
		// Traverse from end of both strings 
		for (int i = n1 - 1; i >= 0; i--) { 
			// Calculate sum
			int sum = ((str1[i] - '0') + (str2[i + diff] - '0') + carry); 
			str.push_back(sum % 10 + '0'); 
			carry = sum / 10; 
		} 
	
		// Add remaining digits of str2[] 
		for (int i = n2 - n1 - 1; i >= 0; i--) { 
			int sum = ((str2[i] - '0') + carry); 
			str.push_back(sum % 10 + '0'); 
			carry = sum / 10; 
		} 
	
		// Add remaining carry 
		if (carry) str.push_back(carry+'0'); 
	
		// reverse resultant string 
		reverse(str.begin(), str.end()); 
	
		return str; 
	};

	// Generate powers
	if (STRPOWERS.size() < N.size() * BITSIZE) genpowers(N.size() * BITSIZE);

	// Create number
	int b = N.size() * BITSIZE - 1;
	for (int i = 0; i < N.size(); i++) {
		for (int j = BITSIZEA; j >= 0; j--) {
			// Add if nth bit is 1
			if ((N[i] >> j) & 0b1) {
				std::string spow = STRPOWERS[b];
				//std::cout << "Adding " << spow << std::endl;
				num = stradd(num, spow);
			}
			b--;
		}
	}

	printf("%s", num.c_str());
}
void JNumbers::Large::printhex(bits N) {
	std::string str = "";

	int i = 0;
	do {
		str = numtohex(N[N.size() - 1] & 0xF) + str;

		if (i == 7) {
			i = 0;
			str = "_" + str;
		} else i++;
		
		// Shift right for next 4 bits
		N = N >> 4;
	} while (N > NUMBERS.ZERO);

	printf("%s\n", str.c_str());
}
void JNumbers::Large::printbinary(bits N) {
	int nb = numbits(N);
	int nc = (N.size() * BITSIZE);

	for (int i = 0; i < N.size(); i++) {
		for (int j = BITSIZEA; j >= 0; j--) {
			// Don't print end zeros
			if (nc <= nb) 
				printf("%d", (N[i] >> j) & 0b1);
			else nc--;
		}
		printf("_");
	}
	printf("\b \b");
	printf("\n");
}
int JNumbers::Large::compare(bits A, bits B) {
	A = zeroreduce(A);
	B = zeroreduce(B);

	if (A.size() > B.size()) return 1;
	else if (A.size() < B.size()) return -1;

	for (int i = 0; i < A.size(); i++) {
		if (A[i] > B[i]) return 1;
		else if (A[i] < B[i]) return -1;
	}

	return 0;
}
int JNumbers::Large::hextonum(bitblock Hex) {
    switch (Hex) {
        case '0': return 0b0000;
        case '1': return 0b0001;
        case '2': return 0b0010;
        case '3': return 0b0011;
        case '4': return 0b0100;
        case '5': return 0b0101;
        case '6': return 0b0110;
        case '7': return 0b0111;
        case '8': return 0b1000;
        case '9': return 0b1001;
        case 'A': case 'a': return 0b1010;
        case 'B': case 'b': return 0b1011;
        case 'C': case 'c': return 0b1100;
        case 'D': case 'd': return 0b1101;
        case 'E': case 'e': return 0b1110;
        case 'F': case 'f': return 0b1111;
        default: printf("X"); return 0b0;
    }
}
char JNumbers::Large::numtohex(bitblock Num) {
    switch (Num) {
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        case 10: return 'A';
        case 11: return 'B';
        case 12: return 'C';
        case 13: return 'D';
        case 14: return 'E';
        case 15: return 'F';
        default: return 'X';
    }
}
JNumbers::bitblock JNumbers::Large::numbits(bits N) {
	zeroreduce(N);

	bool zero = false;
	bitblock count = 0;

	for (int i = 0; i < N.size(); i++) {
		if (zero) count += BITSIZE;
		else {
			for (int j = BITSIZE - 1; j >= 0; j--) {
				if ((N[i] >> j) & 0b1) {
					zero = true;
					count += (j + 1);
					break; 
				}
			}
		}
	}

	return count;
}

JNumbers::bits JNumbers::Large::hexstringToLarge(std::string HexStr) {
	if (HexStr.length() < 1) return NUMBERS.ZERO;

	bits N;
	bitblock x = 0;
	int sh = 0;
	
	for (int i = HexStr.length() - 1; i >= 0; i--) {
		//std::cout << hextonum(HexStr[i]) << std::endl;
		x |= (hextonum(HexStr[i]) << sh);
		//std::cout << "x: " << x << std::endl;
		sh += 4;
		
		if (sh >= 32) {
			N.push_front(x);

			x = 0;
			sh = 0;
		}
	}
	
	if (x) N.push_front(x);

	return N;
}
void JNumbers::Large::genpowers(int Length) {
	auto strmul = [](int x, int res[], int res_size) {
		int carry = 0;
		
		// Calculate
		for (int i = 0; i < res_size; i++) {
			int prod = res[i] * x + carry;
		
			// Store last digit
			res[i] = prod % 10;

			// Put rest in carry
			carry = prod / 10;
		}
		
		// Put carry in res and increase result size
		while (carry) {
			res[res_size] = carry % 10;
			carry = carry / 10;
			res_size++;
		}

		return res_size;
	};
	auto strpow = [&](int x, int n) {
		if (n == 0) return std::string("1");
		else if (n == 1) return std::to_string(x);
		
		int res[10000];
		int res_size = 0;
		int temp = x;
		std::string ret = "";
		
		while (temp != 0) {
			res[res_size++] = temp % 10;
			temp = temp / 10;
		}
		
		// Multiply x n times
		for (int i = 2; i <= n; i++) res_size = strmul(x, res, res_size);

		// Copy to ret
		for (int i = res_size - 1; i >= 0; i--) ret += std::to_string(res[i]);
		
		return ret;
	};

	std::string temppow;
	for (int i = 0; i < Length; i++) {
		if (STRPOWERS.size() <= i) {
			temppow = strpow(2, i); //printf("%s\n", temppow.c_str());
			STRPOWERS.push_back(temppow);
		}
	}
}

// Algorithms
JNumbers::bits JNumbers::Large::add(bits A, bits B) {
	uint64_t a = 0;
	uint64_t b = 0;
	uint64_t total = 0;
    uint64_t carry = 0;
    
	// Set A.size() = B.size()
	if (A.size() != B.size()) {
		if (A.size() < B.size()) A = zeroextend(A, B.size() - A.size());
		else 					 B = zeroextend(B, A.size() - B.size());
	}

	int nbits = A.size();
	bits C; C.resize(nbits);
    for (int i = nbits - 1; i >= 0; i--) {
		// Get [BITSIZE]-bits
        a = A[i];
        b = B[i];
        
		// Calculate total
        total = (a + b) + carry;
		// Store overflow bit in carry
        carry = (total >> BITSIZE);
		
		// Update total
        C[i] = total & ~(0ULL << BITSIZEA);
		
		//printf("Total: %" PRIu32 "\n", total & ~(0U << BITSIZEA));
		//printf("Carry: %d\n", carry);
    }

	// Handle carry
	// - Adding can at [most] overflow 1 bit over 
	if (carry) C.push_front(carry);
	
	return C;
}
JNumbers::bits JNumbers::Large::sub(bits A, bits B) {
	uint64_t a = 0;
	uint64_t b = 0;
    int64_t carry = 0;
	uint64_t total = 0;

	// Set A.size() = B.size()
	if (A.size() != B.size()) {
		if (A.size() < B.size()) A = zeroextend(A, B.size() - A.size());
		else 					 B = zeroextend(B, A.size() - B.size());
	}

	int nbits = A.size();
	bits C; C.resize(nbits);

	if (A < B) {
		printf("SubtractionError: A < B\n");
		return NUMBERS.ZERO;
	}
	
	for (int i = nbits - 1; i >= 0; i--) {
		// Get bits
		a = A[i];
		b = B[i];
		
		// Calculate
		total = (a + (~(b) + 1)) - carry;
		carry = (total >> BITSIZE) & 0b1;
		
		// Update value
		C[i] = total & ~(0ULL << BITSIZEA);
		
		//printf("Total: %" PRIu64 "\n", total & ~(0U << BITSIZEA));
		//printf("Carry: %" PRId64 "\n", carry);
	}
	
	zeroreduce(C);
	return C;
}
JNumbers::bits JNumbers::Large::sqr(bits N) {
	int nsize = N.size();
	int nbits = numbits(N);

	bits W; W.resize(N.size() * 2);
	
	int u, v, uv, c, cc;

	for (int i = nsize - 1; i >= 0; i--) { for (int j = 0; j < BITSIZEA; j++) {
		// Adjusted for LSB -> MSB
		int cbit = i * nsize + j;
		printf("%d--------------------------------------------------\n", cbit);

		// (uv) <- W_2i, + (N_i*N_i)
		u = nthbit(W, cbit * 2); 		printf("u: %d\n", u);
		v = nthbit(N, cbit); 			printf("v: %d\n", v);
		uv = u + v; 					printf("uv: %d\n", uv);

		// W_2i <- v
		setbit(W, cbit * 2, v);			printf("W: "); printbinary(W);

		// c <- u
		c = u;							printf("c: %d\n", c);

		// 
		for (int k = cbit + 1; k < nbits - 1; k++) {
			printf("\t%d-----------------------------------\n", k);
			// (uv) <- w(cbit+k) + 2*n(j)*n(cbit), + c
			u = nthbit(W, cbit + k) + 2 * nthbit(N, k) * nthbit(N, cbit);	printf("u: %d\n", u);
			v = c;															printf("v: %d\n", v);
			uv = u + v; 													printf("uv: %d\n", uv);
			
			// w(cbit+k)
			setbit(W, cbit + k, v);											printf("W: "); printbinary(W);

			// c <- u
			c = u; 															printf("u: %d\n", u);
		}

		// w(cbit+nbits) <- u
		setbit(W, cbit + nbits, u); 										printf("Final W: "); printbinary(W);
	} }

	return W;
}
JNumbers::bits JNumbers::Large::mul(bits A, bits B) {
	A = zeroreduce(A);
	B = zeroreduce(B);
	bits C = NUMBERS.ZERO;

	int chk = compare(A, B);
	
	if (chk == 0) return sqr(A); // A == B
	else if (chk < 0) switchbits(A, B); // A < B

	while (B > NUMBERS.ZERO) {
		// LSB == 1
		if (B[B.size() - 1] & 0b1) C = C + A;

		A = A << 1;
		B = B >> 1;
	}

	C = zeroreduce(C);
	return C;
}
JNumbers::bits JNumbers::Large::kmul(bits A, bits B) {
	// Set A.size() = B.size()
	if (A.size() != B.size()) {
		if (A.size() < B.size()) A = zeroextend(A, B.size() - A.size());
		else 					 B = zeroextend(B, A.size() - B.size());
	}

	if (B > A) {
		bits tempa = A;
		A = B;
		B = tempa;
	}

	int nbits = std::max(numbits(A), numbits(B));
	if (nbits < 511) return mul(A, B);
	//if (nbits <  2) return (A == NUMBERS.ONE && B == NUMBERS.ONE) ? NUMBERS.ONE : NUMBERS.ZERO;
	//else printf("NUMBITS: %d\n", nbits);

	bits alow, ahigh;
	bits blow, bhigh;

	// Split parts
	ksplit(A, nbits, alow, ahigh);
	ksplit(B, nbits, blow, bhigh);
	
	// Apply recursive Karatsuba multipication
	bits p1 = kmul(alow, blow);
	bits p2 = kmul(ahigh, bhigh);
	bits p3 = kmul(alow + ahigh, blow + bhigh);
	int nn = nbits - (nbits / 2);
	
	bits sh1 = NUMBERS.ONE << (2 * nn);
	bits m1 = mul(p2, sh1);
	bits sh2 = (NUMBERS.ONE << nn);
	bits su1 = (p3 - p2) - p1;
	bits m2 = mul(su1, sh2);
	bits res = m1 + m2 + p1;
	
	/*printf("A: "); printhex(A);
	printf("B: "); printhex(B);
	printf("AHigh: "); printdecimal(ahigh); printf("\n");
	printf("BHigh: "); printdecimal(bhigh); printf("\n");
	printf("ALow: "); printdecimal(alow); printf("\n");
	printf("BLow: "); printdecimal(blow); printf("\n");
	printf("\tp1: "); printdecimal(p1); printf("\n");
	printf("\tp2: "); printdecimal(p2); printf("\n");
	printf("\tp3: "); printdecimal(p3); printf("\n");
	printf("sh1: 1 << %d = ", 2 * nn); printdecimal(sh1); printf("\n");
	printf("sh2: 1 << %d = ", nn); printdecimal(sh2); printf("\n");
	printf("su1: "); printdecimal(p3);  printf(" - "); printdecimal(p2);  printf(" - "); printdecimal(p1); printf(" = "); printdecimal(su1); printf("\n");
	printf("m1: ");  printdecimal(p2);  printf(" * "); printdecimal(sh1); printf(" = "); printdecimal(m1);  printf("\n");
	printf("m2: ");  printdecimal(su1); printf(" * "); printdecimal(sh2); printf(" = "); printdecimal(m2);  printf("\n");
	printf("res: "); printdecimal(m1);  printf(" + "); printdecimal(m2);  printf(" + "); printdecimal(p1); printf(" = "); printdecimal(res); printf("\n");
	printf("------------------------\n");*/

	return mul(p2, NUMBERS.ONE << (2 * nn)) + mul(p3 - p2 - p1, (NUMBERS.ONE << nn)) + p1;
}
JNumbers::bits JNumbers::Large::div(bits A, bits B) {
	return rdiv(A, B)[0];
}
JNumbers::bits JNumbers::Large::mod(bits A, bits B) {
	return rdiv(A, B)[1];
}
JNumbers::bits JNumbers::Large::pow(bits A, bits B) {
	bits N = NUMBERS.ONE;

	while (B > NUMBERS.ZERO) {
		if (B[B.size() - 1] & 0b1) N = N * A;

		B = B >> 1;
		A = A * A;
	}

	return N;
}
JNumbers::bits JNumbers::Large::pow(bits A, bits B, bits C) {
	if (C == NUMBERS.ONE) return NUMBERS.ZERO;
	
	bits N = NUMBERS.ONE;
	A = A % C;

	// 35411405
	// 14326304
	// 14466597

	while (B > NUMBERS.ZERO) {
		if (B[B.size() - 1] & 0b1) N = (N * A) % C;

		B = B >> 1;
		A = (A * A) % C;
	}

	return N;
}
std::vector<JNumbers::bits> JNumbers::Large::rdiv(bits A, bits B) {
	bits Q;
	bits R = NUMBERS.ZERO;
	
	// A == B
	if (compare(A, B) == 0) {
		Q.push_back(1);
		R.push_back(0);

		return { Q, R };
	}
	
	A = zeroreduce(A);
	B = zeroreduce(B);
	int nbits = A.size();
	
	Q.resize(nbits);

	for (int i = 0; i < nbits; i++) {
		for (int j = BITSIZEA; j >= 0; j--) {
			// Left-Shift R 1 bit
			R = R << 1;
			
			// Set LSB of R to N[i]
			if ((A[i] >> j) & 0b1) R = OR(R, { 0b1 });  // Set to 1

			// Check R >= D
			if (compare(R, B) != -1) {
				R = R - B;
				Q[i] |= (0x1 << j);
			}
		}
    }

	Q = zeroreduce(Q);
	R = zeroreduce(R);

	return { Q, R };
}
// Binary operations
JNumbers::bits JNumbers::Large::NOT(bits A) {
	//for (int i = 0; i < A.size(); i++) A[i] = ~(A[i]);
	bool flip = false;
	for (int i = 0; i < A.size(); i++) {
		if (flip) A[i] = ~A[i];
		else {
			for (int j = BITSIZE - 1; j >= 0; j--) {
				if (flip) {
					A[i] = A[i] ^ (0b1 << j);
				} else if((A[i] >> j) & 0b1) {
					flip = true;
					A[i] = A[i] ^ (0b1 << j);
				}
			}
		}
	}

	return A;
}
JNumbers::bits JNumbers::Large::OR(bits A, bits B) {
	bits C; C.resize(std::max(A.size(), B.size()));
	
	// Set A.size() = B.size()
	if (A.size() != B.size()) {
		if (A.size() < B.size()) A = zeroextend(A, B.size() - A.size());
		else B = zeroextend(B, A.size() - B.size());
	}

	for (int i = 0; i < C.size(); i++) {
		C[i] = A[i] | B[i];
	}

	return C;
}
JNumbers::bits JNumbers::Large::AND(bits A, bits B) {
	bits C;

	// Set A.size() = B.size()
	if (A.size() != B.size()) {
		if (A.size() < B.size()) A = zeroextend(A, B.size() - A.size());
		else B = zeroextend(B, A.size() - B.size());
	}

	for (int i = 0; i < A.size(); i++) C.push_back(A[i] & B[i]);
	
	return C;
}
JNumbers::bits JNumbers::Large::leftshift(bits N, int Shift) {
	bits LS; 
	uint64_t carry = 0;
	uint64_t total = 0;

	// Adjust if Shift > BITSIZE
	if (Shift >= BITSIZE) {
		for (int i = 0; i < Shift / BITSIZE; i++) N.push_back(0b0);
		Shift = Shift % BITSIZE;
		
		if (Shift == 0) return N;
	}

	int nbits = N.size();
	for (int i = nbits - 1; i >= 0; i--) {
		if (N[i] != 0) {
			// Shift bits
			total = N[i];
			total = (total << Shift) | carry;
			carry = N[i] >> (BITSIZE - Shift);

			// Update value
			LS.push_front(total & ~(0ULL << BITSIZEA));
		} else {
			LS.push_front(carry);
			carry = 0;
		}
	}

	// Extend carried bit
	if (carry) LS.push_front(carry);

	return LS;
}
JNumbers::bits JNumbers::Large::rightshift(bits N, int Shift) {
	bitblock carry = 0;
	bitblock tempcarry = 0;

	if (Shift >= BITSIZE) {
		for (int i = 0; i < Shift / BITSIZE; i++) N.pop_back();
		Shift = Shift % BITSIZE;
		
		if (Shift == 0) return N;
	}

	int nbits = N.size();
	for (int i = 0; i < nbits; i++) {
		if (carry) {
			// Store temp carry
			tempcarry = N[i] & ~(0U << (Shift-1));
			
			// Shift bits right
			N[i] = N[i] >> Shift;

			// Insert carry
			N[i] |= (carry << (BITSIZE - Shift));

			// Restore carry value
			carry = tempcarry;	
		} else {
			// Check for carry
			carry = N[i] & ~(0ULL << (Shift-1));

			// Shift bits right
			N[i] = N[i] >> Shift;
		}
	}

	return N;
}
JNumbers::bits JNumbers::Large::zeroextend(bits N, int Zeros) {
	for (int i = 0; i < Zeros; i++) N.push_front(0);

	return N;
}
JNumbers::bits JNumbers::Large::zeroreduce(bits N) {
	int nbits = N.size();
	for (int i = 0; i < nbits - 1; i++) {
		if (N.front()) break;
		
		N.pop_front();
	}	

	return N;
}

JNumbers::bits JNumbers::Large::twoscomplement(bits N) {
	N = ~N;
	N = ++N;

	return N;
}

int JNumbers::Large::nthbit(bits& N, int Bit) {
	// Index in N
	int con = Bit / BITSIZE;
	// Bit place in N[con]
	int pl = Bit % BITSIZE;

	return (N[con] >> pl) & 0b1;
}
JNumbers::bits JNumbers::Large::subbits(JNumbers::bits N, int Start, int End) {
	bits S;
	int startblock = Start % BITSIZE;
	int endblock = End % BITSIZE;

	for (int i = N.size() - 1; i >= 0; i--) {
		
	}

}
void JNumbers::Large::ksplit(bits& N, bitblock Size, bits& Low, bits& High) {
	int halfhigh = Size / 2;
	int halflow = Size - halfhigh;
	
	// Fill high bits
	High = N >> (Size - halfhigh);

	// Fill low bits
	Low = N & ~(NUMBERS.ONE << halflow);
}