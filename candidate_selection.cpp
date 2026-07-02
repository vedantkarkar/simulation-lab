// 1. 
// in strategy_for() : 
// vector<double> candidates(100);

// 2. 
// Instead of double strategy_for(...)
// introduce
// struct Candidate
// {
//     double skill;
// };
// Later:
// struct HiringResult
// {
//     double hired_skill;
//     double best_skill;
//     bool hired_best;
//     int hired_rank;
//     int hired_index;
// };

#include <iostream> 
#include <random> 
#include <vector>
#include <numeric> // for std::accumulate()
#include <map>
#include <iomanip>

using namespace std; 

class Secretary {
    public: 
    mt19937 rng; 
    uniform_real_distribution<double> skill_dist; 

    Secretary(int t = 0): rng(random_device{}()), skill_dist(0.0, 1.0), total_trials(t) {}

    int total = 100;
    int total_trials;

    double generate_skill(){
        return skill_dist(rng); 
    }

    // rejection can be from 0 meaning take first candidate, to 99 meaning take the last candidate
    double strategy_for(int rejection) {
        double max_skill = 0.0;

        if(rejection != 0) {
            max_skill = generate_skill(); 
        }

        for(int i = 1; i < rejection; i++) {
            double candidate = generate_skill();
            if(candidate > max_skill) {
                max_skill = candidate;
            }
        }

        for(int i = rejection; i < (total-1); i++) {
            double candidate = generate_skill(); 
            if(candidate > max_skill) {
                return candidate;
            }
        }
        
        return generate_skill();
    }

    vector<double> generate_data_for(int rejection, int trials) {
        vector<double> data; 
        data.reserve(trials);
        for(int i = 0; i < trials; i++) {
            data.push_back(strategy_for(rejection));
        }
        return data;
    }

    double average_skill(const vector<double>& data) {
        if(data.empty()) {
            return 0.0;
        }
        double sum = (accumulate(data.begin(), data.end(), 0.0));
        return sum/data.size();
    }

    map<int, double> perform_experiment(int trials) {
        cout << "Performing experiment for " << trials << " trials\n";
        cout << "Please don't press anything ... \n";
        total_trials = trials;
        map<int, double> results;
        for(int i = 0; i < 100; i++) {
            results[i] = average_skill(generate_data_for(i, trials));
        }
        return results;
    }

    int best_strategy(const map<int, double>& results) {
        auto it = results.begin();
        double max_value = it->second;
        int corresponding_key = it->first;
        it++;
        for(; it != results.end(); ++it){
            if(it->second > max_value) {
                max_value = it->second;
                corresponding_key = it->first;
            }
        }
        return corresponding_key;
    }

    void print_results(const map<int, double>& results) {
        int successful_key = best_strategy(results);
        double max_skill = results.at(successful_key);
        cout << "-------------------------------\n";
        cout << "Results for " << total_trials << " trials for each strategy: \n\n";
        cout << "Most successful strategy was for " << successful_key << " rejections\n";
        cout << "Average Skill level of most successful hiring strategy is " << max_skill * 100 << "\%\n"; 
        ask_for_histogram(results);
        cout << "-------------------------------\n";
        cout << "Simulation complete.\n";
        cout << "-------------------------------\n";
    }

    void draw_histogram(const map<int, double>& results) {
        const int bar_width = 100; // Width of the histogram bars
        cout << "Histogram of results:\n";
        for (const auto& pair : results) {
            std::cout << std::setw(3) << pair.first << " | " << std::string(pair.second * bar_width, '*') << " (" << pair.second * bar_width << " \%\n";
        }
    }

    void ask_for_histogram(const map<int, double>& results) {
        cout << "-------------------------------\n";
        cout << "Would you like to see a histogram of the results? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            draw_histogram(results);
        }
    }

};

int main() {
    constexpr int trials = 10000; 
    Secretary hr;
    auto results = hr.perform_experiment(trials);

    hr.print_results(results);

    return 0; 
}