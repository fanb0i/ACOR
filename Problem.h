#pragma once
#include <iostream>
#include <functional>
#include <vector>
class Problem
{
public:
	Problem();
private:
	using ObjectiveFunction = std::function<double(const std::vector<double>&)>;
	int n_vars;
	double lb;
	double ub;
};

