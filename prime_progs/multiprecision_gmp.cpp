/*
 * g++ -Wall -O2 -std=c++11 -I C:\Tools\gmp-6.1.2 
 *                          -L "C:\Google Drive\Documents II\Visual Studio 2017\Projects\mpir-3.0.0\dll\x64\Release" 
 *                          -o multiprecision_gmp.exe multiprecision_gmp.cpp -lmpir
 */						
#include <boost/integer/integer_log2.hpp>
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/random/independent_bits.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <iostream>
#include <iomanip>
#include <cstdint>

#include <cstring>

#define RSA_1024 "135066410865995223349603216278805969938881475605667027524485143851526510604859533833940287150571909441798207282164471551373680419703964191743046496589274256239341020864383202110372958725762358509643110564073501508187510676594629205563685529475213500852879416377328533906109750544334999811150056977236890927563"

int main()
{
	using namespace boost::random;
	using namespace boost::multiprecision;

	typedef mpz_int int_type;
    time_t time_seed = time(NULL);

	mt11213b base_gen(time(NULL));
	independent_bits_engine<mt11213b, 1024, uint64_t> gen(base_gen);
	mpz_t z;

	mpz_init(z);
	mpz_set_str(z, RSA_1024, 10);

	uint64_t n[128];
	int_type m = 1;

	for (unsigned i = 0; i < sizeof(n) / sizeof(uint64_t); i++)
	{
		n[i] = gen();
		m *= n[i];
	}

	mt11213b gen3(time_seed);
	m |= 1;
	while (!miller_rabin_test(m, 5, gen3))
	{
		m += 2;
		putchar(0x2e);
		std::cout.flush();
	}
	if (miller_rabin_test(m, 23, gen3))
		std::cout << std::endl << m << std::endl;
	
	int_type v = z;
    mt19937 gen2(time_seed);
	
	bool x = miller_rabin_test(v, 23);
	
	std::cout << std::endl << "is_prime(" RSA_1024 "): " << x << std::endl;
	
	mpz_clear(z);

	return 0;
}
