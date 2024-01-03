#ifndef HISTORY_H
#define HISTORY_H

#include <vector>
#include <string>
#include <cmath>
#include "Optimizer.h"
#include "Visualizer.h" 

class History {
public:
    History(std::string log_to = "");

    // Public member variables
    std::vector<Agent> list_global_best;
    std::vector<Agent> list_current_best;
    std::vector<double> list_epoch_time;
    std::vector<double> list_global_best_fit;
    std::vector<double> list_current_best_fit;
    std::vector<std::vector<Agent>> list_population;
    std::vector<double> list_diversity;
    std::vector<double> list_exploitation;
    std::vector<double> list_exploration;
    std::vector<Agent> list_global_worst;
    std::vector<Agent> list_current_worst;

    int epoch;
    std::string log_to;
    std::string log_file;

    void store_initial_best_worst(const Agent& best_agent, const Agent& worst_agent);
    int get_global_repeated_times(double epsilon) const;
    double calculate_diversity() const;
    double calculate_exploitation() const;
    double calculate_exploration() const;
    void save_global_best_fitness_chart(const std::string& title, const std::string& x_label, const std::string& y_label, const std::string& filename, bool verbose = true);
    void save_local_best_fitness_chart(const std::string& title, const std::string& x_label, const std::string& y_label, const std::string& filename, bool verbose = true);
    void save_runtime_chart(const std::string& title, const std::string& x_label, const std::string& y_label, const std::string& filename, bool verbose = true);
    void save_exploration_exploitation_chart(const std::string& title, const std::string& filename, bool verbose = true);
    void save_diversity_chart(const std::string& title, const std::string& filename, bool verbose = true);
    void save_global_objectives_chart(const std::string& title, const std::string& x_label, const std::vector<std::string>& y_labels, const std::string& filename, bool verbose = true);
    void save_local_objectives_chart(const std::string& title, const std::string& x_label, const std::vector<std::string>& y_labels, const std::string& filename, bool verbose = true);

private:


#endif // HISTORY_H