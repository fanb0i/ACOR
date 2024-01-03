#include "History.h"
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <numeric>
#include <limits>

History::History(std::string log_to)
    : log_to(log_to) {}

void History::store_initial_best_worst(const Agent& best_agent, const Agent& worst_agent) {
    list_global_best.push_back(best_agent);
    list_current_best.push_back(best_agent);
    list_global_worst.push_back(worst_agent);
    list_current_worst.push_back(worst_agent);
}

int History::get_global_repeated_times(double epsilon) const {
    int count = 0;
    for (size_t idx = 0; idx < list_global_best.size() - 1; ++idx) {
        double temp = std::abs(list_global_best[idx].fitness - list_global_best[idx + 1].fitness);
        if (temp <= epsilon) {
            count++;
        }
        else {
            count = 0;
        }
    }
    return count;
}

double History::calculate_diversity() const {
    if (list_population.empty() || list_population.back().empty()) {
        return 0.0;
    }

    std::vector<double> distances(list_population.back().size());
    std::vector<double> means(list_population.back()[0].solution.size());

    for (size_t i = 0; i < list_population.back()[0].solution.size(); ++i) {
        for (size_t j = 0; j < list_population.back().size(); ++j) {
            means[i] += list_population.back()[j].solution[i];
        }
        means[i] /= list_population.back().size();
    }

    for (size_t i = 0; i < list_population.back().size(); ++i) {
        double sum_square = 0.0;
        for (size_t j = 0; j < list_population.back()[i].solution.size(); ++j) {
            sum_square += std::pow(list_population.back()[i].solution[j] - means[j], 2);
        }
        distances[i] = std::sqrt(sum_square);
    }

    return std::accumulate(distances.begin(), distances.end(), 0.0) / distances.size();
}

double History::calculate_exploitation() const {
    if (list_global_best.empty() || list_global_best.back().objectives.empty()) {
        return 0.0;
    }

    double sum = 0.0;
    for (size_t i = 0; i < list_global_best.back().objectives.size(); ++i) {
        sum += list_global_best.back().objectives[i];
    }

    return sum / list_global_best.back().objectives.size();
}

double History::calculate_exploration() const {
    return 1.0 - calculate_exploitation();
}

void History::save_global_best_fitness_chart(const std::string& title, const std::string& x_label, const std::string& y_label, const std::string& filename, bool verbose) {
    Visualizer::export_convergence_chart(list_global_best_fit, title, "", "-", "b", x_label, y_label, filename, { ".png", ".pdf" }, verbose);
}

void History::save_local_best_fitness_chart(const std::string& title, const std::string& x_label, const std::string& y_label, const std::string& filename, bool verbose) {
    Visualizer::export_convergence_chart(list_current_best_fit, title, "", "-", "b", x_label, y_label, filename, { ".png", ".pdf" }, verbose);
}

void History::save_runtime_chart(const std::string& title, const std::string& x_label, const std::string& y_label, const std::string& filename, bool verbose) {
    Visualizer::export_convergence_chart(list_epoch_time, title, "", "-", "b", x_label, y_label, filename, { ".png", ".pdf" }, verbose);
}

void History::save_exploration_exploitation_chart(const std::string& title, const std::string& filename, bool verbose) {
    Visualizer::export_explore_exploit_chart({ list_exploration, list_exploitation }, title, { "Exploration", "Exploitation" }, filename, verbose);
}

void History::save_diversity_chart(const std::string& title, const std::string& filename, bool verbose) {
    Visualizer::export_diversity_chart({ list_diversity }, title, { "Algorithm" }, filename, verbose);
}

void History::save_global_objectives_chart(const std::string& title, const std::string& x_label, const std::vector<std::string>& y_labels, const std::string& filename, bool verbose) {
    std::vector<std::vector<double>> global_obj_list;
    for (size_t i = 0; i < list_global_best.back().objectives.size(); ++i) {
        std::vector<double> obj_list(list_global_best.size());
        for (size_t j = 0; j < list_global_best.size(); ++j) {
            obj_list[j] = list_global_best[j].objectives[i];
        }
        global_obj_list.push_back(obj_list);
    }
    Visualizer::export_objectives_chart(global_obj_list, title, x_label, y_labels, filename, verbose);
}

void History::save_local_objectives_chart(const std::string& title, const std::string& x_label, const std::vector<std::string>& y_labels, const std::string& filename, bool verbose) {
    std::vector<std::vector<double>> local_obj_list;
    for (size_t i = 0; i < list_current_best.back().objectives.size(); ++i) {
        std::vector<double> obj_list(list_current_best.size());
        for (size_t j = 0; j < list_current_best.size(); ++j) {
            obj_list[j] = list_current_best[j].objectives[i];
        }
        local_obj_list.push_back(obj_list);
    }
    Visualizer::export_objectives_chart(local_obj_list, title, x_label, y_labels, filename, verbose);
}