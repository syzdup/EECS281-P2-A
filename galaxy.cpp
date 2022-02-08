// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
#include <iostream>
#include "Options.h"
#include "Planet.h"

std::vector<Planet> planets;
int num_generals;
int current_time = 0;

// Check for errors
void check_errors(const int general_num, const int planet_num, const int num_troops, const int force_sensitivity, const int timestamp) {
    if(general_num < 0 || general_num >= num_generals) {
        std::cerr << "Invalid general number (out of range)\n";
        exit(1);
    }
    if(planet_num < 0 || planet_num >= int(planets.size())) {
        std::cerr << "Invalid planet number (out of range)\n";
        exit(1);
    }
    if(num_troops <= 0 || force_sensitivity <= 0) {
        std::cerr << "Invalid number of troops\n";
        exit(1);
    }
    if(timestamp < current_time) {
        std::cerr << "invalid time stamp\n";
        exit(1);
    }
}

// Reads input from a deployment list file
void read_DL() {
    std::string line;
    getline(std::cin, line);
    // How to get correct general number? errors............
    std::cout << line;
    std::cout << num_generals;
    getline(std::cin, line);
    planets.resize((unsigned long)line.back());

    int timestamp;
    std::string side;
    std::string general_num;
    std::string planet_num;
    std::string force_sensitivity;
    std::string num_troops;

    while(std::cin >> timestamp >> side >> general_num >> planet_num >> force_sensitivity >> num_troops) {
        // Trim chars off of string to convert to int
        general_num.erase(0, 1);
        planet_num.erase(0, 1);
        force_sensitivity.erase(0, 1);
        num_troops.erase(0, 1);
        // DBG:
        // std::cout << "TIME: " << timestamp << "\nSIDE: " << side << "\nGENERAL NUMBER: " << general_num << "\nPLANET NUMBER: " << planet_num << "\nFORCE SENSITIVITY: " << force_sensitivity << "\nNUM TROOPS: " << num_troops << "\n";
        // Error checking
        check_errors(stoi(general_num), stoi(planet_num), stoi(num_troops), stoi(force_sensitivity), timestamp);
        // Update time here?
        current_time = timestamp;

        Deployment temp{timestamp, stoi(general_num), stoi(force_sensitivity), stoi(num_troops)};
        // If side is Jedi
        if(side[0] == 'J') {
            planets[(unsigned long)stoi(planet_num)].jedi_pq.push(temp);
            // After push, check on planet_num to see if a battle will take place there
        // If side is Sith
        } else {
            planets[(unsigned long)stoi(planet_num)].sith_pq.push(temp);
            // After push, check on planet_num to see if a battle will take place there
        }
    }

}

// Reads input from a pseudorandom input file
void read_PR() {
    std::string line;
    getline(std::cin, line);
    num_generals = line.back();
    getline(std::cin, line);
    planets.resize((unsigned long)line.back());

    std::string random_seed;
    std::string num_deployments;
    std::string arrival_rate;
    getline(std::cin, line);
    for(int i = int(line.size()) - 1; i > 0; --i) {
        if(line[(unsigned long)i] == ' ') {
            break;
        }
        random_seed += line[(unsigned long)i];
    }
    std::cout << random_seed << "\n";


}

int main(int argc, char * argv[]) {
    Option_handler oh;
    oh.get_options(argc, argv);

    // Read in the file
    std::string line;
    getline(std::cin, line);
    getline(std::cin, line);
    if(line[6] == 'D') {
        read_DL();
    } else {
        read_PR();
    }

}