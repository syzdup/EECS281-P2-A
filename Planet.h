// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
#include "Deployment.h"
#include <queue>

class Planet {
    public:
        std::priority_queue<Deployment, std::vector<Deployment>, Jedi_Comparator> jedi_pq;
        std::priority_queue<Deployment, std::vector<Deployment>, Sith_Comparator> sith_pq;

        // Max heap for lower part of numbers, min heap for upper
        // If heaps are same size, take average of the top of each, else take the top of the larger one
        std::priority_queue<int> lower_pq;
        std::priority_queue<int, std::vector<int>, std::greater<int> > upper_pq;
        // Has a battle occured
        bool battle_occured = false;
        // Check to see if a match can be made
        void check_match(bool verbose_on, int planet_num, int &num_battles);

        // Trade troops one for one
        void fight(bool verbose_on, int planet_num, int &num_battles);

        // Update the running median for troops lost on the planet
        void update_median(int troops_lost);

        // Balances the number of elements between the two queues
        void balance_median();

        // Returns the current median of troops lost on planet
        double get_median();
      
};