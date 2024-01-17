
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

//Rosenbrock function
double rosenbrock(const std::vector<double>& x) {
    double sum = 0.0;
    for (size_t i = 0; i < x.size() - 1; ++i) {
        double term = 100.0 * pow(x[i + 1] - pow(x[i], 2), 2) + pow(1 - x[i], 2);
        sum += term;
    }
    return sum;
}

//Rastrigin function
double rastrigin(const std::vector<double>& x) {
    size_t n = x.size();
    double A = 10.0;
    double sum = A * n;

    for (size_t i = 0; i < n; ++i) {
        sum += pow(x[i], 2) - A * cos(2 * M_PI * x[i]);
    }

    return sum;
}

//Weierstrass function
double weierstrass(const std::vector<double>& x) {
    int kMax = 20;
    double a = 0.5;
    double b = 3.0;
    double sum = 0.0;

    for (int k = 0; k <= kMax; ++k) {
        sum += pow(a, k) * cos(2.0 * M_PI * pow(b, k) * (x[0] + 0.5));
    }

    return sum;
}

//Michalewicz function
double michalewicz(const std::vector<double>& x) {
    double sum = 0.0;
    double m = 10.0;
    for (size_t i = 0; i < x.size(); ++i) {
        sum += sin(x[i]) * pow(sin((i + 1) * pow(x[i], 2) / M_PI), 2 * m);
    }

    return -sum; // Minimization problem, so negating the result.
}

//Griewank
double griewank(const std::vector<double>& x) {
    double sum = 0.0;
    double product = 1.0;

    for (size_t i = 0; i < x.size(); ++i) {
        sum += pow(x[i], 2) / 4000.0;
        product *= cos(x[i] / sqrt(i + 1.0));
    }

    return 1.0 + sum - product;
}

//Ackley function
double ackley(const std::vector<double>& x) {
    const double M_E = std::exp(1.0);

    size_t n = x.size();
    double sum1 = 0.0;
    double sum2 = 0.0;

    for (size_t i = 0; i < n; ++i) {
        sum1 += pow(x[i], 2);
        sum2 += cos(2.0 * M_PI * x[i]);
    }

    return -20.0 * exp(-0.2 * sqrt(sum1 / n)) - exp(sum2 / n) + 20.0 + M_E;
}

//schwefel function
double schwefel(const std::vector<double>& x) {
    double sum = 0.0;

    for (size_t i = 0; i < x.size(); ++i) {
        sum += x[i] * sin(sqrt(fabs(x[i])));
    }

    return -sum; // Minimization problem, so negating the result.
}

#include <cmath>


int main()
{
	Problem* p = new Problem{30,-5.12,5.12,ackley,"min"};
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
