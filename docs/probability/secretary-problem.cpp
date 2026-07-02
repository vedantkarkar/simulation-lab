#include <iostream> 
#include <random> 
#include <vector>
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

    struct HiringResult {
        double best_skill;
        double hired_skill;
        int hired_index;
        int hired_rank;
        bool hired_best;
    };

    struct Printer {
        int rejections; 
        double final_avg_skill;
        double final_prob;
        double final_avg_rank;
    };

    double best_skilled(vector<double>& candidate) {
        double local_max = candidate.at(0);
        for(int i = 1; i < candidate.size(); i++) {
            if(candidate.at(i) > local_max){
                local_max = candidate.at(i);
            }
        }
        return local_max;
    }

    int hired_candidate_index(vector<double>& candidate, int rejection) {

        if(rejection == 0) { 
            return 0; 
        }

        double rejected_max_skill = candidate.at(0);

        for(int i = 1; i < rejection; i++) {
            if(candidate.at(i) > rejected_max_skill) {
                rejected_max_skill = candidate.at(i);
            }
        }

        for(int i = rejection; i < (total-1); i++) { 
            if(candidate.at(i) > rejected_max_skill) {
                return i;
            }
        }
        
        return (total - 1);
    }

    int hired_rank(const vector<double>& candidate, double hired_man) {
        int rank = 1; 
        for(int i = 0; i < candidate.size(); i++) {
            if(candidate.at(i) > hired_man) {
                rank++;
            }
        }
        return rank;
    }

    // rejection can be from 0 meaning take first candidate, to 99 meaning take the last candidate
    HiringResult strategy_for(int rejection) {
        vector<double>candidate;
        candidate.reserve(total);

        for(int i = 0; i < total; i++) {
            candidate.push_back(generate_skill());
        }

        HiringResult hired_emp;

        hired_emp.best_skill = best_skilled(candidate);
        hired_emp.hired_index = hired_candidate_index(candidate, rejection);
        hired_emp.hired_skill = candidate.at(hired_emp.hired_index);
        hired_emp.hired_rank = hired_rank(candidate, hired_emp.hired_skill); 
        hired_emp.hired_best = (hired_emp.hired_rank == 1);

        return hired_emp;
    }

    vector<HiringResult> generate_data_for(int rejection, int trials) {
        vector<HiringResult> data; 
        data.reserve(trials);
        for(int i = 0; i < trials; i++) {
            data.push_back(strategy_for(rejection));
        }
        return data;
    }

    double average_skill(const vector<HiringResult>& data) {
        if(data.empty()) {
            return 0.0;
        }
        double sum = 0.0;
        for(int i = 0; i < data.size(); i++) {
            sum += (data.at(i)).hired_skill;
        }
        return sum/data.size();
    }

    double prob_hired_best(const vector<HiringResult>& data){
        if(data.empty()) {
            return 0.0;
        }
        double success = 0;
        for(int i = 0; i < data.size(); i++) {
            if((data.at(i)).hired_best) {
                success++;
            }
        }
        return success/data.size();
    }

    double average_rank(const vector<HiringResult>& data) {
        if(data.empty()) {
            return -1;
        }
        double sum = 0.0;
        for(int i = 0; i < data.size(); i++) {
            sum += (data.at(i)).hired_rank;
        }
        return sum/data.size();
    }

    vector<vector<HiringResult>> perform_experiment(int trials) {
        cout << "Performing experiment for " << trials << " trials\n";
        cout << "Please don't press anything ... \n";
        total_trials = trials;
        vector<vector<HiringResult>>result;
        result.reserve(total);
        for(int i = 0; i < total; i++) {
            result.push_back(generate_data_for(i, trials));
        }
        return result;
    }

    vector<Printer> calculate_results(vector<vector<HiringResult>>& result) {
        vector<Printer> pages(total); 

        for(int i = 0; i < total; i++) {
            (pages.at(i)).rejections = i;
            (pages.at(i)).final_avg_skill = average_skill(result.at(i));
            (pages.at(i)).final_prob = prob_hired_best(result.at(i)); 
            (pages.at(i)).final_avg_rank = average_rank(result.at(i));
        }
        return pages;
    }

    int max_prob_index(vector<Printer>& pages) {
        double local_max = (pages.at(0)).final_prob;
        int index = (pages.at(0)).rejections;
        for(int i = 1; i < pages.size(); i++){
            if((pages.at(i)).final_prob > local_max) {
                local_max = (pages.at(i)).final_prob;
                index = (pages.at(i)).rejections;
            }
        }
        return index;
    }

    int max_avg_skill_index(vector<Printer>& pages) {
        double local_max = (pages.at(0)).final_avg_skill;
        int index = (pages.at(0)).rejections;
        for(int i = 1; i < pages.size(); i++){
            if((pages.at(i)).final_avg_skill > local_max) {
                local_max = (pages.at(i)).final_avg_skill;
                index = (pages.at(i)).rejections;
            }
        }
        return index;
    }

    void draw_histogram(vector<Printer>& pages, int choice) {
        const int bar_width = 100; // Width of the histogram bars
        cout << "\nHistogram of results:\n";
        if(choice == 0) {
            for(int i = 0; i < pages.size(); i++) {
                cout << std::setw(3) << (pages.at(i)).rejections << " | " << std::string((pages.at(i)).final_prob * bar_width, '*') << " (" << (pages.at(i)).final_prob * bar_width << " \%) [Average rank = " << (pages.at(i)).final_avg_rank << "/100 ]\n";
            }
        }
        else {
            for(int i = 0; i < pages.size(); i++) {
                cout << std::setw(3) << (pages.at(i)).rejections << " | " << std::string((pages.at(i)).final_avg_skill * bar_width, '*') << " (" << (pages.at(i)).final_avg_skill * bar_width << " \%) \n";
            }
        }
    }

    void ask_for_histogram(vector<Printer>& pages, int c = 0) {
        cout << "-------------------------------\n";
        cout << "Would you like to see a histogram of the results? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            draw_histogram(pages, c);
        }
    }


    void print_results(vector<vector<HiringResult>>& result) {
        vector<Printer> pages = calculate_results(result);
        int index1 = max_prob_index(pages);
        cout << "-------------------------------\n";
        cout << "Results for " << total_trials << " trials for each strategy:\n\n";
        cout << "-------------------------------\n";
        cout << "Requirement: Hire the Best candidate from the " << total << " total candidates\n";
        cout << "Solution: If best candidate hired, the experiment was successful\n\n";
        cout << "Number of rejections that results in the highest probability of hiring the best candidate is " << index1 << endl;
        cout << "Probability of successful drive being: " << (pages.at(index1)).final_prob * 100 << "\%\n";
        cout << "Average rank hired when most successful strategy applied: " << (pages.at(index1)).final_avg_rank << " out of 100\n";
        cout << "-------------------------------\n\n";
        ask_for_histogram(pages, 0); // for strategy 1
        int index2 = max_avg_skill_index(pages);
        cout << "\nRequirement: Overall average hiring should result in best average skill percentile of hired candidates\n";
        cout << "Solution: Average skill level of all " << total << " candidates hired through a strategy is considered\n\n";
        cout << "Number of rejections that resulted in the highest average skill of candidate: " << index2 << " rejections\n";
        cout << "Average skill level of hired candidate acheived: " << (pages.at(index2)).final_avg_skill * 100 << "\%\n";
        ask_for_histogram(pages, 1); // for strategy 2
        cout << "-------------------------------\n";
        cout << "Simulation complete.\n";
        cout << "-------------------------------\n";
    }

};

int main() {
    constexpr int trials = 50000; 
    Secretary hr;
    auto results = hr.perform_experiment(trials);

    hr.print_results(results);

    return 0; 
}