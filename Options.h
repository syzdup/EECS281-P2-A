// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <getopt.h>

class Option_handler {

private:
    bool median_on = false;
    bool verbose_on = false;
    bool general_eval_on = false;
    bool watcher_on = false;
public:
    void get_options(int argc, char *argv[]) {
        opterr = false; 
        int choice;
        int option_index = 0;
        option long_options[] = {
            { "median",  no_argument,       nullptr, 'm'  },
            {"verbose", no_argument, nullptr, 'v'},
            {"general-eval", no_argument, nullptr, 'g'},
            {"watcher", no_argument, nullptr, 'w'},
            { nullptr, 0,                 nullptr, '\0' }
        };
        while ((choice = getopt_long(argc, argv, "mvgw", long_options, &option_index)) != -1) {
            switch (choice) {
                case 'm':
                    median_on = true;
                    break;
                case 'v':
                    verbose_on = true;
                    break;
                case 'g':
                    general_eval_on = true;
                    break;
                case 'w':
                    watcher_on = true;
                default:
                    std::cerr << "Unknown command line option\n";
                    exit(1);
            }
        }
    }
    bool get_median_on() { return median_on; }
    bool get_verbose_on() { return verbose_on; }
    bool get_general_eval_on() { return general_eval_on; }
    bool get_watcher_on() { return watcher_on; }
};
