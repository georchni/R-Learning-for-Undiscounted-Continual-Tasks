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
	int I;
	int P;
	int action;
	double epsilon;
	double alpha;
	double beta;
	double average_reward;
	double reward;
	bool greedy_yes;
	int s1;	
	int s2;
	int s1_tonos;
	int s2_tonos;
	double max_Q_sa;
	double max_Q_sa_new;

	vector< vector<actions> > Q_sa;

	RL()
	{
		average_reward = 0.0;
		alpha = 0.1;
		beta = 0.02;
		epsilon = 0.1;
		action = 0;
		reward = 0.0;
		I = 0;
		P = 0;
		max_Q_sa = 0.0;
		max_Q_sa_new = 0.0;
	}

	void init_RL(int x,int y)
	{
		Q_sa.resize(x);
		for(unsigned int i=0; i<Q_sa.size(); i++)
		{
			Q_sa[i].resize(y);
		}
		I = y;
		P = x;
		for(unsigned int i =0;i<Q_sa.size();i++)
		{
			for(unsigned int j=0;j<Q_sa[i].size();j++)
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
		double rnd=RL_rnd->Random();
		double decay_factor = 0.999;

		if(rnd>epsilon)
		{
			//cout<<endl<<"random "<<rnd<<endl;
			
		
			if(s2 == 0)
			{
				if (Q_sa[s1][s2].b > Q_sa[s1][s2].c)
			    {
					action = 2;
				}
				else 
				{
					action = 0;
				}
			}
			else
		    {
				if (Q_sa[s1][s2].a >max_Q_sa)
				{
					action = 1;
			    }
				else if (Q_sa[s1][s2].b > max_Q_sa)
				{
					action = 2;
				}
				else 
				{
					action = 0;
				}
			}
			greedy_yes = 1;
			epsilon = epsilon * decay_factor;
		}
		else if(rnd<epsilon)
		{
			if ((s2 == 0) && (action == 2))
				action = RL_rnd->IRandom(0,1);
			else if ((s2<=I) && (action==1))
			{
				action = RL_rnd->IRandom(0,1);
				{
					if (action == 1)
						action++;
				}
			}
			epsilon = epsilon * decay_factor;
		}
		return action;
	}

	double find_max_Qvalue(int s_1, int s_2)
	{
		s1 = s_1;
		s2 = s_2;

		if (Q_sa[s1][s2].a > max_Q_sa)
		{
			 max_Q_sa = Q_sa[s1][s2].a;}
		else if (Q_sa[s1][s2].b > max_Q_sa)
		{
			 max_Q_sa = Q_sa[s1][s2].b;}
		else 
		{
			 max_Q_sa = Q_sa[s1][s2].c;}
		return max_Q_sa;
	}
	
	double find_max_Qvalue_tonos(int s_1_new, int s_2_new)
	{
		s1_new = s_1_new;
		s2_new = s_2_new;

		if (Q_sa[s1_new][s2_new].a > max_Q_sa_new)
		{
			max_Q_sa_new = Q_sa[s1_new][s2_new].a;
		}
		else if (Q_sa[s1_new][s2_new].b > max_Q_sa_new)
		{
			max_Q_sa_new = Q_sa[s1_new][s2_new].b;
		}
		else 
		{
			max_Q_sa_new = Q_sa[s1_new][s2_new].c;
		}
		return max_Q_sa_new;
	}

	double update_Q_sa_value(double r)
	{
		reward = r;
		if (action == 1)
		{
			Q_sa[s1][s2].a = (1 - alpha) * Q_sa[s1][s2].a + alpha * (reward + max_Q_sa_new - average_reward);}
		else if (action == 2)
		{
			Q_sa[s1][s2].b = (1 - alpha) * Q_sa[s1][s2].b + alpha * (reward + max_Q_sa_new - average_reward);}
		else 
		{
			Q_sa[s1][s2].c = (1 - alpha) * Q_sa[s1][s2].c + alpha * (reward + max_Q_sa_new - average_reward);}

		if (greedy_yes == 1)
		{
			if (action == 1)
			{
				average_reward = (1 - beta) * average_reward + beta * (r + max_Q_sa_new - Q_sa[s1][s2].a);}
			else if (action == 2)
			{
				average_reward = (1 - beta) * average_reward + beta * (r + max_Q_sa_new - Q_sa[s1][s2].b);}
			else 
			{
				average_reward = (1 - beta) * average_reward + beta * (r + max_Q_sa_new - Q_sa[s1][s2].c);}
		}
		return average_reward;
	}

	void print_Q_sa()
	{
		ofstream arxeio;   arxeio.open("RL_policy.txt",ios::out);

		for(unsigned int i=0;i<Q_sa.size();i++)
		{
			for(unsigned int j=0;j<Q_sa[i].size();j++)
			{
				arxeio<<Q_sa[i][j].a<<"	"<<Q_sa[i][j].b<<"	"<<Q_sa[i][j].c<<"	";}
			arxeio<<endl<<endl;
		}
		arxeio.close();
	}
};

