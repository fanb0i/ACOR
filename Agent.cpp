#include "Agent.h"

Agent::Agent(std::vector<double>& solution):d_solution{solution}
{
}


std::vector<double> Agent::getsolution()
{
	return d_solution;
}


void Agent::set_fitness(double fitness)
{
	d_fitness = fitness;
}

Agent::Agent()
{

}
