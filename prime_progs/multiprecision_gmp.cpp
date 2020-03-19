/*
 * g++ -Wall -O2 -std=c++11 -I C:\Tools\gmp-6.1.2 
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
#include <chrono>

#define MILLIS(x) setprecision(0) << fixed << x * 1000.0f
#define SECONDS(x) setprecision(1) << fixed << x

#define RSA_1024 "135066410865995223349603216278805969938881475605667027524485143851526510604859533833940287150571909441798207282164471551373680419703964191743046496589274256239341020864383202110372958725762358509643110564073501508187510676594629205563685529475213500852879416377328533906109750544334999811150056977236890927563"

int main()
{
	using namespace boost::random;
	using namespace boost::multiprecision;
	using namespace std;

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
		m ^= n[i];
		m <<= 32;
	}

    auto start = chrono::system_clock::now();
	mt11213b gen3(time_seed);
	m |= 1;
	while (!miller_rabin_test(m, 5, gen3))
	{
		m += 2;
		putchar(0x2e);
		// cout.flush();
	}
	auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;

	if (miller_rabin_test(m, 23, gen3))
	{
		cout << endl << m << endl << endl;

		if (elapsed_seconds.count() >= 1.0f)
			cout << "elapsed time: " << SECONDS(elapsed_seconds.count()) << " s" << endl << endl;
		else
			cout << "elapsed time: " << MILLIS(elapsed_seconds.count()) << " ms" << endl << endl;
	}
	
	int_type v = z;
    mt19937 gen2(time_seed);
	
	bool x = miller_rabin_test(v, 23);
	
	cout << endl << "is_prime(" RSA_1024 "): " << x << endl;
	
	mpz_t rop, base, exp, mod, ONE;
	mpz_inits(rop, base, exp, mod, ONE, NULL);
	
	mpz_set_ui(ONE, 1L);
	mpz_set_ui(base, 13L);
	
	mpz_mul_2exp(z, ONE, 1048576L);
	mpz_set(mod, z);
	mpz_sub_ui(exp, z, 1L);
	
	start = chrono::system_clock::now();
	mpz_powm(rop, base, exp, mod);
	end = chrono::system_clock::now();
	elapsed_seconds = end - start;

	int_type w = rop;
	cout << endl << "mpz_powm(rop, base, exp, mod): " << w << endl;

	cout << endl << "elapsed time: " << SECONDS(elapsed_seconds.count()) << " s" << endl << endl;				// elapsed time: 3975.4 s
	
	//void mpz_powm (mpz_t rop, const mpz_t base, const mpz_t exp, const mpz_t mod)
	
	
	mpz_clears(z, rop, base, exp, mod, ONE);

	return 0;
}
