#include <iostream>
#include <random>
#include <vector>
#include <unordered_set>
#include <algorithm> // for std::sort
#include <map>
#include <iomanip>

class BirthdaySimulator {
    public: 
    std::mt19937 rng;
    std::uniform_int_distribution<int> bday_dist;
    std::unordered_set<int> birthdays;

    BirthdaySimulator() : rng(std::random_device{}()), bday_dist(1, 365) {}

    int people_in_room() {
        int people = 0;
        int bday;
        while(true) {
        bday = bday_dist(rng);
        ++people;
        if(birthdays.count(bday) > 0) {
            birthdays.clear(); 
            return people;
        }
        birthdays.insert(bday);
        }
    }

    std::vector<int> store_data_for(int trials) {
        std::vector<int> results;
        results.reserve(trials);
        for (int i = 0; i < trials; ++i) {
            results.push_back(people_in_room());
        }
        return results;
    }

    double mean_vector(const std::vector<int>& data) {
        double sum = 0.0;
        for (int value : data) {
            sum += value;
        }
        return sum / data.size();
    }

    double std_dev_vector(const std::vector<int>& data, double mean) {
        double sum_squared_diff = 0.0;
        for (int value : data) {
            sum_squared_diff += (value - mean) * (value - mean);
        }
        return std::sqrt((sum_squared_diff) / (data.size() - 1));
    }

    int min_data(const std::vector<int>& data) {
        int min = data[0];
        for (int i = 1; i < data.size(); ++i) {
            if (data[i] < min) {
                min = data[i];
            }
        }
        return min;
    }

    int max_data(const std::vector<int>& data) {
        int max = data[0];
        for (int i = 1; i < data.size(); ++i) {
            if (data[i] > max) {
                max = data[i];
            }
        }
        return max;
    }

    double median_data(const std::vector<int>& data) {
        std::vector<int> sorted_data = data;
        std::sort(sorted_data.begin(), sorted_data.end());
        size_t size = sorted_data.size();
        if (size % 2 == 0) {
            return (sorted_data[size / 2 - 1] + sorted_data[size / 2]) / 2.0;
        } else {
            return sorted_data[size / 2];
        }
    }

    void draw_histogram(const std::vector<int>& data) {
        const int bar_width = 100; // Width of the histogram bars
        std::map<int, int> histogram;
        for (int value : data) {
            histogram[value]++;
        }
        int max_value = 0;
        for (const auto& pair : histogram) {
            if (pair.second > max_value) {
                max_value = pair.second;
            }
        }
        std::cout << "Histogram of results:\n";
        for (const auto& pair : histogram) {
            std::cout << std::setw(3) << pair.first << " | " << std::string(pair.second * bar_width / max_value, '*') << " (" << pair.second << ")\n";
        }
    }

    void askfor_histogram(const std::vector<int>& data) {
        std::cout << "Would you like to see a histogram of the results? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            draw_histogram(data);
        }
    }

    void print_results(const std::vector<int>& data) {
        double mean = mean_vector(data);
        double std_dev = std_dev_vector(data, mean);
        std::cout << "-------------------------------\n";
        std::cout << "Results for " << data.size() << " trials:\n";
        std::cout << "-------------------------------\n";
        std::cout << "Minimum: " << min_data(data) << "\n";
        std::cout << "Maximum: " << max_data(data) << "\n";
        std::cout << "Median: " << median_data(data) << "\n";
        std::cout << "Mean: " << mean << "\n";
        std::cout << "Standard Deviation: " << std_dev << "\n";
        std::cout << "-------------------------------\n";
        std::cout << "50\% collision threshold approximation: " << 1.0 + std::sqrt(2.0 * 365.0 * std::log(2.0)) << "\n";
        std:: cout << "Expected theoretical mean: " << std::sqrt(M_PI * 365.0 / 2.0) + (2.0 / 3.0) << "\n";
        std::cout << "-------------------------------\n";
        askfor_histogram(data);
        std::cout << "-------------------------------\n";
        std::cout << "Simulation complete.\n";
        std::cout << "-------------------------------\n";
    }
};

int main() {
    BirthdaySimulator sim;
    constexpr int trials = 100000; // Number of trials to run
    auto data = sim.store_data_for(trials);
    sim.print_results(data);

    return 0; 
}