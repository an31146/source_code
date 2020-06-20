#include <iostream>
#include <iomanip>
#include <complex>
#include <cmath>

using std::complex;

complex<double> dirichlet(complex<double> s, int terms)
{
    complex<double> z = 0;
    for (int n = 1; n < terms; n++)
    {
        double d = (double)n;
        z += d / pow(d + 1.0, s);
        z -= (d - s) / pow(d, s);
    }
    z /= (s - 1.0);
    return z;
}
 
int main()
{
    using namespace std;
    cout << fixed << setprecision(14);
 
    complex<double> z(1, 2);
    cout << "(1,2)^2 = " << pow(z, 2) << '\n';
 
    complex<double> z2(-1, 0);  // square root of -1
    cout << "-1^0.5 = " << pow(z2, 0.5) << '\n';
 
    complex<double> z3(-1, -0.0);  // other side of the cut
    cout << "(-1, -0)^0.5 = " << pow(z3, 0.5) << '\n';
 
    complex<double> i(0, 1); // i^i = exp(-pi/2)
    cout << "i^i = " << pow(i, i) << '\n';

    double step = 0.0001d;
    double multiplier = 10000.0;
    int lastSign = true;
    for (double t = 14.134; t < 50.0; t += step)
    {
        t = ceil(t * multiplier) / multiplier;
        complex<double> z4(0.5, t);
        complex<double> zeta = dirichlet(z4, 100000000);
        cout << "zeta" << z4 << " = " << zeta << "\tphase: " << arg(zeta) << endl;
        int _sign = signbit(arg(zeta));
        if (_sign != lastSign)
        {
            t -= step;
            step /= 10.0d;
            multiplier *= 10.0;
            lastSign = !_sign;
            cout << "-------------------" << endl;

        }
    }
    
    return 0;
}
