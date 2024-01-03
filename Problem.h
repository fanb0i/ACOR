#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <random>

class Problem
{
public:
	Problem(int _n_vars, double _lb, double ub, std::function<double(const std::vector<double>&)> ss,bool _min_max);
	std::vector<double> generate_solution();
	double calculate_fitness(std::vector<double>& solution);
	int n_dims();
	bool minmax();
private:
	using ObjectiveFunction = std::function<double(const std::vector<double>&)>;
	ObjectiveFunction obj_func;
	int n_vars;
	double lb;
	double ub;
	bool min_max;

};

