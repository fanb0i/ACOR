#include "Agent.h"
Agent::Agent() : d_solution{},d_fitness{0.0}
{
}

Agent::Agent(std::vector<double>& solution) : d_solution(solution) {
}



std::vector<double> Agent::getsolution()
{
	return d_solution;
}

double Agent::getfitness() const
{
	return d_fitness;
}


void Agent::set_fitness(double fitness)
{
	d_fitness = fitness;
}


