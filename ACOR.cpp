#include "ACOR.h"

OriginalACOR::OriginalACOR(int epoch, int pop_size, int sample_count, double intent_factor, double zeta, bool sort_flag)
    : epoch(epoch), pop_size(pop_size), sample_count(sample_count),
    intent_factor(intent_factor), zeta(zeta), sort_flag(sort_flag)
{
    pop.reserve(pop_size);
}

double OriginalACOR::calculate_fitness(std::vector<double>& solution)
{
    d_p->calculate_fitness(solution);
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
    Agent a = generate_empty_agent();
    std::vector<double> solution = a.getsolution();
    a.set_fitness(calculate_fitness(solution));
    return a;
}

Agent OriginalACOR::generate_empty_agent()
{
    std::vector<double> solution = d_p->generate_solution();
    return Agent(solution);
}

Agent OriginalACOR::generate_empty_agent(std::vector<double>& solution)
{
    return Agent(solution);
}

void OriginalACOR::after_initialization()
{
    // The initial population is sorted or not depending on the algorithm's strategy
    std::vector<Agent> pop_temp;
    std::vector<Agent> best;
    std::vector<Agent> worst;
    get_special_agents(pop, 1, 1, d_p->minmax(), pop_temp,best,worst);
   
    if (!best.empty() && !worst.empty()) {
        g_best = best[0];
        g_worst = worst[0];
    }

    if (sort_flag) {
        pop = pop_temp;
    }

    // Store initial best and worst solutions
    history.store_initial_best_worst(g_best, g_worst);
}


std::vector<Agent> OriginalACOR::get_sorted_population(const std::vector<Agent>& pop, const std::string& minmax)
{
    if (minmax == "min")
    {
        std::sort(pop.begin(), pop.end(), [](const Agent& a, const Agent& b) {
            return a.getfitness() < b.getfitness();
            });
    }
    else
    {
        std::sort(pop.begin(), pop.end(), [](const Agent& a, const Agent& b) {
            return a.getfitness() > b.getfitness();
            });
    }
    return pop;
}

void OriginalACOR::get_special_agents(
    const std::vector<Agent>& pop, int n_best, int n_worst,
    const std::string& minmax, std::vector<Agent>& sorted_pop,
    std::vector<Agent>& best_agents, std::vector<Agent>& worst_agents)
{
    // Sort the population based on the problem type (min or max)
 
    
    sorted_pop = get_sorted_population(pop,minmax);

    // Get the best agents
    if (n_best > 0) {
        best_agents.assign(sorted_pop.begin(), sorted_pop.begin() + std::min(n_best, static_cast<int>(sorted_pop.size())));
    }

    // Get the worst agents
    if (n_worst > 0) {
        worst_agents.assign(sorted_pop.end() - std::min(n_worst, static_cast<int>(sorted_pop.size())), sorted_pop.end());
    }
}


void OriginalACOR::solve(Problem* probleme)
{
    d_p = probleme;
    generate_population();
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
    std::vector<std::vector<double>> matrix_pos(pop_size, std::vector<double>(d_p->n_dims()));
    for (int idx = 0; idx < pop_size; ++idx)
    {
        matrix_pos[idx] = pop[idx].getsolution();
    }

    std::vector<std::vector<double>> matrix_sigma(pop_size, std::vector<double>(pop[0].getsolution().size()));
    for (int idx = 0; idx < pop_size; ++idx)
    {
        std::vector<double> temp_vec(pop_size);
        for (int jdx = 0; jdx < pop[idx].getsolution().size(); ++jdx)
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
        std::vector<double> child(d_p->n_dims());
        for (int jdx = 0; jdx < d_p->n_dims(); ++jdx)
        {
            int rdx = get_index_roulette_wheel_selection(matrix_p);
            child[jdx] = pop[rdx].getsolution()[jdx] + generator.normal() * matrix_sigma[rdx][jdx];
        }

        std::vector<double> pos_new = correct_solution(child);
        Agent agent = generate_empty_agent(pos_new);
        pop_new.push_back(agent);
        // You need to add a condition for the 'mode' check here
        pop_new.back().set_fitness(calculate_fitness(pos_new));

    }

    pop_new = update_target_for_population(pop_new);
    pop = get_sorted_and_trimmed_population(pop + pop_new, pop_size, d_p->minmax());
}
