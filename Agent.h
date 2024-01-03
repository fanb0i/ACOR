#pragma once
#include "Solution.h"
class Agent {
private:
    static int ID;

public:
    std::vector<double> solution;
    Target target;
    int id;
    std::map<std::string, std::string> kwargs;  // Assuming this is used to store additional attributes

    Agent(std::vector<double> solution = {}, Target target = {}, std::map<std::string, std::string> kwargs = {})
        : solution(solution), target(target), kwargs(kwargs) {
        id = increase();
    }

    static int increase() {
        return ++ID;
    }

    void set_kwargs(std::map<std::string, std::string>& kwargs) {
        for (auto& kv : kwargs) {
            this->kwargs[kv.first] = kv.second;
        }
    }

    Agent copy() const {
        Agent agent(solution, target.copy(), kwargs);
        for (auto& kv : *this) {
            if (kv.first != "target" && kv.first != "solution" && kv.first != "id" && kv.first != "kwargs") {
                agent[kv.first] = kv.second;
            }
        }
        return agent;
    }

    void update_agent(const std::vector<double>& solution, const Target& target) {
        this->solution = solution;
        this->target = target;
    }

    void update(std::map<std::string, std::string>& kwargs) {
        for (auto& kv : kwargs) {
            (*this)[kv.first] = kv.second;
        }
    }

    Agent get_better_solution(const Agent& compared_agent, const std::string& minmax = "min") const {
        if (minmax == "min") {
            return (this->target.fitness < compared_agent.target.fitness) ? *this : compared_agent;
        }
        else {
            return (this->target.fitness < compared_agent.target.fitness) ? compared_agent : *this;
        }
    }

    bool is_duplicate(const Agent& compared_agent) const {
        return (this->solution == compared_agent.solution);
    }

    bool compare_duplicate(const Agent& compared_agent) {
        if (this->solution == compared_agent.solution) {
            this->target = compared_agent.target;
            return true;
        }
        return false;
    }

    bool is_better_than(const Agent& compared_agent, const std::string& minmax = "min") const {
        if (minmax == "min") {
            return (this->target.fitness < compared_agent.target.fitness);
        }
        else {
            return (this->target.fitness < compared_agent.target.fitness);
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Agent& agent) {
        os << "id: " << agent.id << ", target: " << agent.target << ", solution: [";
        for (const auto& value : agent.solution) {
            os << value << ", ";
        }
        os << "]";
        return os;
    }
};

int Agent::ID = 0;
}