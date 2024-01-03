#pragma once
#include "Agent.h"
#include "Problem.h"
#include <vector>
#include <numeric>
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
    Agent g_best;
    Agent g_worst;


public:
    OriginalACOR(int epoch, int pop_size, int sample_count, double intent_factor, double zeta, bool sort_flag);
    double calculate_fitness(std::vector<double>& solution);
    void generate_population();
    Agent generate_agent();
    Agent generate_empty_agent();
    Agent generate_empty_agent(std::vector<double>& solution);
    void after_initialization();
    std::vector<Agent> get_sorted_population(const std::vector<Agent>& pop, const std::string& minmax);
    void get_special_agents(const std::vector<Agent>& pop, int n_best, int n_worst, const std::string& minmax, std::vector<Agent>& sorted_pop, std::vector<Agent>& best_agents, std::vector<Agent>& worst_agents);
    void solve(Problem* probleme );
    void evolve(int epoch);
};