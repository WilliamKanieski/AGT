// FTLvsFTPLMultipleChoicesVersion2.cpp : Defines the entry point for the console application.
//

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

	//INIT 0: Player makes its first choice
	for (int i = 0; i < input2; i++)
	{
		if (i / input2 <= chooser && (i + 1) / input2 > chooser)
		{
			first_choice = i;
			break;
		}
	}
	player_choices[0] = first_choice;

	//INIT 1: Opponent builds initial cost vector
	for (size_t i = 0; i < input2; i++)
	{
		opponent_costs[i] = i * EPSILON / input2; //opponent intializes the first costs to manipulate the player's next decision
	}

	//INIT 2: Make sure player pays in round 0
	player_totals[first_choice] = opponent_costs[first_choice];
	opponent_totals[first_choice] = opponent_costs[first_choice];	
	
	//INIT 3: Update player and opponent totals
	for (size_t i = 0; i < input2; i++)
        {
		player_totals[i] += i * EPSILON / input2;
		opponent_totals[i] += i * EPSILON / input2;
	}


	double regret;
	for (size_t i = 1; i < CHOICES; i++)
	{
	  //Calculate regret
	  //regret = player_totals[player_choices[i]] / (i + 1) - player_totals[min_index(player_totals)] / (i + 1);
	  //outs << "Regret: " << regret << endl;

	  //PRINT ROUND NUMBER
	  outs << "\nROUND: " << i << endl;

	  //1. Print costs of each choice
	  outs << "Costs for each choice: " << endl;
	  for (size_t j = 0; j < input2; j++)
	    {
	      outs << "Choice " << j << ": " << opponent_costs[j] << endl;
	    }

	  //2. Have player make its choice (based on player totals)
	  player_choices[i] = min_index(player_totals); //what the player actually picks
	  outs << "Player choice: " << player_choices[i] << endl;

	  //3. Update player and opponent totals
	  for (size_t j = 0; j < input2; j++)
	    {
	      outs << "opponent_totals[" << j << "] = " << opponent_totals[j] << endl;
	      outs << "player_totals[" << j << "] = " << player_totals[j] << endl;		  
	      player_totals[j] += opponent_costs[j];
	      opponent_totals[j] += opponent_costs[j];
	    }
	  //outs << "min_index_opponent (round " << i << ") = " << min_index(opponent_totals) << endl;
	  //outs << "min_index_player (round " << i << ") = " << min_index(player_totals) << endl;

	  //4. Construct cost vector for next round (based on opponent totals)
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
	}

	outs.close();
    return 0;
}

int min_index(costs list)
{
	int answer = 0;
	for (size_t i = 0; i < list.size(); i++)
	{
	  if (list[i] < list[answer])
	    {
	      answer = i;
	    }
	}
	return answer;
}



