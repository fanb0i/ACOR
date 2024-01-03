#pragma once
#include "optimizer.h"
#include "Agent.h"
#include "Problem.h"
# define M_PI 3.14159265358979323846

class OriginalACOR {
private:
    int sample_count;
    double intent_factor;
    double zeta;
    int pop_size;
    int epoch;
    bool sort_flag;

    // Add other ACOR-specific parameters and member functions

public:
    OriginalACOR(int epoch, int pop_size, int sample_count, double intent_factor, double zeta, bool sort_flag);
    void solve(const Problem& probleme );
    void evolve(int epoch);
};