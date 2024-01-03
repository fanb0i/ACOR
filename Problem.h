#pragma once
#include <iostream>
#include <functional>
#include <vector>
class Problem
{
public:
	Problem(int _n_vars, double _lb, double ub, std::function<double(const std::vector<double>&)> ss);
private:
	using ObjectiveFunction = std::function<double(const std::vector<double>&)>;
	ObjectiveFunction obj_func;
	int n_vars;
	double lb;
	double ub;
};

