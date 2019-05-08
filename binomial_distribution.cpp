/*
 * g++ -Wall -O2 -std=c++11 -o binomial_distribution.exe binomial_distribution.cpp -lm
 */
 
#include <iostream> 
#include <iomanip> 
#include <string> 
#include <map> 
#include <random>   

using namespace std;

int main() 
{ 
	random_device rd; 
	mt19937 gen(rd());
	minstd_rand lcg;
	ranlux24_base sub_eng;
	
//	lcg.seed((unsigned)time(NULL));
	//gen.seed((unsigned)time(NULL));
	sub_eng.seed((unsigned)time(NULL));
	
	// perform 4 trials, each succeeds 1 in 2 times 
	binomial_distribution<> bi(199, 0.5f);
	negative_binomial_distribution<> neg(17, 0.5f);
	uniform_int_distribution<> uni(0, 99);
	normal_distribution<> norm(5, 3);   
	extreme_value_distribution<> ext;
	bernoulli_distribution bern(0.5);
	poisson_distribution<> p_dist(99);
	map<int, int> hist; 
	
	cout << setprecision(10);
	
	for (int n = 0; n < 1000000; ++n) 
	{ 
		//int x = lcg() % 100;
		//int x = sub_eng() % 100;
		//int x = uni(lcg);
		int x = uni(sub_eng);
		//int x = neg(gen);
		//int x = p_dist(lcg);
//		int x = bi(gen);
		//int x = bi(lcg);
		//int x = bi(sub_eng);
		//int x = round(norm(lcg));
		//double y = ext(gen);
		//int x = bern(lcg);
		//cout << y << ' ';
		++hist[x]; 
	}
	cout << endl; 
	for (auto p : hist) 
	{ 
		cout << setw(3) << p.first << ' ' << string(p.second/600, '*') << ' ' << p.second << endl; 
	} 
}
