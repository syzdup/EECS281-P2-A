// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
#include "Deployment.h"
#include <queue>

class Planet {
    public:
        std::priority_queue<Deployment, std::vector<Deployment>, Jedi_Comparator> jedi_pq;
        std::priority_queue<Deployment, std::vector<Deployment>, Sith_Comparator> sith_pq;

        // Check to see if a match can be made
        void check_match(bool verbose_on, int planet_num);

        // Trade troops one for one
        void fight(bool verbose_on, int planet_num);
      
};