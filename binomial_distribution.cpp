#include <boost/random/random_device.hpp>
#include <boost/random/binomial_distribution.hpp>
#include <boost/random/negative_binomial_distribution.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/extreme_value_distribution.hpp>
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <iostream> 
#include <iomanip> 
#include <string> 
#include <map> 

using namespace std;
using namespace boost;
using namespace boost::random;

int main() 
{ 
	random_device rd; 
	mt19937 gen;
	minstd_rand lcg; 
	
	//lcg.seed((unsigned)time(NULL));
	gen.seed((unsigned)time(NULL));
	
	// perform 4 trials, each succeeds 1 in 2 times 
	binomial_distribution<> bi(19, 0.5f);
	negative_binomial_distribution<> neg(17, 0.5f);
	boost::random::uniform_int_distribution<> uni(0, 99);
	normal_distribution<> norm(5, 3);   
	extreme_value_distribution<> ext;
	bernoulli_distribution<> bern(0.1);
	map<int, int> hist; 
	
	cout << setprecision(10);
	
	for (int n = 0; n < 1000000; ++n) 
	{ 
		//int x = lcg() % 100;
		//int x = uni(lcg);
		int x = neg(gen);
		//int x = round(norm(lcg));
		//double y = ext(gen);
		//cout << bern(lcg);
		//cout << y << ' ';
		++hist[x]; 
	}
	cout << endl; 
	for (auto p : hist) 
	{ 
		cout << p.first << ' ' << string(p.second/1000, '*') << ' ' << p.second << endl; 
	} 
}
