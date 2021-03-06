// FollowthePerturbedLeader.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <random>
#include <ctime>
#include <algorithm>

using namespace std;

const int CHOICES = 1000; //Number of iterations in the program

const double EPSILON = 0.5; //Constant used in generating geometric distribution

vector<pair<double, double> > opponent(); //Generates uniformly distributed costs

pair<double, double> Xa(); //Generates geometrically distributed bonuses

int main()
{
	vector<pair<double, double> > costs = opponent();
	pair<double, double> bonuses = Xa();
	vector<pair<string, double> > choices;
	double red_total = 0.0;
	double blue_total = 0.0;
	double total = 0.0;
	ofstream outStream;

	cout << "Red bonus = " << bonuses.first << endl;
	cout << "Blue bonus = " << bonuses.second << endl;

	costs[0].first -= bonuses.first;
	costs[0].second -= bonuses.second;

	for (size_t i = 0; i < CHOICES; i++)
	{
		red_total += costs[i].first;
		blue_total += costs[i].second;
		if (red_total < blue_total)
		{
		  total += costs[i].first;
		  choices.push_back(make_pair("red", costs[i].first));
		}
		else
		{
		  total += costs[i].second;		  
                  choices.push_back(make_pair("blue", costs[i].second));
		}
		cout << "Total = " << total << endl;
		cout << "Total cost of Red = " << red_total << endl;;
		cout << "Total cost of Blue = " << blue_total << endl;
		
	}
	outStream.open("Actions.txt");
	for (size_t i = 0; i < CHOICES; i++)
	{
		outStream << "Choice: " << choices[i].first << endl;
		outStream << "Cost: " << choices[i].second << endl << endl;
	}
    return 0;
}

pair<double, double> Xa()
{
	pair<double, double> bonuses;
	double red, blue;
	time_t timer;
	time(&timer);
	default_random_engine generator(timer);
	uniform_real_distribution<double> distribution(0.0, 1/EPSILON);
	red = distribution(generator);
	blue = distribution(generator);
	return make_pair(red, blue);
}

vector<pair<double, double> > opponent()
{
	time_t timer;
	time(&timer);
	vector<pair<double, double> > moves;
	default_random_engine generator(timer);
	uniform_real_distribution<double> distribution(0.0, 1.0);
	pair<double, double> choices;
	double red;
	double blue;
	for (int i = 0; i < CHOICES; i++)
	{
		red = distribution(generator);
		blue = distribution(generator);
		choices = make_pair(red, blue);
		moves.push_back(choices);
	}
	return moves;
}

