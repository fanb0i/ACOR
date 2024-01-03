#pragma once
#include "Agent.h"
#include "Problem.h"
#include <vector>
#include <numeric>
# define M_PI 3.14159265358979323846

class OriginalACOR {
private:
    int sample_count;
    double intent_factor;
    double zeta;
    int pop_size;
    int epoch;
    bool sort_flag;
    std::vector<Agent> pop;
    Problem* d_p;


public:
    OriginalACOR(int epoch, int pop_size, int sample_count, double intent_factor, double zeta, bool sort_flag);
    double calculate_fitness(std::vector<double>& solution);
    void generate_population();
    Agent generate_agent();
    Agent generate_empty_agent();
    Agent generate_empty_agent(std::vector<double>& solution);
    void solve(Problem* probleme );
    void evolve(int epoch);
};