// FTLvsFTPLmultiplechoices.cpp : Defines the entry point for the console application.
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

typedef vector<pair<int, double> > choices;

typedef vector<vector<double> > costs;

typedef vector<double> total;

const int CHOICES = 1000;

const double EPSILON = 0.5;

int main()
{
	//initializes variables and structures the program based on user input
	char input1;
	int input2;
	choices player;
	ofstream outs;
	outs.open("Actions.txt");
	cout << "Do you want bonuses? (Y/N) ";
	cin >> input1;
	cout << "How many choices do you want?";
	cin >> input2;
	costs opponent(input2);

	//initializes all costs to 0
	for (size_t i = 0; i < input2; i++)
	{
		opponent[i].resize(CHOICES);
		for (size_t j = 0; j < CHOICES; j++)
		{
			opponent[i][j] = 0.0;
		}
	}
	total player_totals(input2);
	total opponent_totals(input2);

	//opponent uses EPSILON to manipulate the player's second choice
	for (size_t i = 0; i < input2; i++)
	{
		opponent[i].resize(CHOICES);
		opponent[i][0] = EPSILON * i / input2;
		opponent_totals[i] = EPSILON * i / input2;
		player_totals[i] += EPSILON * i / input2;
	}

	//chooses player's first move
	int first_choice;
	double chooser;
	time_t timer;
	time(&timer);
	default_random_engine generator(timer);
	uniform_real_distribution<double> distribution(0.0, 1.0);
	chooser = distribution(generator);
	for (int i = 0; i < input2; i++)
	{
		if (i / input2 <= chooser && (i + 1) / chooser > first_choice)
		{
			first_choice = i;
			player.push_back(make_pair(first_choice, opponent[first_choice][0]));
			break;
		}
	}

	//allocates bonuses
	if (input1 == 'Y' || input1 == 'y')
	{
		int bonus;
		time_t clock;
		time(&clock);
		default_random_engine geom(clock);
		geometric_distribution<int> random(0.5); //Selects integer bonus from a geometric distribution
		for (size_t i = 0; i < input2; i++)
		{
			bonus = random(geom);
			player_totals[i] = -bonus;
		}
	}

	//opponent uses binary costs for the rest of the game
	int min_choice;
	for (size_t i = 1; i < CHOICES; i++)
	{
		min_choice = player_totals[i];
		opponent[i % input2][i] = 1;
		opponent_totals[i % input2] += 1;
		player_totals[i % input2] += 1;
		for (size_t j = 1; j < input2; j++)
		{
			if (player_totals[j] < min_choice)
			{
				min_choice = player_totals[j];
			}
		}
		player.push_back(make_pair(min_choice, opponent[min_choice]));
	}

	for (size_t i = 0; i < CHOICES; i++)
	{
		outs << "Player choice: " << player[i].first << endl;
		outs << "Player payment: " << player[i].second << endl;
	}

	outs.close();

    return 0;
}

