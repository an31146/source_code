/*
 * g++ -Wall -O2 -std=c++11 -I C:\Tools\gmp-6.1.2 -o mpz_powm_neg-exp.exe mpz_powm_neg-exp.cpp -lgmpxx -lgmp
 */
#include <gmpxx.h>
#include <iostream>

int main()
{
    mpz_class n(17), k(23), res;
    mpz_powm(res.get_mpz_t(), k.get_mpz_t(), mpz_class(-1).get_mpz_t(), n.get_mpz_t());
    std::cout << res << std::endl;
    
    n = mpz_class("8044519426324086992388511943860210371379357135842224134212814310730659405905187050236423326842895406851752905185507785094492055233367688297241115376133579", 10);
    std::cout << "mpz_is_prime(" << n << "): " << mpz_probab_prime_p(n.get_mpz_t(), 23) << std::endl;

	return 0;
}
