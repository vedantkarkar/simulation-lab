#include <iostream>
#include <iomanip>
#include <random>
#include <map>
#include <cmath>

class GaltonBoard {
    public: 
    double bias;
    int rows;
    std::mt19937 rng;
    std::bernoulli_distribution turn;

    // Set up the Galton Board with a specified number of rows and bias for rightward movement.
    // p is probability to go right
    GaltonBoard(int row = 30, double p = 0.5) : bias(p), rows(row), rng(std::random_device{}()), turn(bias) {}

    void set_bias(double p) {
        bias = p;
        turn.param(std::bernoulli_distribution::param_type(p));
    }

    void set_rows(int r) {
        rows = r;
    }
    
    double get_bias(){
        return bias;
    }

    int get_rows(){
        return rows;
    }

    std::map <int, int> pits;

    int drop_ball() {
        int x  = 0;
        // if turn == true, ball goes right, otherwise it goes left
        for(int i = 0; i < rows; i++) {
            if(turn(rng)){
                x++;
            }
            else {
                x--;
            }
        }
        return x;
    }

    std::map <int, int> generate_data(int trials) {
        std::cout << "Dropping " << trials << " balls through a Galton Board with " << rows << " rows and bias of " << bias << "...\n";
        std::cout << "Please wait while the simulation runs...\n";
        std::map <int, int> data;
        for(int i = 0; i < trials; i++) {
            data[drop_ball()]++;
        }
        return data;
    }

    double compute_mean(int trials, const std::map <int, int>& data) {
        double sum = 0;
        for(const auto& pair : data) {
            sum += pair.first * pair.second;
        }
        return sum / trials;
    }

    double compute_stddev(int trials, double mean, const std::map <int, int>& data) {
        double sum_sq = 0;
        for(const auto& pair : data) {
            sum_sq += (pair.first - mean) * (pair.first - mean) * pair.second;
        }
        return sqrt(sum_sq / (trials - 1));
    }

    void draw_histogram(const std::map <int, int>& data) {
        constexpr int bar_width = 100; // Width of the histogram bars
        int max_count = 0;
        // getting max_count to scale the histogram bars
        for(const auto& pair : data) {
            if(pair.second > max_count) {
                max_count = pair.second;
            }
        }
        std::cout << "Histogram of ball distribution:" << std::endl;
        for (const auto& pair : data) {
            std::cout << std::setfill(' ') << std::setw(10) << " " << std::setfill('-') << std::setw(bar_width + 10) << "" << std::endl;
            std::cout << "bin :" << std::setfill(' ') << std::setw(3) << pair.first << " | " << std::string(pair.second * bar_width / max_count, '*') << " (" << pair.second << ")\n";
        }
        std::cout << std::setfill('-') << std::setw(bar_width) << "" << std::endl;
    }

    void askfor_histogram(const std::map<int, int>& data) {
        std::cout << "Would you like to see a histogram of the results? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            draw_histogram(data);
        }
    }

    void run_for(int trials) {
        std::map <int, int> data = generate_data(trials);
        double mean = compute_mean(trials, data);
        double stddev = compute_stddev(trials, mean, data);

        std::cout << std::setfill('-') << std::setw(50) << "" << std::endl;
        std::cout << "Galton Board Simulation Results" << std::endl;
        std::cout << "Total number of rows: " << rows << std::endl;
        std::cout << "Total number of balls dropped: " << trials << "\n\n";

        std::cout << "Obsereved Mean landing position: " << mean << std::endl;
        std::cout << "Expected mean landing position: 0" << std::endl;
        std::cout << "Absolute Error: " << std::fixed << std::setprecision(3) << std::abs(mean - 0) << "\n\n";

        std::cout << "Obsereved Standard deviation: " << stddev << std::endl;
        std::cout << "Expected standard deviation: " << sqrt(rows) << std::endl;
        std::cout << "Absolute Error: " << std::fixed << std::setprecision(3) << std::abs(stddev - sqrt(rows)) << std::endl;

        std::cout << std::setfill('-') << std::setw(50) << "" << std::endl;
        askfor_histogram(data);
        std::cout << std::setfill('=') << std::setw(50) << "" << std::endl;
        std::cout << "Simulation complete." << std::endl;
        std::cout << std::setfill('=') << std::setw(50) << "" << std::endl;
    }
};

int main() {
    constexpr int trials = 500'000; // Number of trials to run

    // Initalise Galton Board with rows and bias (probability of going right)
    GaltonBoard board(100, 0.5); 
    board.run_for(trials);
    return 0;
}