#include "StaticEavesdropper.h"
#include <stdio.h>
#include <veins/modules/application/Thesis_src/General/PrivBase.h>
#include "veins/base/utils/FindModule.h"
#include <veins/modules/application/Thesis_src/messages/Beacon_m.h>
using namespace veins;
Define_Module(veins::StaticEavesdropper);

void StaticEavesdropper::initialize(int stage) {
    BaseApplLayer::initialize(stage);
    if (stage == 0) {
        myId = findHost()->getId();
        dumpEncrypted = par("dumpEncrypted").boolValue();
        localDump = par("localDump").boolValue();
        nBeacons = 0;
        encounteredPsynms.clear();
        tracker = FindModule<Tracker*>::findSubModule(getParentModule()->getParentModule());
        ASSERT(tracker);
        if (localDump) {
            char filename[50];
            sprintf(filename, "Eavesdropper_%d.txt", myId);
            outfile.open(filename,ios::out);
            outfile.setf(ios::fixed, ios::floatfield);
            outfile.precision(4);
        }

        sBcns = registerSignal("nbcn");
        sPsynms = registerSignal("npsynm");
    }
}
void StaticEavesdropper::onBeacon(Beacon* beacon){
    tracker->beaconReceived(beacon, findHost()->getId());
    tracker->setCountNE();
    EV << "Got V position" << beacon->getSenderPos() << std::endl;
}
void StaticEavesdropper::finish() {
    if (localDump) outfile.close();
}
