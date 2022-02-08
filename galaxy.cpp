// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
#include <iostream>
#include "Options.h"
#include "Planet.h"
#include "P2random.h"

std::vector<Planet> planets;
int num_generals;
int current_time = 0;

// Checks for errors when reading a deployment
void check_errors(const int general_num, const int planet_num, const int num_troops, const int force_sensitivity, const int timestamp) {
    if(general_num < 0 || general_num >= num_generals) {
        std::cerr << "Invalid general ID\n";
        exit(1);
    }
    if(planet_num < 0 || planet_num >= int(planets.size())) {
        std::cerr << "Invalid planet ID\n";
        exit(1);
    }
    if(num_troops <= 0) {
        std::cerr << "Invalid number of troops\n";
        exit(1);
    }
    if(force_sensitivity <= 0) {
        std::cerr << "Invalid force sensitivity level\n";
        exit(1);
    }
    if(timestamp < current_time) {
        std::cerr << "Invalid decreasing timestamp\n";
        exit(1);
    }
}

// Returns an integer from a given string (used mainly for extracting from getline())
int get_int_value(std::string line) {
    size_t line_index = 0;
    for ( ; line_index < line.length(); ++line_index) { 
        if(isdigit(line[line_index])) {
            break;
        }
    }
    line = line.substr(line_index, line.length() - line_index);
    return atoi(line.c_str());
}

// Reads input from a deployment list file
void read_deployments(std::istream &input_stream) {

    int timestamp;
    std::string side;
    std::string general_num;
    std::string planet_num;
    std::string force_sensitivity;
    std::string num_troops;

    while(input_stream >> timestamp >> side >> general_num >> planet_num >> force_sensitivity >> num_troops) {
        // Trim chars off of string to convert to int
        general_num.erase(0, 1);
        planet_num.erase(0, 1);
        force_sensitivity.erase(0, 1);
        num_troops.erase(0, 1);
        // Error checking
        // Are these conversions necessary or is there a better way of handling it? Would making more integers increase memory and decrease speed?
        check_errors(stoi(general_num), stoi(planet_num), stoi(num_troops), stoi(force_sensitivity), timestamp);
        // Update time here
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

int main(int argc, char * argv[]) {
    Option_handler oh;
    oh.get_options(argc, argv);

    std::string line;
    std::string mode;
    // Read comment, get mode, number of generals, number of planets
    getline(std::cin, line);
    getline(std::cin, mode);
    getline(std::cin, line);
    num_generals = get_int_value(line);
    getline(std::cin, line);
    planets.resize((unsigned long)get_int_value(line));

    std::stringstream ss;
    std::istream &input_stream = mode[6] == 'P' ? ss : std::cin;

    // Mode is deployment list
    if(mode[6] == 'D') {
        read_deployments(input_stream);
    // Mode is pseudorandom
    } else {
        unsigned int random_seed;
        unsigned int num_deployments;
        unsigned int arrival_rate;
        // Get random seed, number of deployments, arrival rate
        getline(std::cin, line);
        random_seed = (unsigned int)get_int_value(line);
        getline(std::cin, line);
        num_deployments = (unsigned int)get_int_value(line);
        getline(std::cin, line);
        arrival_rate = (unsigned int)get_int_value(line);

        P2random::PR_init(ss, random_seed, (unsigned int)num_generals, (unsigned int)planets.size(), num_deployments, arrival_rate);
        read_deployments(input_stream);
    }
}