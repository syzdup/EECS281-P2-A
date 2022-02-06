// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
class Deployment {
    private:
        int timestamp;
        int general_id;
        int force_sensitivity;
        int num_troops;
    public:
        Deployment(int ts, int gid, int fs, int nt)
            :timestamp(ts), general_id(gid), force_sensitivity(fs), num_troops(nt) {}
};