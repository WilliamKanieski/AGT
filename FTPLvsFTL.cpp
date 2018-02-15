// FTPLvsFTL.cpp : Defines the entry point for the console application.
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

const int CHOICES = 1000; //Total number of iterations of the loop

vector<pair<double, double> > opponent(); //Set of costs created by the self-aware opponent

int main()
{
	vector<pair<string, double> > choices; 
	ofstream outs;
	outs.open("Actions.txt"); //Opens file to write to
	char input;
	cout << "Do you want to add bonuses? (Y/N)"; //Decides whether or not bonuses will be used
	cin >> input;
	vector<pair<double, double> > costs = opponent();
	if (input == 'y' || input == 'Y')
	{
		int bonus1;
		int bonus2;
		time_t clock;
		time(&clock);
		default_random_engine geom(clock);
		geometric_distribution<int> random(0.5); //Selects integer bonus from a geometric distribution
		bonus1 = random(geom);
		bonus2 = random(geom);
		cout << "Red bonus: " << bonus1 << endl;
		cout << "Blue bonus: " << bonus2 << endl;
		costs[0].first -= bonus1;
		costs[0].second -= bonus2;
	}
	double red_total = 0.0;
	double blue_total = 0.0;
	double total = 0.0;
	for (size_t i = 0; i < CHOICES; i++)
	{
		red_total += costs[i].first;
		blue_total += costs[i].second;
		if (red_total < blue_total) //Adds choices the player makes
		{
			total += costs[i].first;
			choices.push_back(make_pair("red", costs[i].first));
		}
		else
		{
			total += costs[i].second;
			choices.push_back(make_pair("blue", costs[i].second));
		}
		outs << "Choice: " << choices[i].first << endl; //Writes results to file
		outs << "Cost: " << choices[i].second << endl;
		outs << "Red total: " << red_total << endl;
		outs << "Blue total: " << blue_total << endl;
		outs << "Player total: " << total << endl;
	}
	outs.close();
    return 0;
}

vector<pair<double, double> > opponent()
{
	time_t timer;
	time(&timer);
	vector<pair<double, double> > moves;
	default_random_engine generator(timer);
	uniform_real_distribution<double> distribution(0.0, 1.0); //Generates uniform distribution of costs
	pair<double, double> choices;
	double red;
	double blue;
	double small;
	double large;
	red = distribution(generator); //First two costs are generated uniformly and randomly
	blue = distribution(generator);
	choices = make_pair(red, blue);
	moves.push_back(choices);
	double red_total = red;
	double blue_total = blue;
	for (int i = 1; i < CHOICES; i++)
	{
		red = distribution(generator); //Generates two random costs and assigns the larger one to the choice that had previously had a lower cost in an attempt to trick the player
		blue = distribution(generator);
		small = min(red, blue);
		large = max(red, blue);
		if (red_total < blue_total)
		{
			choices = make_pair(large, small);
		}
		else
		{
			choices = make_pair(small, large);
		}
		moves.push_back(choices);
		red_total += red;
		blue_total += blue;
	}
	return moves;
}

