#include "Target.h"
#include <numeric>

Target::Target(const std::vector<double>& objectives, const std::vector<double>& weights, double fitness) : _objectives{ objectives }, _weights{ weights }, _fitness{ fitness }
{}

Target::Target(const vector<double>& objectives, const vector<double>& weights) : _objectives(objectives), _weights(weights), _fitness(0.0)
{}

void Target::calculateFitness(vector<double>& weights)
{
	if (weights.empty() || weights.size() != _objectives.size()) {
		weights = vector<double>(_objectives.size(), 1.0);
	}

	// Calculer la somme pondérée des objectifs
	_fitness = std::inner_product(weights.begin(), weights.end(), _objectives.begin(), 0.0);
}