#include "ACOR.h"

OriginalACOR::OriginalACOR(int epoch, int pop_size, int sample_count, double intent_factor, double zeta, bool sort_flag)
    : epoch(epoch), pop_size(pop_size), sample_count(sample_count),
    intent_factor(intent_factor), zeta(zeta), sort_flag(sort_flag)
{
    pop.reserve(pop_size);
}

void OriginalACOR::generate_population()
{
    for (int i = 0; i < pop_size; ++i)
    {
        pop.push_back(generate_agent());
    }
}

Agent OriginalACOR::generate_agent()
{
    return Agent();
}

void OriginalACOR::solve(const Problem& probleme)
{
    // Your solve implementation goes here
}

void OriginalACOR::evolve(int epoch)
{
    std::vector<int> pop_rank(pop_size);
    std::iota(pop_rank.begin(), pop_rank.end(), 1);

    double qn = intent_factor * pop_size;
    std::vector<double> matrix_w(pop_size);
    for (int idx = 0; idx < pop_size; ++idx)
    {
        matrix_w[idx] = 1.0 / (std::sqrt(2.0 * M_PI) * qn) * std::exp(-0.5 * std::pow((pop_rank[idx] - 1) / qn, 2));
    }

    std::vector<double> matrix_p(pop_size);
    double sum_w = std::accumulate(matrix_w.begin(), matrix_w.end(), 0.0);
    for (int idx = 0; idx < pop_size; ++idx)
    {
        matrix_p[idx] = matrix_w[idx] / sum_w;
    }

    // Means and Standard Deviations
    std::vector<std::vector<double>> matrix_pos(pop_size, std::vector<double>(problem.n_dims));
    for (int idx = 0; idx < pop_size; ++idx)
    {
        matrix_pos[idx] = pop[idx].solution;
    }

    std::vector<std::vector<double>> matrix_sigma(pop_size, std::vector<double>(pop[0].solution.size()));
    for (int idx = 0; idx < pop_size; ++idx)
    {
        std::vector<double> temp_vec(pop_size);
        for (int jdx = 0; jdx < pop[idx].solution.size(); ++jdx)
        {
            std::vector<double> temp_vec(pop_size);
            for (int kdx = 0; kdx < pop_size; ++kdx)
            {
                temp_vec[kdx] = zeta * std::abs(matrix_pos[kdx][jdx] - matrix_pos[idx][jdx]) / (pop_size - 1);
            }
            matrix_sigma[idx][jdx] = *std::max_element(temp_vec.begin(), temp_vec.end());
        }
    }

    // Generate Samples
    std::vector<Agent> pop_new;
    for (int idx = 0; idx < sample_count; ++idx)
    {
        std::vector<double> child(problem.n_dims);
        for (int jdx = 0; jdx < problem.n_dims; ++jdx)
        {
            int rdx = get_index_roulette_wheel_selection(matrix_p);
            child[jdx] = pop[rdx].solution[jdx] + generator.normal() * matrix_sigma[rdx][jdx];
        }

        std::vector<double> pos_new = correct_solution(child);
        Agent agent = generate_empty_agent(pos_new);
        pop_new.push_back(agent);
        // You need to add a condition for the 'mode' check here
    }

    pop_new = update_target_for_population(pop_new);
    pop = get_sorted_and_trimmed_population(pop + pop_new, pop_size, problem.minmax);
}
