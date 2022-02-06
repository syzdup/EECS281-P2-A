// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
#include <iostream>
#include "Options.h"
#include "Planet.h"

std::vector<Planet> planets;
int num_generals;

// Reads input from a deployment list file
void read_DL() {
    std::string line;
    getline(std::cin, line);
    num_generals = line.back();
    getline(std::cin, line);
    planets.resize((unsigned long)line.back());

}

// Reads input from a pseudorandom input file
void read_PR() {
    std::string line;
    getline(std::cin, line);
    num_generals = line.back();
    getline(std::cin, line);
    planets.resize((unsigned long)line.back());
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