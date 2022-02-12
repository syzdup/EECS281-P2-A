// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
#include <iostream>
#include "Options.h"
#include "Planet.h"
#include "P2random.h"

std::vector<Planet> planets;
std::vector<General> generals;
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

// Reads deployments from an input stream
void read_deployments(std::istream &input_stream, bool verbose_on, bool median_on, bool general_eval_on, bool watcher_on) {
    std::cout << "Deploying troops...\n";
    int num_battles = 0;
    int id = 0;
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
        check_errors(stoi(general_num), stoi(planet_num), stoi(num_troops), stoi(force_sensitivity), timestamp);
        if(timestamp != current_time) {
            // Print median information here
            if(median_on && num_battles != 0) {
                for(int i = 0; i < int(planets.size()); ++i) {
                    if(planets[(unsigned long)i].battle_occured) {
                        std::cout << "Median troops lost on planet " << i << " at time " << current_time << " is " << planets[(unsigned long)i].get_median() << ".\n";
                    }
                }
            }
            current_time = timestamp;
        }
        Deployment temp{id, timestamp, stoi(general_num), stoi(force_sensitivity), stoi(num_troops)};
        id += 1;
        // If side is Jedi
        if(side[0] == 'J') {
            planets[(unsigned long)stoi(planet_num)].jedi_pq.push(temp);
            // If general eval on update general info
            if(general_eval_on) {
                generals[(unsigned long)stoi(general_num)].deployed += stoi(num_troops);
                generals[(unsigned long)stoi(general_num)].jedi_troops += stoi(num_troops);
                generals[(unsigned long)stoi(general_num)].survived += stoi(num_troops);
            }
            // If watcher mode is on, update the best possible attack and ambush passing in 1 for Jedi
            if(watcher_on) {
                planets[(unsigned long)stoi(planet_num)].update_best_attack(temp.timestamp, temp.force_sensitivity, 1);
            }
            // After push, check on planet_num to see if a battle will take place there
            planets[(unsigned long)stoi(planet_num)].check_match(verbose_on, stoi(planet_num), num_battles, general_eval_on, generals, watcher_on);
        // If side is Sith
        } else {
            planets[(unsigned long)stoi(planet_num)].sith_pq.push(temp);
            // If general eval on update general info
            if(general_eval_on) {
                generals[(unsigned long)stoi(general_num)].deployed += stoi(num_troops);
                generals[(unsigned long)stoi(general_num)].sith_troops += stoi(num_troops);
                generals[(unsigned long)stoi(general_num)].survived += stoi(num_troops);
            }
            // If watcher mode is on, update the best possible attack and ambush passing in 0 for Sith
            if(watcher_on) {
                planets[(unsigned long)stoi(planet_num)].update_best_attack(temp.timestamp, temp.force_sensitivity, 0);
            }
            // After push, check on planet_num to see if a battle will take place there
            planets[(unsigned long)stoi(planet_num)].check_match(verbose_on, stoi(planet_num), num_battles, general_eval_on, generals, watcher_on);
        }
    }
    // Print median information again if median mode is on
    if(median_on && num_battles != 0) {
        for(int i = 0; i < int(planets.size()); ++i) {
            if(planets[(unsigned long)i].battle_occured) {
                std::cout << "Median troops lost on planet " << i << " at time " << current_time << " is " << planets[(unsigned long)i].get_median() << ".\n";
            }
        }
    }
    // End of day summary
    std::cout << "---End of Day---\nBattles: " << num_battles << "\n";

    // General eval
    if(general_eval_on) {
        std::cout << "---General Evaluation---\n";
        for(int i = 0; i < int(generals.size()); ++i) {
            std::cout << "General " << i << " deployed " << generals[(unsigned long)i].jedi_troops << " Jedi troops and " 
            << generals[(unsigned long)i].sith_troops << " Sith troops, and " << generals[(unsigned long)i].survived << "/" << generals[(unsigned long)i].deployed << " troops survived.\n";
        }
    }
    if(watcher_on) {
        std::cout << "---Movie Watcher---\n";
        // Loop and print attacks
        for(int i = 0; i < int(planets.size()); ++i) {
            if(planets[(unsigned long)i].best_jedi_attack.best_timestamp == -1 || planets[(unsigned long)i].best_sith_attack.best_timestamp == -1) {
                planets[(unsigned long)i].best_jedi_attack.best_timestamp = -1;
                planets[(unsigned long)i].best_sith_attack.best_timestamp = -1;
            }
            std::cout << "A movie watcher would enjoy an attack on planet " << i << " with Jedi at time " <<
            planets[(unsigned long)i].best_jedi_attack.best_timestamp << " and Sith at time " << planets[(unsigned long)i].best_sith_attack.best_timestamp <<
            " with a force difference of " << planets[(unsigned long)i].best_sith_attack.best_force_sensitivity - planets[(unsigned long)i].best_jedi_attack.best_force_sensitivity <<
            ".\n";
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
    if(oh.get_general_eval_on()) {
        generals.resize((unsigned long)num_generals);
    }
    getline(std::cin, line);
    planets.resize((unsigned long)get_int_value(line));

    std::stringstream ss;
    std::istream &input_stream = mode[6] == 'P' ? ss : std::cin;

    // Mode is deployment list
    if(mode[6] == 'D') {
        read_deployments(input_stream, oh.get_verbose_on(), oh.get_median_on(), oh.get_general_eval_on(), oh.get_watcher_on());
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
        read_deployments(input_stream, oh.get_verbose_on(), oh.get_median_on(), oh.get_general_eval_on(), oh.get_watcher_on());
    }
}