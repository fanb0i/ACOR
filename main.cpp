
#include <iostream>
#include "ACOR.h"
double f(std::vector<double>& solution)
{
	double sum = 0;
	for (int i = 0; i < solution.size(); i++)
		sum += solution[i] * solution[i];
	return sum;
}
double fonctionRastrigin(std::vector<double> x)
{
	const double A = 10.0;
	int n = x.size();
	double sum = A * n;

	for (int i = 0; i < n; ++i) {
		sum += (x[i] * x[i] - A * cos(2.0 * M_PI * x[i]));
	}

	return sum;
}
int main()
{
	Problem* p = new Problem{30,-5.12,5.12,fonctionRastrigin,"min"};
	OriginalACOR acor(10000, 30, 20, 0.5, 1.0, 3.15);
	Agent g_best = acor.solve(p);
	std::cout << g_best.getfitness();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
