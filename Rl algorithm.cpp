#include <vector>
#include <iostream>
#include <fstream>    
#include <algorithm>
#include <time.h>
#include "mersenne.cpp"
#include "randomc.h"
#include <cstdlib>

using namespace std;

class actions
{
public:
	double a,b,c;
};

class RL
{
public:
	int I; //1st variable that defines the state
	int P;//2nd variable that defines the state
	int action;
	double epsilon;
	double alpha;
	double beta;
	double average_reward;
	double reward;
	bool greedy_yes;
	int s1;	
	int s2;
	int s1_new;
	int s2_new;
	double max_Q_sa;
	double max_Q_sa_tonos;
	double decay_factor;

	vector< vector<actions> > Q_sa;

	RL()
	{
	    //The_parameters_values_need_tuning
	    
		average_reward=0.0;
		alpha=0.05;
		beta=0.02;
		epsilon=0.5;
		action=0;
		reward=0.0;
		I=0;
		P=0;
		max_Q_sa=0.0;
		max_Q_sa_tonos=0.0;
		decay_factor=0.9999;
	}

	void init_RL(int x,int y)
	{
		
		Q_sa.resize(x);

		for(unsigned int i =0; i<Q_sa.size(); i++)
		{
			Q_sa[i].resize(y);}
		I=y;
		P=x;

		for(unsigned int i =0; i<Q_sa.size(); i++)
		{
			for(unsigned int j=0; j<Q_sa[i].size(); j++)
			{
				Q_sa[i][j].a = 0;	
				Q_sa[i][j].b = 0;	
				Q_sa[i][j].c = 0;
			}
			cout<<endl;
		}
	}

    int select_actions(int s_1,int s_2, TRandomMersenne* RL_rnd)
	{
		s1 = s_1;
		s2 = s_2;
		double rnd = RL_rnd->Random();

		if(rnd > epsilon)
		{			
			if(s2 == 0)
			{
				if (Q_sa[s1][s2].b > Q_sa[s1][s2].c)
			    {
					action = 2;}
				else 
				{
					action = 0;}
			}

			else 
		    {
				if (Q_sa[s1][s2].b >= max_Q_sa)
				{
					action = 2;}
				else if (Q_sa[s1][s2].a >= max_Q_sa)
				{
					action = 1;}
				else
				{
					action = 0;}
				
			}
			greedy_yes = 1;
			//epsilon=epsilon*decay_factor;
		}
		else if(rnd < epsilon)
		{
			if (s2 == 0)
				action = RL_rnd->IRandom(0,1);
			else
			{
				action = RL_rnd->IRandom(0,1);
				{
					if (action == 1)
						action++;
				}
			}
				greedy_yes = 0;
		}
		return action;
	}

	double find_max_Qvalue(int s_1, int s_2)
	{
		s1 = s_1;
		s2 = s_2;

		if (Q_sa[s1][s2].a >= max_Q_sa)
		{
			 max_Q_sa = Q_sa[s1][s2].a;}
		else if (Q_sa[s1][s2].b >= max_Q_sa) 
		{
			 max_Q_sa = Q_sa[s1][s2].b;}
		else 
		{
			 max_Q_sa = Q_sa[s1][s2].c;}
		return max_Q_sa;
	}
	
	double find_max_Qvalue_tonos(int s_1_new, int s_2_new)
	{
		s1_tonos = s_1_new;
		s2_tonos = s_2_new;

		if (Q_sa[s1_new[s2_new].a >= max_Q_sa_new)
		{
			max_Q_sa_new = Q_sa[s1_new][s2_new].a;}
		else if (Q_sa[s1_new][s2_new].b >= max_Q_sa_new)
		{
			max_Q_sa_new = Q_sa[s1_new][s2_new].b;}
		else 
		{
			max_Q_sa_new = Q_sa[s1_new][s2_new].c;}
		return max_Q_sa_new;
	}

	double update_Q_sa_value(double r)
	{
		reward = r;	
		if (action == 1)
		{
		    Q_sa[s1][s2].a = Q_sa[s1][s2].a + alpha * (reward - average_reward + max_Q_sa_new - Q_sa[s1][s2].a);}
		else if (action == 2)
		{
			Q_sa[s1][s2].b = Q_sa[s1][s2].b + alpha * (reward - average_reward + max_Q_sa_new - Q_sa[s1][s2].b);}
		else 
		{
			Q_sa[s1][s2].c = Q_sa[s1][s2].c + alpha * (reward - average_reward + max_Q_sa_new - Q_sa[s1][s2].c);}
		if (greedy_yes == 1)
		{
			average_reward = average_reward + beta * (reward - average_reward + max_Q_sa_new - max_Q_sa);}

		return average_reward;
	}
	
	void print_Q_sa()
	{
	    ofstream arxeio;   arxeio.open("RL_policy.txt",ios::out);
		
		for(unsigned int i=0; i<Q_sa.size(); i++)
		{
			for(unsigned int j=0; j<Q_sa[i].size(); j++)
			{
				arxeio<<Q_sa[i][j].a<<"	"<<Q_sa[i][j].b<<"	"<<Q_sa[i][j].c<<"	";
			}
			arxeio<<endl<<endl;
		}
		arxeio.close();
	}
};

