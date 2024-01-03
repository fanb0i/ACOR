
#include <iostream>
#include "ACOR.h"
double f(std::vector<double>& solution)
{
	double sum = 0;
	for (int i = 0; i < solution.size(); i++)
		sum += solution[i] * solution[i];
	return sum;
}
int main()
{
	Problem* p = new Problem{ 30,-2,10,f,"min" };
	OriginalACOR acor(1000, 50, 25, 0.5, 1, 1);
	Agent g_best = acor.solve(p);
	std::cout << g_best.getfitness();
}