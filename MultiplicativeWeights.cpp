// MultiplicativeWeights.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <random>
#include <vector>
#include <fstream>

using namespace std;

vector<pair<double, double> > opponent();

double w_plus(double weight, double cost);

double regrets(vector<pair<double, double> > costs, size_t t, string color, double total_cost);

int main()
{
	vector<pair<double, double> > costs = opponent();
	vector<pair<double, double> > weights;
	vector<pair<double, double> > avg_regrets;
	default_random_engine generator;
	double total_cost = 0.0;
	double choice;
	bool color;
	double p_red = 1;
	double p_blue = 1;
	double r_red;
	double r_blue;
	weights.push_back(make_pair(p_red, p_blue));
	double gamma;
	for (size_t i = 0; i < 1000; i++)
	{
		gamma = p_red + p_blue;
		uniform_real_distribution<double> distribution(0.0, gamma);
		choice = distribution(generator);
		if (choice <= p_red)
		{
			total_cost += costs[i].first;
			color = true;
			if (costs[i].first >= costs[i].second)
			{
				p_red /= 2;
				p_blue = w_plus(p_blue, costs[i].second);
			}
			else
			{
				p_blue /= 2;
				p_red = w_plus(p_red, costs[i].first);
			}
		}
		else if (choice > p_red)
		{
			total_cost += costs[i].second;
			color = false;
			if (costs[i].first >= costs[i].second)
			{
				p_red /= 2;
				p_blue = w_plus(p_blue, costs[i].second);
			}
			else
			{
				p_blue /= 2;
				p_red = w_plus(p_red, costs[i].first);
			}
		}
		weights.push_back(make_pair(p_red, p_blue));
		r_red = regrets(costs, i, "red", total_cost);
		r_blue = regrets(costs, i, "blue", total_cost);
		avg_regrets.push_back(make_pair(r_red, r_blue));
	}
	ofstream outstream;
	outstream.open("regrets.txt");
	for (size_t i = 0; i < 1000; i++)
	{
		outstream << "Red: " << avg_regrets[i].first << endl;
		outstream << "Blue: " << avg_regrets[i].second << endl << endl;
	}
	outstream.close();
    return 0;
}

double w_plus(double weight, double cost)
{
	return weight * pow((1 - 0.5 * cost), cost);
}

double regrets(vector<pair<double, double> > costs, size_t t, string color, double total_cost)
{
	double sigma_cta = 0.0;
	if (color == "red")
	{
		for (size_t j = 0; j < t; j++)
		{
			sigma_cta += costs[j].first;
		}
	}
	else if (color == "blue");
	{
		for (size_t j = 0; j < t; j++)
		{
			sigma_cta += costs[j].second;
		}
	}
	return (sigma_cta - total_cost) / t;
}

vector<pair<double, double> > opponent()
{
	vector<pair<double, double> > moves;
	default_random_engine generator;
	uniform_real_distribution<double> distribution(0.0, 1.0);
	pair<double, double> choices;
	double red;
	double blue;
	for (int i = 0; i < 1000; i++)
	{
		red = distribution(generator);
		blue = distribution(generator);
		choices = make_pair(red, blue);
		moves.push_back(choices);
	}
	return moves;
}
