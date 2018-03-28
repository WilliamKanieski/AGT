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
	char have_bonuses;
	int number_choices;
	ofstream outs;
	outs.open("Actions.txt");
	cout << "Do you want bonuses? (Y/N) ";
	cin >> have_bonuses;
	cout << "How many choices do you want? ";
	cin >> number_choices;
	costs opponent_costs(number_choices);
	choices player_choices(CHOICES);
	costs player_costs(CHOICES);
	costs player_totals(number_choices, 0.0);
	costs opponent_totals(number_choices, 0.0);

	//chooses player's first move
	int first_choice;
	double chooser;
	time_t timer;
	time(&timer);
	default_random_engine generator(timer);
	uniform_real_distribution<double> distribution(0.0, 1.0);
	chooser = distribution(generator);

	//allocates bonuses
	if (have_bonuses == 'Y' || have_bonuses == 'y')
	{
		double bonus;
		time_t clock;
		time(&clock);
		default_random_engine geom(clock);
		geometric_distribution<int> random(EPSILON / 1000000.0); //selects integer bonus from a geometric distribution
		for (size_t i = 0; i < number_choices; i++)
		{
			bonus = random(geom);
			bonus /= 1000000.0;
			player_totals[i] = -bonus;
			cout << "Bonus " << i << ": " << bonus << endl;
		}
	}

	//player makes its first choice
	for (int i = 0; i < number_choices; i++)
	{
		if (i / number_choices <= chooser && (i + 1) / number_choices > chooser)
		{
			first_choice = i;
			break;
		}
	}
	player_choices[0] = first_choice;

	for (size_t i = 0; i < number_choices; i++)
	{
		opponent_costs[i] = i * EPSILON / number_choices; //opponent intializes the first costs to manipulate the player's next decision
	}

	//player pays in the first round
	player_totals[first_choice] = opponent_costs[first_choice];
	opponent_totals[first_choice] = opponent_costs[first_choice];

	//opponent and player play against one another
	for (size_t i = 0; i < number_choices; i++)
	{
		player_totals[i] += i * EPSILON / number_choices;
		opponent_totals[i] += i * EPSILON / number_choices;
	}

	//opponent and player play against one another
	for (size_t i = 0; i < number_choices; i++)
	{
		player_totals[i] += i * EPSILON / number_choices;
		opponent_totals[i] += i * EPSILON / number_choices;
	}

	double regret;
	for (size_t i = 1; i < CHOICES; i++)
	{
		regret = player_totals[player_choices[i]] / (i + 1) - player_totals[min_index(player_totals)] / (i + 1);
		outs << "Player choice: " << player_choices[i] << endl;
		outs << "Costs for each choice: " << endl;
		for (size_t j = 0; j < number_choices; j++)
		{
			outs << "Choice " << j << ": " << opponent_costs[j] << endl;
		}
		outs << "Regret: " << regret << endl;
		player_choices[i] = min_index(player_totals); //what the player actually picks
		for (size_t j = 0; j < number_choices; j++)
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
		for (size_t j = 0; j < number_choices; j++)
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



