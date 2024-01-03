#include "Problem.h"

Problem::Problem(int _n_vars, double _lb, double _ub, std::function<double(const std::vector<double>&)> ss,std::string _min_max):n_vars{ _n_vars }, lb{ _lb }, ub{_ub}, obj_func{ss},min_max(_min_max)
{
}

std::vector<double> Problem::generate_solution()
{
    std::vector<double> solutions(n_vars);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(lb, ub);

    for (int i = 0; i < n_vars; ++i)
    {
        solutions[i] = dis(gen);
    }

    return solutions;
}

double Problem::calculate_fitness(std::vector<double>& solution)
{
    return obj_func(solution);
}

int Problem::n_dims()
{
    return n_vars;
}

std::string Problem::minmax()
{
    return min_max;
}
