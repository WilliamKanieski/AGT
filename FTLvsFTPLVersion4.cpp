// FTLvsFTPLVersion4.cpp : Defines the entry point for the console application.
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

const int CHOICES = 1000; //Total number of iterations of the loop

const double EPSILON = 0.5;

vector<vector<double> > opponent(size_t number_choices); //Set of costs created by the self-aware opponent

int main()
{
	vector<pair<string, double> > choices;
	ofstream outs;
	outs.open("Actions.txt"); //Opens file to write to
	char have_bonuses;
	size_t number_choices;
	cout << "How many choices do you want?" << endl;
	cin >> number_choices;
	cout << "Do you want to add bonuses? (Y/N)" << endl; //Decides whether or not bonuses will be used
	cin >> have_bonuses;
	vector<vector<double> > costs = opponent(number_choices);
	vector<double> player_totals;
	if (have_bonuses == 'y' || have_bonuses == 'Y')
	{
		vector<double> bonuses;
		double random_bonus;
		size_t bonus_counter = 0;
		time_t clock;
		time(&clock);
		default_random_engine geom(clock);
		exponential_distribution<double> random(0.5); //Selects integer bonus from a geometric distribution
		for (size_t i = 0; i < number_choices; i++)
		{
			random_bonus = random(geom);
			bonuses.push_back(random_bonus);
			costs[0][i] += random_bonus;
		}
		for (size_t i = 0; i < number_choices; i++)
		{
			cout << "Bonus " << i << ": " << bonuses[i] << endl;
		}
		player_totals = bonuses;
	}
	else
	{
		vector<double> bonuses(number_choices, 0);
		player_totals = bonuses;
	}



	//----------------------------------------------------------------------------------------------------------------


	double chooser;
	int first_choice;
	time_t timer;
	time(&timer);
	default_random_engine generator(timer);
	uniform_real_distribution<double> distribution(0.0, 1.0);
	chooser = distribution(generator);
	first_choice = static_cast<int>(chooser * (number_choices - 1));
	cout << first_choice << endl;


	//----------------------------------------------------------------------------------------------------------------


	double total_player_cost = costs[0][first_choice];

	outs << "Round 0: " << endl;
	outs << "Player's choice: " << first_choice << endl;
	outs << "Player's total payment: " << total_player_cost << endl;
	outs << "Player's total regret: " << costs[0][first_choice] << endl;
	outs << "----------------------------------------------------" << endl << endl;

	size_t min_index;
	for (size_t i = 1; i < CHOICES; i++) //Zero was added in for debugging, was originally a one
	{
		for (size_t j = 0; j < number_choices; j++)
		{
			player_totals[j] += costs[i][j];
			cout << costs[i][j] << " "; //debugging
		}
		min_index = 0;
		for (size_t j = 0; j < number_choices; j++)
		{
			if (player_totals[j] < player_totals[min_index])
			{
				min_index = j;
			}
		}
		cout << " : " << costs[i][min_index] << " : " << min_index;
		cout << endl; //debugging
		total_player_cost += costs[i][(min_index - 1) % number_choices];
		outs << costs[i][min_index] << endl;
		outs << "Round " << i << ": " << endl;
		outs << "Player's choice: " << min_index << endl;
		outs << "Player's total payment: " << total_player_cost << endl;
		outs << "Player's total regret: " << total_player_cost - (i / number_choices + i % number_choices * EPSILON / (number_choices - 1)) << endl;
		outs << i / number_choices + i % number_choices * EPSILON / (number_choices - 1) << endl;
		outs << "----------------------------------------------------" << endl << endl;
	}

	outs.close();

    return 0;
}

vector<vector<double> > opponent(size_t number_choices)
{
	vector<vector<double> > opponent(CHOICES);
	for (size_t i = 0; i < CHOICES; i++)
	{
		opponent[i].resize(number_choices);
		for (size_t j = 0; j < number_choices; j++)
		{
			opponent[i][j] = 0;
		}
	}
	for (size_t i = 0; i < number_choices; i++) //first cost vector where opponent tries to manipulate player's future choices
	{
		opponent[0][i] = i * EPSILON / (number_choices - 1);
		//cout << opponent[0][i] << endl;
	}
	int suspected_choice = 0;
	for (size_t i = 1; i < CHOICES; i++) //opponent puts ones for the costs it thinks the player will choose and makes the rest zeros
	{
		opponent[i][suspected_choice] = 1;
		//cout << suspected_choice << endl;
		suspected_choice = (suspected_choice + 1) % number_choices;
	}
	for (size_t i = 0; i < CHOICES; i++) //Debugging
	{
		for (size_t j = 0; j < number_choices; j++)
		{
			cout << opponent[i][j] << " ";
		}
		cout << endl;
	}
	return opponent;
}

