#include "Problem.h"

Problem::Problem(int _n_vars, double _lb, double _ub, std::function<double(const std::vector<double>&)> ss):n_vars{ _n_vars }, lb{ _lb }, ub{_ub}, obj_func{ss}
{
}