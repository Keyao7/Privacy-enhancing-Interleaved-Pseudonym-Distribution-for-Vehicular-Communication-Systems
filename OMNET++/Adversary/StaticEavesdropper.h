#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include <fstream>
#include <veins/modules/application/Thesis_src/Adversary/Tracker.h>
#include "veins/base/utils/Coord.h"
#include <set>
namespace veins {
class VEINS_API StaticEavesdropper : public DemoBaseApplLayer {
    struct VehicleData {
            std::string pseudonym;
            std::string pcaID;
            Coord position;
            simtime_t time;
    };
    public:
        void initialize(int stage) override;
        void finish() override;
        void onBeacon(Beacon* beacon) override;
    protected:
        std::ofstream outfile;
        bool dumpEncrypted;
        bool localDump;
        Tracker* tracker;
        int myId;
        simsignal_t sBcns;
        simsignal_t sPsynms;
        std::vector<uint64_t> encounteredPsynms;
        int nBeacons;
        vector<Beacon*> recordBeaconVec;
};
}
