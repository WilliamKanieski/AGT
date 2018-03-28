// GeometricDistributionTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <random>
#include <ctime>
#include <algorithm>

using namespace std;

int main()
{
	double seed;
	int number;
	double bonus;
	double accuracy;
	while (true)
	{
		cout << "Please enter a seed: " << endl;
		cin >> seed;
		cout << "Please enter a number of iterations: " << endl;
		cin >> number;
		cout << "Please enter the accuracy: " << endl;
		cin >> accuracy;
		time_t clock;
		time(&clock);
		default_random_engine geom(clock);
		geometric_distribution<int> random(seed / accuracy); //selects integer bonus from a geometric distribution
		for (size_t i = 0; i < number; i++)
		{
			bonus = random(geom);
			bonus /= accuracy;
			cout << "Bonus " << i << ": " << bonus << endl;
		}
	}
    return 0;
}

