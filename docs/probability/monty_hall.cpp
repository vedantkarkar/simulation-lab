#include <iostream>
#include <iomanip>
#include <random>
#include <exception>
#include <cmath>

class MontyHallSolution {
    public: 
    std::mt19937 rng;
    std::uniform_int_distribution<int> door;
    enum class Strategy {
        STAY,
        SWITCH
    };

    MontyHallSolution() : door(1, 3), rng(std::random_device{}()) {}

    // We do not explicitly simulate the host opening a door.
    // Once the contestant makes the initial choice,
    // only two cases exist:
    //
    // chosen == prize
    //      Staying wins.
    //      Switching loses.
    //
    // chosen != prize
    //      Staying loses.
    //      Switching wins.
    // That is the core of Monty Hall problem.
    // The host's action of revealing a goat does not change the fact that if the contestant initially chose the prize, switching will lose, and if they initially chose a goat, switching will win.
    // Therefore the host's action does not affect the outcome of the simulation and can be omitted.

    bool experiment(Strategy strat) {
        int prize = door(rng);
        int chosen = door(rng);
        
        // Strategy 1 for staying wherver you choose
        // Strategy 2 for switching with the remaining door after one goat is revealed
        if (strat == Strategy::STAY) {
            return (prize == chosen);
        }
        else if (strat == Strategy::SWITCH) { 
            return !(prize == chosen);
        }

        return false; // This line should never be reached, but is included to avoid compiler warnings.
    }

    int perform_for(int trials, Strategy strategy) {
        int success = 0;
        for(int i = 0; i < trials; i++) {
            success += experiment(strategy);
        }
        return success;
    }

    void run(int trials) {
        try {
            if (trials <= 0) {
                throw std::invalid_argument("Number of trials must be a positive integer.");
            }
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return;
        }
        int strat1 = perform_for(trials, Strategy::STAY);
        int strat2 = perform_for(trials, Strategy::SWITCH);

        std::cout << std::setfill('-') << std::right << std::endl;
        std::cout << std::setw(80) << "" << std::endl;
        std::cout << "Monty Hall Problem Simulation Results" << std::endl;
        std::cout << "Total number of trials: " << trials << std::endl;
        std::cout << std::setw(80) << "" << std::endl;
        std::cout << "Strategy 1: " << "Staying with the same door\n" << strat1 << " wins, " << (trials - strat1) << " losses, "
                  << std::fixed << std::setprecision(3) << (static_cast<double>(strat1) / trials) * 100 << "% winning probability" << std::endl;
        std::cout << std::setw(80) << "" << std::endl;
        std::cout << "Strategy 2 (Switch): " << "Switching to the other door\n" << strat2 << " wins, " << (trials - strat2) << " losses, "
                  << std::fixed << std::setprecision(3) << (static_cast<double>(strat2) / trials) * 100 << "% winning probability" << std::endl;
        std::cout << std::setw(80) << "" << std::endl;
        std::cout << "Theoretical Probability: Strategy 1 (Stay): 33.33%, Strategy 2 (Switch): 66.67%" << std::endl;
        std::cout << "Absolute Error: Strategy 1 (Stay): " << std::fixed << std::setprecision(3) << std::abs((static_cast<double>(strat1) / trials) - 0.3333) * 100 << "%, "
                  << "Strategy 2 (Switch): " << std::fixed << std::setprecision(3) << std::abs((static_cast<double>(strat2) / trials) - 0.6667) * 100 << "%" << std::endl;
        std::cout << "Conclusion: Switching doors yields a higher probability of winning than staying with the original choice." << std::endl;
        std::cout << std::setw(80) << "" << std::endl;
    }
};

int main() {

    // Change your number of trials here to see how the results converge with more trials
    constexpr int trials = 10'000'000;

    MontyHallSolution simulation;

    simulation.run(trials);

    return 0;
}