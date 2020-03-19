#include <gmpxx.h>
#include <iostream>

int main()
{
    mpz_class n(17), k(23), res;
    mpz_powm(res.get_mpz_t(), k.get_mpz_t(), mpz_class(-1).get_mpz_t(), n.get_mpz_t());
    std::cout << res << std::endl;

	return 0;
}
