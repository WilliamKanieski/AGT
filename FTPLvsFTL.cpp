// FTPLvsFTL.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <random>
#include <ctime>
#include <algorithm>

using namespace std;

const int CHOICES = 1000; //Total number of iterations of the loop

vector<pair<double, double> > opponent(); //Set of costs created by the self-aware opponent

int main()
{
	vector<pair<string, double> > choices; 
	ofstream outs;
	outs.open("Actions.txt"); //Opens file to write to
	char input;
	cout << "Do you want to add bonuses? (Y/N)"; //Decides whether or not bonuses will be used
	cin >> input;
	vector<pair<double, double> > costs = opponent();
	if (input == 'y' || input == 'Y')
	{
		int bonus1;
		int bonus2;
		time_t clock;
		time(&clock);
		default_random_engine geom(clock);
		geometric_distribution<int> random(0.5); //Selects integer bonus from a geometric distribution
		bonus1 = random(geom);
		bonus2 = random(geom);
		cout << "Red bonus: " << bonus1 << endl;
		cout << "Blue bonus: " << bonus2 << endl;
		costs[0].first -= bonus1;
		costs[0].second -= bonus2;
	}
	double red_total = 0.0;
	double blue_total = 0.0;
	double total = 0.0;
	for (size_t i = 0; i < CHOICES-1; i++)
	{
		red_total += costs[i].first;
		blue_total += costs[i].second;
		if (red_total < blue_total) //Adds choices the player makes
		{
			outs << "Chose red" << endl;		  
			total += costs[i+1].first;
			choices.push_back(make_pair("red", costs[i].first));
		}
		else
		{
			outs << "Chose blue" << endl;					  
			total += costs[i+1].second;
			choices.push_back(make_pair("blue", costs[i].second));
		}
		//outs << "Choice: " << choices[i].first << endl; //Writes results to file
		//outs << "Cost: " << choices[i].second << endl;
		outs << "Red total: " << red_total << endl;
		outs << "Blue total: " << blue_total << endl;
		outs << "Player total: " << total << endl;
	}
	outs.close();
    return 0;
}

vector<pair<double, double> > opponent()
{
	vector<pair<double, double> > moves;
	double epsilon = 0.5;
	for (int i = 1; i < CHOICES; i++)
	{
	  if (i == 1) {
	    moves.push_back(make_pair(0, epsilon));
	  }
	  else if (i % 2 == 0) {
	    moves.push_back(make_pair(1, 0));
	  }
	  else {
	    moves.push_back(make_pair(0, 1));
	  }
	}
	return moves;
}

