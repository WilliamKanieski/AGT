// FTLvsFTPLMultipleChoicesVersion2.cpp : Defines the entry point for the console application.
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

const int CHOICES = 1000;

const double EPSILON = 0.5;

typedef vector<int> choices;

typedef vector<double> costs;

int min_index(costs list);

int main()
{
	//initializes variables and structures the program based on user input
	char input1;
	int input2;
	ofstream outs;
	outs.open("Actions.txt");
	cout << "Do you want bonuses? (Y/N) ";
	cin >> input1;
	cout << "How many choices do you want? ";
	cin >> input2;
	costs opponent_costs(input2);
	choices player_choices(CHOICES);
	costs player_costs(CHOICES);
	costs player_totals(input2, 0.0);
	costs opponent_totals(input2, 0.0);

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
		if (i / input2 <= chooser && (i + 1) / input2 > chooser)
		{
			first_choice = i;
			break;
		}
	}
	player_choices[0] = first_choice;

	//allocates bonuses
	if (input1 == 'Y' || input1 == 'y')
	{
		int bonus;
		time_t clock;
		time(&clock);
		default_random_engine geom(clock);
		geometric_distribution<int> random(0.5); //selects integer bonus from a geometric distribution
		for (size_t i = 0; i < input2; i++)
		{
			bonus = random(geom);
			player_totals[i] = -bonus;
			cout << "Bonus " << i << ": " << bonus << endl;
		}
	}

	//opponent and player play against one another
	for (size_t i = 0; i < input2; i++)
	{
		opponent_costs[i] = i * EPSILON / input2; //opponent intializes the first costs to manipulate the player's next decision
		player_totals[i] += i * EPSILON / input2;
		opponent_totals[i] += i * EPSILON / input2;
	}

	double regret;
	for (size_t i = 1; i < CHOICES; i++)
	{
		regret = player_totals[player_choices[i]] / (i + 1) - player_totals[min_index(player_totals)] / (i + 1);
		outs << "Player choice: " << player_choices[i] << endl;
		outs << "Costs for each choice: " << endl;
		for (size_t j = 0; j < input2; j++)
		{
			outs << "Choice " << j << ": " << opponent_costs[j] << endl;
		}
		outs << "Regret: " << regret << endl;
		player_choices[i] = min_index(player_totals); //what the player actually picks
		for (size_t j = 0; j < input2; j++)
		{
			if (j == min_index(opponent_totals)) //what the opponent thinks the player will pick
			{
				opponent_costs[j] = 1.0;
			}
			else
			{
				opponent_costs[j] = 0.0;
			}
		}
		for (size_t j = 0; j < input2; j++)
		{
			player_totals[j] += opponent_costs[j];
			opponent_totals[j] += opponent_costs[j];
		}
	}

	outs.close();
    return 0;
}

int min_index(costs list)
{
	int answer = list[0];
	for (size_t i = 0; i < list.size(); i++)
	{
		if (list[i] < answer)
		{
			answer = i;
		}
	}
	return answer;
}



