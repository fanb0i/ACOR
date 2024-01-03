#pragma once
#include <vector>

using namespace std;

class Target
{
public:
	Target(const vector<double>& objectives, const vector<double>& weights, double fitness);
	Target(const vector<double>& objectives, const vector<double>& weights);
	void calculateFitness(std::vector<double>& weights);
private:
	std::vector<double> _objectives;
	std::vector<double> _weights;
	double _fitness;
};

