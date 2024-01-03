#pragma once
#include <vector>
#include <iostream>
class Agent {
public:
	Agent();
	Agent(std::vector<double>& solution);
	void set_fitness(double fitness);
	double getfitness() const;
	std::vector<double> getsolution();
private:
	std::vector<double> d_solution;
	double d_fitness;
};