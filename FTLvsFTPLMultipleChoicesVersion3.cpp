// FTLvsFTPLMultipleChoicesVersion3.cpp : Defines the entry point for the console application.
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

const int CHOICES = 1000;

const double EPSILON = 0.5;

typedef vector<pair<int, double> > choices;

typedef vector<vector<double> > choice_vector;

int min_index(vector<double> list);

int main()
{
	//initializes the variables and opens the output file
	char bonuses;
	size_t number_choices;
	choices player_choices;
	ofstream outs;
	outs.open("Actions.txt");

	//asks the user how many choices the player would like and whether or not there should be bonuses
	cout << "Would you like to allocate bonuses? (Y/N)" << endl;
	cin >> bonuses;
	cout << "How many choices would you like the player to have?" << endl;
	cin >> number_choices;

	//opponent creates its cost vectors
	choice_vector opponent(CHOICES);
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
		opponent[0][i] = i * EPSILON / number_choices;
	}
	int suspected_choice = 0;
	for (size_t i = 1; i < CHOICES; i++)
	{
		opponent[i][suspected_choice] = 1;
		suspected_choice = (suspected_choice + 1) % number_choices;
	}

	//selects bonuses from a continuous geometric (exponential) distribution, chooses player's first choice
	int first_choice;
	vector<double> bonus_list(number_choices, 0.0);
	if (bonuses == 'y' || bonuses == 'Y')
	{
		time_t clock;
		time(&clock);
		default_random_engine geom(clock);
		exponential_distribution<double> random(EPSILON); 
		for (size_t i = 0; i < number_choices; i++)
		{
			bonus_list[i] = -random(geom);
		}
		first_choice = min_index(bonus_list);
	}
	else //if there are no bonuses the first choice is chosen uniformly and randomly
	{
		double chooser;
		time_t timer;
		time(&timer);
		default_random_engine generator(timer);
		uniform_real_distribution<double> distribution(0.0, 1.0);
		chooser = distribution(generator);
		for (int i = 0; i < number_choices; i++)
		{
			if (i / number_choices <= chooser && (i + 1) / number_choices > chooser)
			{
				first_choice = i;
				break;
			}
		}
	}
	
	//player gets to pay for its decisions and acts accordingly
	pair<int, double> decision;
	double regret;
	vector<vector<double> > running_totals;
	int next_choice;
	vector<double> player_totals = bonus_list;
	for (size_t i = 0; i < number_choices; i++)
	{
		player_totals[i] += opponent[0][i];
	}
	running_totals.push_back(player_totals);
	decision = make_pair(first_choice, opponent[0][first_choice]);
	player_choices.push_back(decision);
	player_totals[0] += decision.second;
	for (size_t i = 1; i < CHOICES; i++)
	{
		next_choice = min_index(player_totals);
		decision = make_pair(next_choice, opponent[i][next_choice]);
		player_choices.push_back(decision);
		player_totals[i] += decision.second;
		running_totals.push_back(player_totals);
	}

	//results are written to the output file
	for (size_t i = 0; i < CHOICES; i++)
	{
		regret = (running_totals[i][player_choices[i].first] - running_totals[i][min_index(player_totals)]) / (i + 1);
		outs << "Player's choice: " << player_choices[i].first << endl;
		outs << "Cost: " << player_choices[i].second << endl;
		outs << "-------------------------------------------------" << endl;
		for (size_t j = 0; j < number_choices; j++)
		{
			outs << "Choice " << j << " total: " << running_totals[i][j] << endl;
		}
		outs << "Player's regret: " << regret << endl;
	}
	
	outs.close();

    return 0;
}

int min_index(vector<double> list)
{
	int answer = 0;
	for (size_t i = 0; i < list.size(); i++)
	{
		if (list[i] < answer)
		{
			answer = i;
		}
	}
	return answer;
}

