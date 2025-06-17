#pragma once

#include <veins/modules/application/Thesis_src/messages/Beacon_m.h>
#include "veins/veins.h"
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include <veins/modules/application/Thesis_src/Adversary/Tracker.h>

using namespace omnetpp;

namespace veins {
class VEINS_API PrivateVehicleApp : public DemoBaseApplLayer {
public:
    void initialize(int stage) override;
    void finish() override;

protected:
    void changePsynm();
    void handleSelfMsg(cMessage* msg) override;
    void onBeacon(Beacon* beacon) override;
    void onMixZoneAd(mixZoneAd* mixzonead) override;
    Beacon* setBeacon(Beacon* beacon);
    void generatePsynmBaseline();
    void generatePsynmSwitch();
    bool isInMixZone();
    void receiveSignal(cComponent *source, simsignal_t signalID, cObject *value, cObject *details) override;
    void handlePositionUpdate(cObject* obj) override;
    void loadPsynms();
    std::vector<uint64_t> rollingPCAID(int, uint64_t, uint64_t);
    int schemeFlag;
    int changePseudonymFlag = 0;

    uint64_t pcaID;
    uint64_t pcaID1;
    uint64_t pcaID2;
    int vehicleId;
    cMessage* timerEvent;
    simtime_t startTime;
    simtime_t psynmStartTime;
    simtime_t mxzStartTime;
    simsignal_t pseudonymSwitchSignal;
    Tracker* tracker;
    int psynmLifetime = 60;
    struct mixZoneInfo {
          int mixZoneID;
          Coord mixZonePos;
          int mixZoneRange;
    };
    map<int, mixZoneInfo> mixZoneMap;

    struct Pseudonym {
        uint64_t psynmId; 
        simtime_t psynmStartTime;
        simtime_t psynmEndTime;
        uint64_t pcaID;
    };
    int pseudonymIndexA;
    int pseudonymIndexB;
    std::vector<Pseudonym> pseudonymPoolA;
    std::vector<Pseudonym> pseudonymPoolB;
    int currentPseudonymPool;
    Pseudonym currentPseudonym;
    bool isInMixZoneFlag = false;
    std::vector<uint64_t> rollingPCAIDVector;
};
}
