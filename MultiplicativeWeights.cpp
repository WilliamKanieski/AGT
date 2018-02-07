// MultiplicativeWeights.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <random>
#include <vector>
#include <fstream>

#define N (10000)

using namespace std;

vector<pair<double, double> > opponent();

double regrets(vector<pair<double, double> > costs, size_t t, string color, double total_cost);

double action_cost(vector<pair<double, double> > costs, size_t t, string color);

int main()
{
	vector<pair<double, double> > costs = opponent();
	vector<pair<double, double> > weights;
	vector<pair<double, double> > avg_regrets;
	default_random_engine generator(2789710);
	double total_cost = 0.0;
	double choice;
	double p_red = 1;
	double p_blue = 1;
	double r_red;
	double r_blue;
	weights.push_back(make_pair(p_red, p_blue));
	double gamma;
	double epsilon = 0.000001;
	for (size_t i = 0; i < N; i++)
	{
		gamma = p_red + p_blue;
		uniform_real_distribution<double> distribution(0.0, gamma);
		choice = distribution(generator);
		if (choice <= p_red)
		{
			total_cost += costs[i].first;
		}
		else if (choice > p_red)
		{
			total_cost += costs[i].second;
		}
		p_red = p_red * (1 - epsilon * costs[i].first);
		p_blue = p_blue * (1 - epsilon * costs[i].second);
		weights.push_back(make_pair(p_red, p_blue));
		r_red = regrets(costs, i, "red", total_cost);
		r_blue = regrets(costs, i, "blue", total_cost);
		avg_regrets.push_back(make_pair(r_red, r_blue));
	}
	ofstream outstream;
	outstream.open("regrets.txt");
	for (size_t i = 0; i < N; i++)
	{
                outstream << "t=" << i << endl;
		outstream << "Red: " << action_cost(costs, i, "red") << endl;
		outstream << "Blue: " << action_cost(costs, i, "blue") << endl;
		if (action_cost(costs, i, "red") < action_cost(costs, i, "blue")) {
		  outstream << "Regret(Red) = " << avg_regrets[i].first << endl;
		} else { //blue did best
		  outstream << "Regret(Blue) = " << avg_regrets[i].second << endl;
		}
		outstream << endl;
	}
	outstream.close();
    return 0;
}

double action_cost(vector<pair<double, double> > costs, size_t t, string color)
{
  double cost = 0.0;
  for (size_t i = 0; i < t; i++) {
    if (color == "red") cost += costs[i].first;
    else if (color == "blue") cost += costs[i].second;
  }
  return cost;
}

double regrets(vector<pair<double, double> > costs, size_t t, string color, double total_cost)
{
  return (total_cost - action_cost(costs, t, color)) / t;
}

vector<pair<double, double> > opponent()
{
	vector<pair<double, double> > moves;
	default_random_engine generator(728);
	uniform_real_distribution<double> distribution(0.0, 1.0);
	pair<double, double> choices;
	double red;
	double blue;
	for (int i = 0; i < N; i++)
	{
		red = distribution(generator);
		blue = distribution(generator);
		choices = make_pair(red, blue);
		moves.push_back(choices);
	}
	return moves;
}
