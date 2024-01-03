#pragma once
#include "Agent.h"
#include "Problem.h"
#include <vector>
#include <numeric>
#include <tuple>
#include <algorithm>

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
    void after_initialization();
    std::tuple<std::vector<Agent>, std::vector<Agent>, std::vector<Agent>> get_special_agents(
        std::vector<Agent>& pop, int n_best = 3, int n_worst = 3, const std::string& minmax = "min");
    std::vector<Agent> get_sorted_population(std::vector<Agent>& pop, const std::string& minmax);
    void solve(Problem* probleme );
    void evolve(int epoch);
};