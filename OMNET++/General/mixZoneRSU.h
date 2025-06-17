#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include <veins/modules/application/Thesis_src/messages/mixZoneAd_m.h>
#include <veins/base/modules/BaseApplLayer.h>

namespace veins {
class VEINS_API mixZoneRSU : public DemoBaseApplLayer {
public:
    void initialize(int stage) override;
    void finish() override;
    mixZoneAd* setMixZoneAd(mixZoneAd*);
    void handleSelfMsg(cMessage* msg) override;
    double advertiseInterval;
    int myId;
    int zoneCircularRange;
    cMessage* sendAdvertiseEvt;
    Coord Pos;

};
}
