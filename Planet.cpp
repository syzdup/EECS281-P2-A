// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
#include "Planet.h"
#include <iostream>
// Check to see if a fight will happen, to be called after pushing a new deployment to PQ
void Planet::check_match(bool verbose_on, int planet_num) {
    if(!jedi_pq.empty() && !sith_pq.empty()) {
        if(jedi_pq.top().force_sensitivity <= sith_pq.top().force_sensitivity) {
        fight(verbose_on, planet_num);
        }
    }
}

void Planet::fight(bool verbose_on, int planet_num) {
    int troops_lost = 0;
    Deployment temp_jedi(jedi_pq.top().timestamp, jedi_pq.top().general_id, jedi_pq.top().force_sensitivity, jedi_pq.top().num_troops);
    Deployment temp_sith(sith_pq.top().timestamp, sith_pq.top().general_id, sith_pq.top().force_sensitivity, sith_pq.top().num_troops);
    if(jedi_pq.top().num_troops > sith_pq.top().num_troops) {
        jedi_pq.pop();
        temp_jedi.num_troops -= temp_sith.num_troops;
        troops_lost = 2 * temp_sith.num_troops;
        jedi_pq.push(temp_jedi);
        sith_pq.pop();
    } else if(jedi_pq.top().num_troops < sith_pq.top().num_troops) {
        sith_pq.pop();
        temp_sith.num_troops -= temp_jedi.num_troops;
        troops_lost = 2 * temp_jedi.num_troops;
        sith_pq.push(temp_sith);
        jedi_pq.pop();
    } else {
        troops_lost = 2 * temp_jedi.num_troops;
        sith_pq.pop();
        jedi_pq.pop();
    }
    if(verbose_on) {
        std::cout << "General " << temp_sith.general_id << "'s battalion attacked General " << temp_jedi.general_id <<
        "'s battalion on planet " << planet_num << ". " << troops_lost << " troops were lost.\n";
    }
}