#include "ACOR.h"


OriginalACOR::OriginalACOR(int epoch, int pop_size, int sample_count, double intent_factor, double zeta, bool sort_flag)
    : epoch(epoch), pop_size(pop_size), sample_count(sample_count),
    intent_factor(intent_factor), zeta(zeta), sort_flag(sort_flag)
{
    pop.reserve(pop_size);
}

double OriginalACOR::calculate_fitness(std::vector<double>& solution)
{
    return d_p->calculate_fitness(solution);
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

int OriginalACOR::get_index_roulette_wheel_selection(const std::vector<double>& list_fitness)
{
    std::mt19937 generator;
    if (list_fitness.empty()) {
        // Handle empty list
        return -1; // or any other appropriate value
    }

    std::vector<double> final_fitness = list_fitness;
    if (*std::min_element(list_fitness.begin(), list_fitness.end()) < 0) {
        // Shift to handle negative fitness values
        std::transform(final_fitness.begin(), final_fitness.end(), final_fitness.begin(),
            [min_val = *std::min_element(list_fitness.begin(), list_fitness.end())](double val) {
                return val - min_val;
            });
    }

    double sum_fitness = std::accumulate(final_fitness.begin(), final_fitness.end(), 0.0);

    if (sum_fitness == 0) {
        // Handle the case where all fitness values are the same
        return static_cast<int>(generator() % list_fitness.size());
    }

    // Normalize fitness values
    std::for_each(final_fitness.begin(), final_fitness.end(),
        [&sum_fitness](double& val) { val /= sum_fitness; });

    std::discrete_distribution<int> distribution(final_fitness.begin(), final_fitness.end());

    // Capture variables inside the lambda
    return [this, &distribution, &generator]() {
        return distribution(generator);
        }();
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
}


std::vector<Agent> OriginalACOR::get_sorted_population(std::vector<Agent>& pop, const std::string& minmax)
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
    std::vector<Agent>& pop, int n_best, int n_worst,
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


Agent OriginalACOR::solve(Problem* probleme)
{
    d_p = probleme;
    generate_population();
    for (int i = 0; i < epoch; i++)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        evolve(i);
        pop = get_sorted_population(pop,d_p->minmax());
        g_best = pop[0];



        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        track_optimize_step(pop, i, elapsed_time);

    }
    return g_best;
}

std::vector<Agent> OriginalACOR::get_sorted_and_trimmed_population(std::vector<Agent>& population)
{
    std::vector<Agent> newpop = get_sorted_population(population, d_p->minmax());
    newpop.resize(pop_size);
    return newpop;
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
    std::random_device rd;
    std::mt19937 generator(rd());
    for (int idx = 0; idx < sample_count; ++idx)
    {
        std::vector<double> child(d_p->n_dims());
        for (int jdx = 0; jdx < d_p->n_dims(); ++jdx)
        {
            int rdx = get_index_roulette_wheel_selection(matrix_p);
            double mean = 0.0;
            double stddev = 1.0;

            // Create a normal distribution
            std::normal_distribution<double> normalDistribution(mean, stddev);

            // Generate a random number from the normal distribution
            double randomValue = normalDistribution(generator);
            child[jdx] = pop[rdx].getsolution()[jdx] + randomValue * matrix_sigma[rdx][jdx];
        }

        std::vector<double> pos_new = d_p->correct_solution(child);
        Agent agent = generate_empty_agent(pos_new);
        pop_new.push_back(agent);
        // You need to add a condition for the 'mode' check here
        pop_new.back().set_fitness(calculate_fitness(pos_new));

    }
    std::vector<Agent> finalpop;
    finalpop.reserve(pop.size() + pop_new.size());

    // Insert elements from the first vector
    finalpop.insert(finalpop.end(), pop.begin(), pop.end());

    // Insert elements from the second vector
    finalpop.insert(finalpop.end(), pop_new.begin(), pop_new.end());
    pop = get_sorted_and_trimmed_population(finalpop);
}

void OriginalACOR::track_optimize_step(std::vector<Agent>& population, int epoch, long time)
{
    std::cout << "Epoch : " << epoch << " , Current Best : " << population[0].getfitness() << " Runtime : " << time <<std::endl;
}
