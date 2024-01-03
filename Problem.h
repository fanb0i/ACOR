#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <random>

class Problem
{
public:
	Problem(int _n_vars, double _lb, double ub, std::function<double(const std::vector<double>&)> ss,std::string _min_max);
	std::vector<double> generate_solution();
	double calculate_fitness(std::vector<double>& solution);
	int n_dims();
	std::string minmax();
private:
	using ObjectiveFunction = std::function<double(std::vector<double>&)>;
	ObjectiveFunction obj_func;
	int n_vars;
	double lb;
	double ub;
	std::string min_max;

};

