/*
 * g++ -Wall -O2 -std=c++11 -o binomial_distribution.exe binomial_distribution.cpp
 */
 
#include <iostream> 
#include <iomanip> 
#include <string> 
#include <map> 
#include <random>   

#define STARS 108
#define REPS 1000000

using namespace std;

int main() 
{ 
	random_device rd; 
	mt19937 gen(rd());
	minstd_rand lcg;
	ranlux24_base sub_eng;
	
	lcg.seed((unsigned)time(NULL));
	gen.seed((unsigned)time(NULL));
	sub_eng.seed((unsigned)time(NULL));
	
	cout << "Enter probability: ";
	float success_probability;
	cin >> success_probability;

	// perform N trials, each succeeds <success_probability> times 
	binomial_distribution<> 			bi(199, success_probability);
	negative_binomial_distribution<> 	neg(17, success_probability);
	uniform_int_distribution<> 			uni(0, 99);
	normal_distribution<> 				norm(5, 3);   
	extreme_value_distribution<> 		ext;
	bernoulli_distribution 				bern(success_probability);
	poisson_distribution<>		 		p_dist(79);
	map<int, int> hist; 
	
	cout << setprecision(10);
	cout << "Counting integers..." << endl;
	
	for (int n = 0; n < REPS; ++n) 
	{ 
		//int x = lcg() % 100;
		//int x = sub_eng() % 100;
		//int x = uni(lcg);
		// int x = uni(sub_eng);
		//int x = neg(gen);
		//int x = p_dist(lcg);
		//int x = p_dist(sub_eng);
		int x = p_dist(gen);
		//int x = bi(gen);
		//int x = bi(lcg);
		//int x = bi(sub_eng);
		//int x = round(norm(lcg));
		//double y = ext(gen);
		//int x = bern(lcg);
		//cout << y << ' ';
		++hist[x]; 
	}
	cout << endl; 

	int max_value = hist.begin()->second;
	for (const auto& nvp : hist) 
	{ 
		if (nvp.second > max_value)
			max_value = nvp.second;
	}
	// cout << max_value << endl;
	
	float scale = max_value / STARS;
	for (const auto& nvp : hist) 
	{ 
		cout << setw(3) << nvp.first << ' ' << string(nvp.second/scale, '*') << ' ' << nvp.second << endl; 
	} 
}
