/*
 * g++ -Wall -O2 -std=c++11 -o mygmpcxxprog mygmpcxxprog.cpp -lgmpxx -lgmp
 */
#include <iostream>
#include <iomanip>
#include <gmpxx.h>

using namespace std;

int main (void)
{
    mpz_class a, b, c, p;

    mpf_set_default_prec(16383);
    cout << "mpf_get_default_prec() is " << mpf_get_default_prec() << endl << endl;

    a = 1234;
    b = "-5678";
    c = a+b;
    cout << "sum is " << c << endl;
    cout << "absolute value is " << abs(c) << endl << endl;

    p = a*b;
    cout << "product is " << p << endl;
    cout << "absolute value is " << abs(p) << endl << endl;

    mpf_class d(1.0d);
    mpz_t f;
    mpf_t fact;

    unsigned int n = 17;
    mpz_init(f);
    mpz_fac_ui(f, n);
    mpf_init(fact);
    mpf_set_z(fact, f);

    mpf_class e(fact);
    
    d /= e;
    //cout << "1.0/" << n << "! is " << setw(20) << d << endl;
    gmp_printf("1.0/%d! is %0.Ff\n\n", n, d.get_mpf_t());

    mpf_class s(2);
    mpf_sqrt(s.get_mpf_t(), s.get_mpf_t());

    gmp_printf("mpf_sqrt(2):\n%0.Ff\n", s.get_mpf_t());


    return 0;
}
