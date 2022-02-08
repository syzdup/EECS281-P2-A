// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
#include "Deployment.h"
#include <queue>

// class Planet {
//     public:
//         std::priority_queue<Deployment> jedi_pq;
//         std::priority_queue<Deployment> sith_pq;

// };

struct Planet {
    std::priority_queue<Deployment, std::vector<Deployment>, Jedi_Comparator> jedi_pq;
    std::priority_queue<Deployment, std::vector<Deployment>, Sith_Comparator> sith_pq;
};