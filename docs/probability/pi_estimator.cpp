#include <iostream>
#include <random>
#include <cmath>


using namespace std;

class WhatisPI {
    public:
    mt19937 rng;
    uniform_real_distribution<double> dist;
    // constexpr double radius = 1.0; 

    int trials;
    int success;

    WhatisPI(): rng(random_device{}()), dist(-1.0, 1.0) {}
    
    double genX() {
        return dist(rng);
    }

    double genY() {
        return dist(rng);
    }
    
    bool experiment() {
        double x = genX();
        double y = genY(); 
        double distance = sqrt((x*x) + (y*y));
        if(distance < 1) {
            return true;
        }
        else {
            return false;
        }
    }

    int do_for(int trials) {
        int inside = 0;
        for(int i = 0; i < trials; i++) {
            if (experiment()) {
                inside++;
            }
        }
        return inside;
    }

    void print_result(int success, int trials) {
        double pi = (((double)success * 4.0) / (double)trials) ;
        cout << "-----------------------------------------------\n"; 
        cout << "Experiment for trials: " << trials << endl;
        cout << "Hits inside circle: " << success << endl;
        cout << "Estimated Value of pi = " << pi << endl;
        cout << "-----------------------------------------------\n"; 
        cout << "Actual Value of pi = " << M_PI << endl;
        cout << "-----------------------------------------------\n"; 
    }


};

int main() {
    WhatisPI obj; 
    constexpr int trials = 10000000;
    int success = obj.do_for(trials);
    obj.print_result(success, trials);

    return 0;
}