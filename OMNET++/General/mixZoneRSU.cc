#include "mixZoneRSU.h"
#include <stdio.h>
#include <veins/modules/application/Thesis_src/General/PrivBase.h>
#include "veins/base/utils/FindModule.h"
#include <veins/modules/application/Thesis_src/messages/mixZoneAd_m.h>
using namespace veins;
Define_Module(veins::mixZoneRSU);

void mixZoneRSU::initialize(int stage) {
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        myId = findHost()->getId();
        sendAdvertiseEvt = new cMessage("RSU Ad evt");
        advertiseInterval = par("advertiseInterval");
        scheduleAt(simTime() + advertiseInterval, sendAdvertiseEvt);
    }
}

mixZoneAd* mixZoneRSU::setMixZoneAd(mixZoneAd* mixzonead){
    //EV << "mixzone" << curPosition <<std::endl;
    mixzonead->setSenderPos(curPosition);
    mixzonead->setChannelNumber(static_cast<int>(Channel::cch));
    mixzonead->addBitLength(beaconLengthBits);
    mixzonead->setUserPriority(beaconUserPriority);
    mixzonead->setPsid(-1);
    mixzonead->setMixZoneID(myId);
    return mixzonead;
}

void mixZoneRSU::handleSelfMsg(cMessage* msg){
    if (strcmp(msg->getName(), "RSU Ad evt") == 0) {
        if (static_cast<int>(simTime().dbl()) >= 57600){
            //
            mixZoneAd* mixzonead = new mixZoneAd();
            mixzonead = setMixZoneAd(mixzonead);
            sendDown(mixzonead);
            EV << findHost()->getId() << "sent mixZoneAd" << std::endl;
            scheduleAt(simTime() + advertiseInterval, sendAdvertiseEvt);
        }else{
            scheduleAt(simTime() + advertiseInterval, sendAdvertiseEvt);
        }

    }
}
void mixZoneRSU::finish() {
    DemoBaseApplLayer::finish();
    cancelAndDelete(sendAdvertiseEvt);
}
