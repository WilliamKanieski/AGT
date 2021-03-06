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

pair<int, double> min_index(vector<double> list);

void file_write(vector<double> list, ofstream& outs);

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
	for (size_t i = 1; i < CHOICES; i++) //opponent puts ones for the costs it thinks the player will choose and makes the rest zeros
	{
		opponent[i][suspected_choice] = 1;
		suspected_choice = (suspected_choice + 1) % number_choices;
	}

	//selects bonuses from a continuous geometric (exponential) distribution, chooses player's first choice
	pair<int, double> first_choice;
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
		for (size_t i = 0; i < number_choices; i++)
		{
			if (i / number_choices <= chooser && (i + 1) / number_choices > chooser)
			{
				first_choice = make_pair(i, opponent[0][i]);
				break;
			}
		}
	}
	
	//player gets to pay for its decisions and acts accordingly
	pair<int, double> decision;
	double regret;
	int next_choice;
	vector<pair<int, double> > player_totals(number_choices); 
	for (size_t i = 0; i < number_choices; i++)
	{
		player_totals[i] = make_pair(i, bonus_list[i]);
	}
	cout << "Code runs beyond this line." << endl;
	for (size_t i = 0; i < number_choices; i++)
	{
		player_totals[i].first += opponent[0][i];
	}
	decision = make_pair(first_choice.first, opponent[0][first_choice.first]);
	player_choices.push_back(decision);
	player_totals[decision.first].second += decision.second;
	regret = player_totals[player_choices[0].first].second - player_totals[min_index(player_totals).first].second;
	outs << "Iteration 0" << endl;
	outs << "Player's choice: " << player_choices[0].first << endl;
	outs << "Cost: " << player_choices[0].second << endl;
	outs << "-------------------------------------------------" << endl;
	file_write(player_totals, outs);
	outs << "Player's regret: " << regret << endl;
	outs << "-------------------------------------------------" << endl;
	next_choice = min_index(player_totals).first;
	for (size_t i = 1; i < CHOICES; i++)
	{
		decision = make_pair(next_choice, opponent[i][next_choice]);
		player_choices.push_back(decision);
		player_totals[i].second += decision.second;
		regret = player_totals[player_choices[i].first] - player_totals[min_index(player_totals).first];
		outs << "Iteration " << i << endl;
		outs << "Player's choice: " << player_choices[i].first << endl;
		outs << "Cost: " << player_choices[i].second << endl;
		outs << "-------------------------------------------------" << endl;
		file_write(player_totals, outs);
		outs << "Player's regret: " << regret << endl;
		outs << "-------------------------------------------------" << endl;
		next_choice = min_index(player_totals).first;
	}

	
	outs.close();

    return 0;
}

pair<int, double> min_index(vector<pair<int, double> > list)
{
	int answer = 0;
	for (size_t i = 0; i < list.size(); i++)
	{
		if (list[i].second < list[answer].second)
		{
			answer = i;
		}
	}
	return make_pair(answer, list[answer].second);
}

void file_write(vector<pair<int, double> > list, ofstream& outs)
{
	for (size_t i = 0; i < list.size(); i++)
	{
		outs << "Choice " << i << " total: " << list[i].second << endl;
	}
}

