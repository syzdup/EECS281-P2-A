// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
#include "Planet.h"

#include <iostream>
// Check to see if a fight will happen, to be called after pushing a new deployment to PQ
void Planet::check_match(bool verbose_on, int planet_num, int &num_battles, bool general_eval_on, std::vector<General> &generals) {
    if(!jedi_pq.empty() && !sith_pq.empty()) {
        if(jedi_pq.top().force_sensitivity <= sith_pq.top().force_sensitivity) {
            fight(verbose_on, planet_num, num_battles, general_eval_on, generals);
            battle_occured = true;
        }
    }
}

void Planet::fight(bool verbose_on, int planet_num, int &num_battles, bool general_eval_on, std::vector<General> &generals) {
    int troops_lost = 0;
    num_battles += 1;
    Deployment temp_jedi(jedi_pq.top().unique_id ,jedi_pq.top().timestamp, jedi_pq.top().general_id, jedi_pq.top().force_sensitivity, jedi_pq.top().num_troops);
    Deployment temp_sith(sith_pq.top().unique_id ,sith_pq.top().timestamp, sith_pq.top().general_id, sith_pq.top().force_sensitivity, sith_pq.top().num_troops);
    if(jedi_pq.top().num_troops > sith_pq.top().num_troops) {
        jedi_pq.pop();
        temp_jedi.num_troops -= temp_sith.num_troops;
        troops_lost = 2 * temp_sith.num_troops;
        jedi_pq.push(temp_jedi);
        sith_pq.pop();
        if(general_eval_on) {
            if((unsigned long)temp_jedi.general_id == (unsigned long)temp_sith.general_id) {
                generals[(unsigned long)temp_jedi.general_id].survived -= troops_lost;
            } else {
                generals[(unsigned long)temp_jedi.general_id].survived -= temp_sith.num_troops;
                generals[(unsigned long)temp_sith.general_id].survived -= temp_sith.num_troops;
            }
        }
    } else if(jedi_pq.top().num_troops < sith_pq.top().num_troops) {
        sith_pq.pop();
        temp_sith.num_troops -= temp_jedi.num_troops;
        troops_lost = 2 * temp_jedi.num_troops;
        sith_pq.push(temp_sith);
        jedi_pq.pop();
        if(general_eval_on) {
            if((unsigned long)temp_jedi.general_id == (unsigned long)temp_sith.general_id) {
                generals[(unsigned long)temp_jedi.general_id].survived -= troops_lost;
            } else {
                generals[(unsigned long)temp_jedi.general_id].survived -= temp_jedi.num_troops;
                generals[(unsigned long)temp_sith.general_id].survived -= temp_jedi.num_troops;
            }
        }
    } else {
        troops_lost = 2 * temp_jedi.num_troops;
        sith_pq.pop();
        jedi_pq.pop();
        if(general_eval_on) {
            if((unsigned long)temp_jedi.general_id == (unsigned long)temp_sith.general_id) {
                generals[(unsigned long)temp_jedi.general_id].survived -= troops_lost;
            } else {
                generals[(unsigned long)temp_jedi.general_id].survived -= temp_jedi.num_troops;
                generals[(unsigned long)temp_sith.general_id].survived -= temp_jedi.num_troops;
            }
        }
    }
    if(verbose_on) {
        std::cout << "General " << temp_sith.general_id << "'s battalion attacked General " << temp_jedi.general_id <<
        "'s battalion on planet " << planet_num << ". " << troops_lost << " troops were lost.\n";
    }
    // After a battle occurs, update the median, balance
    update_median(troops_lost);
    balance_median();
    check_match(verbose_on, planet_num, num_battles, general_eval_on, generals);
}

void Planet::update_median(int troops_lost) {
    if(lower_pq.empty() || troops_lost < lower_pq.top()) {
        lower_pq.push(troops_lost);
    } else {
        upper_pq.push(troops_lost);
    }
}

void Planet::balance_median() {
    if(upper_pq.size() > lower_pq.size()) {
        if(upper_pq.size() - lower_pq.size() > 1) {
            lower_pq.push(upper_pq.top());
            upper_pq.pop();
        }
    } else if(lower_pq.size() > upper_pq.size()) {
        if(lower_pq.size() - upper_pq.size() > 1) {
            upper_pq.push(lower_pq.top());
            lower_pq.pop();
        }
    }
}

double Planet::get_median() {
    if(lower_pq.size() > upper_pq.size()) {
        return double(lower_pq.top());
    } else if(lower_pq.size() == upper_pq.size()) {
        return (double(lower_pq.top()) + double(upper_pq.top())) / 2;
    } else {
        return double(upper_pq.top());
    }
}