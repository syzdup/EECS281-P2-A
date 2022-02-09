// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
class Deployment {

    public:
        int timestamp;
        int general_id;
        int force_sensitivity;
        int num_troops;
        Deployment(int ts, int gid, int fs, int nt)
            :timestamp(ts), general_id(gid), force_sensitivity(fs), num_troops(nt) {}       
};

class Sith_Comparator {
    public:
        bool operator()(const Deployment &D1, const Deployment &D2) {
            if(D1.force_sensitivity < D2.force_sensitivity) {
                return true;
            }
            return false;
        }
};

class Jedi_Comparator {
    public:
        bool operator()(const Deployment &D1, const Deployment &D2) {
            if(D1.force_sensitivity > D2.force_sensitivity) {
                return true;
            } else if(D1.force_sensitivity == D2.force_sensitivity) {
                if(D1.timestamp > D2.timestamp) {
                    return true;
                }
            }
            return false;
        }
};