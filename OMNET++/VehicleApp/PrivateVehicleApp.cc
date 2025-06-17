#include <veins/modules/application/Thesis_src/messages/Beacon_m.h>
#include <veins/modules/application/Thesis_src/messages/mixZoneAd_m.h>
#include <veins/modules/application/Thesis_src/VehicleApp/PrivateVehicleApp.h>
#include <random>
#include <veins/modules/application/Thesis_src/General/PrivBase.h>
#include <cmath>
#include <algorithm>
#include <chrono>    
using namespace veins;
Define_Module(veins::PrivateVehicleApp);

void PrivateVehicleApp::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        vehicleId = findHost()->getId();

        schemeFlag = par("schemeFlag").intValue();
        tracker = FindModule<Tracker*>::findSubModule(getParentModule()->getParentModule());
        tracker->setVLifeTime(vehicleId, simTime().str());
        if(!schemeFlag){
            generatePsynmBaseline();
        }else{
            generatePsynmSwitch();
        }
        mxzStartTime = 0;
        pseudonymIndexA = 0;
        pseudonymIndexB = 0;
        loadPsynms();
        tracker->setVPCount(vehicleId);
    }
}

void PrivateVehicleApp::loadPsynms(){
    if (!pseudonymPoolA.empty() && !pseudonymPoolB.empty()) {
        // Clean up pseudonym pool A
        auto newEndA = std::remove_if(pseudonymPoolA.begin(), pseudonymPoolA.end(),
            [this](const Pseudonym& pseudonym) {
                return simTime().dbl() > pseudonym.psynmEndTime.dbl();
            });
        int removedA = std::distance(newEndA, pseudonymPoolA.end());
        if (removedA > 0) {
            EV << "Removed " << removedA << " expired pseudonyms A\n";
            pseudonymPoolA.erase(newEndA, pseudonymPoolA.end());
        } else {
            EV << "No expired pseudonyms A to remove\n";
        }

        // Clean up pseudonym pool B
        auto newEndB = std::remove_if(pseudonymPoolB.begin(), pseudonymPoolB.end(),
            [this](const Pseudonym& pseudonym) {
                return simTime().dbl() > pseudonym.psynmEndTime.dbl();
            });
        int removedB = std::distance(newEndB, pseudonymPoolB.end());
        if (removedB > 0) {
            EV << "Removed " << removedB << " expired pseudonyms B\n";
            pseudonymPoolB.erase(newEndB, pseudonymPoolB.end());
        } else {
            EV << "No expired pseudonyms B to remove\n";
        }
    }

    // RIPD
    if(schemeFlag){
        rollingPCAIDVector = rollingPCAID(40, pcaID1, pcaID2);
    }
    double remainder = std::fmod(simTime().dbl(), static_cast<double>(psynmLifetime));
    simtime_t firstPStartTimeA = simtime_t(simTime().dbl() - remainder);
    simtime_t firstPStartTimeB = simtime_t(firstPStartTimeA.dbl() + psynmLifetime/2);
    for (int i = 0; i < 20; i++){
        std::string pseudonymIndexAString = std::to_string(pseudonymIndexA);
        while (pseudonymIndexAString.length() < 3) {
            pseudonymIndexAString = "0" + pseudonymIndexAString;
        }
        uint64_t pseudonymA = std::stoull( std::to_string(vehicleId) + "1" + pseudonymIndexAString);
        if (!schemeFlag){
            pseudonymPoolA.push_back({pseudonymA, simtime_t(firstPStartTimeA.dbl() + i * psynmLifetime), simtime_t(firstPStartTimeA.dbl() + (i+1) * psynmLifetime), pcaID});
        }else{
//            pseudonymPoolA.push_back({pseudonymA, simtime_t(firstPStartTimeA.dbl() + i * psynmLifetime), simtime_t(firstPStartTimeA.dbl() + (i+1) * psynmLifetime), pcaID1});
            pseudonymPoolA.push_back({pseudonymA, simtime_t(firstPStartTimeA.dbl() + i * psynmLifetime), simtime_t(firstPStartTimeA.dbl() + (i+1) * psynmLifetime), rollingPCAIDVector.back()});
            rollingPCAIDVector.pop_back();
        }


        std::string pseudonymIndexBString = std::to_string(pseudonymIndexB);
        while (pseudonymIndexBString.length() < 3) {
            pseudonymIndexBString = "0" + pseudonymIndexBString;
        }
        uint64_t pseudonymB = std::stoull( std::to_string(vehicleId) + "2" + pseudonymIndexBString);
        if (!schemeFlag){
            pseudonymPoolB.push_back({pseudonymB, simtime_t(firstPStartTimeB.dbl() + i * psynmLifetime), simtime_t(firstPStartTimeB.dbl() + (i+1) * psynmLifetime), pcaID});
        }else{
//            pseudonymPoolB.push_back({pseudonymB, simtime_t(firstPStartTimeB.dbl() + i * psynmLifetime), simtime_t(firstPStartTimeB.dbl() + (i+1) * psynmLifetime), pcaID2});
            pseudonymPoolB.push_back({pseudonymB, simtime_t(firstPStartTimeB.dbl() + i * psynmLifetime), simtime_t(firstPStartTimeB.dbl() + (i+1) * psynmLifetime), rollingPCAIDVector.back()});
            rollingPCAIDVector.pop_back();
        }

        pseudonymIndexA += 1;
        pseudonymIndexB += 1;
    }
    currentPseudonym = pseudonymPoolA[0];
    currentPseudonymPool = 1;
}

void PrivateVehicleApp::handleSelfMsg(cMessage* msg)
{
    if (strcmp(msg->getName(), "beacon evt") == 0) {
        if (static_cast<int>(simTime().dbl()) >= 57600){
            //57600
            if (currentPseudonym.psynmEndTime.dbl() <= simTime().dbl()){
                EV << "Have to change P" << std::endl;
                changePsynm();
                changePseudonymFlag = 1;
            }
            Beacon* beacon = new Beacon();
            beacon = setBeacon(beacon);
            if (isInMixZone()){
                beacon->setEncrypted(true);
            }else{
                beacon->setEncrypted(false);
            }
            sendDown(beacon);

            EV << findHost()->getId() << "sent beacon" << std::endl;
            scheduleAt(simTime() + beaconInterval, sendBeaconEvt);
        }else{
            scheduleAt(simTime() + beaconInterval, sendBeaconEvt);
        }

    }
}

void PrivateVehicleApp::receiveSignal(cComponent *source, simsignal_t signalID, cObject *value, cObject *details){
}

void PrivateVehicleApp::changePsynm() {
    bool changeFlag = false;
    if(currentPseudonymPool == 1){
        // Use std::find_if to find a pseudonym in pseudonymPoolB valid for the current time
        auto itB = std::find_if(pseudonymPoolB.begin(), pseudonymPoolB.end(),
            [this](const Pseudonym& pseudonym) {
                double currentTime = simTime().dbl();
                return (currentTime < pseudonym.psynmEndTime.dbl()) &&
                       (currentTime >= pseudonym.psynmStartTime.dbl());
            });

        if(itB != pseudonymPoolB.end()){
            // Correct assignment
            currentPseudonym.psynmId = itB->psynmId;
            currentPseudonym.pcaID = itB->pcaID;
            currentPseudonym.psynmStartTime = itB->psynmStartTime;
            currentPseudonym.psynmEndTime = itB->psynmEndTime;

            changeFlag = true;
            currentPseudonymPool = 2; // Switch to pool B
            EV << "Switched pseudonym to: " << currentPseudonym.psynmId << " from pool B" << std::endl;
        }

        if (!changeFlag){
        auto itA = std::find_if(pseudonymPoolA.begin(), pseudonymPoolA.end(),
            [this](const Pseudonym& pseudonym) {
                double currentTime = simTime().dbl();
                return (currentTime < pseudonym.psynmEndTime.dbl()) &&
                       (currentTime >= pseudonym.psynmStartTime.dbl());
            });

        if(itA != pseudonymPoolA.end()){
            // Correct assignment
            currentPseudonym.psynmId = itA->psynmId;
            currentPseudonym.pcaID = itA->pcaID;
            currentPseudonym.psynmStartTime = itA->psynmStartTime;
            currentPseudonym.psynmEndTime = itA->psynmEndTime;

            changeFlag = true;
            currentPseudonymPool = 1; // Switch to pool A
            EV << "Switched pseudonym to: " << currentPseudonym.psynmId << " from pool A" << std::endl;
        }
        }

        if (!changeFlag) {
            EV << "No valid pseudonym found in pool B. Loading new pseudonyms." << std::endl;
            loadPsynms();
        }
    }
    else if(currentPseudonymPool == 2){
       // Use std::find_if to find a pseudonym in pseudonymPoolA valid for the current time
        auto itA = std::find_if(pseudonymPoolA.begin(), pseudonymPoolA.end(),
            [this](const Pseudonym& pseudonym) {
                double currentTime = simTime().dbl();
                return (currentTime < pseudonym.psynmEndTime.dbl()) &&
                       (currentTime >= pseudonym.psynmStartTime.dbl());
            });

        if(itA != pseudonymPoolA.end()){
            // Correct assignment
            currentPseudonym.psynmId = itA->psynmId;
            currentPseudonym.pcaID = itA->pcaID;
            currentPseudonym.psynmStartTime = itA->psynmStartTime;
            currentPseudonym.psynmEndTime = itA->psynmEndTime;

            changeFlag = true;
            currentPseudonymPool = 1; // Switch to pool A
            EV << "Switched pseudonym to: " << currentPseudonym.psynmId << " from pool A" << std::endl;
        }

        if(!changeFlag){
            auto itB = std::find_if(pseudonymPoolB.begin(), pseudonymPoolB.end(),
                [this](const Pseudonym& pseudonym) {
                    double currentTime = simTime().dbl();
                    return (currentTime < pseudonym.psynmEndTime.dbl()) &&
                           (currentTime >= pseudonym.psynmStartTime.dbl());
                });

            if(itB != pseudonymPoolB.end()){
                // Correct assignment
                currentPseudonym.psynmId = itB->psynmId;
                currentPseudonym.pcaID = itB->pcaID;
                currentPseudonym.psynmStartTime = itB->psynmStartTime;
                currentPseudonym.psynmEndTime = itB->psynmEndTime;

                changeFlag = true;
                currentPseudonymPool = 2; // Switch to pool B
                EV << "Switched pseudonym to: " << currentPseudonym.psynmId << " from pool B" << std::endl;
            }
        }

        if (!changeFlag) {
            EV << "No valid pseudonym found in pool A. Loading new pseudonyms." << std::endl;
            loadPsynms();
        }
    }
    else{
        EV << "Invalid currentPseudonymPool value: " << currentPseudonymPool << std::endl;
    }
    tracker->setVPCount(vehicleId);
}


Beacon* PrivateVehicleApp::setBeacon(Beacon* beacon){
    //mobility

    beacon->setSenderAddress(vehicleId);
    beacon->setSenderPos(mobility->getPositionAt(simTime()));
    beacon->setSenderSpeed(mobility->getHostSpeed());
    beacon->setChannelNumber(static_cast<int>(Channel::cch));
    beacon->addBitLength(beaconLengthBits);
    beacon->setUserPriority(beaconUserPriority);
    beacon->setPcaID(currentPseudonym.pcaID);
    beacon->setPseudonym(currentPseudonym.psynmId);
    beacon->setSenderAngle(mobility->getHeading().getRad());
    beacon->setTimestamp(simTime());
    beacon->setPsid(-1);
    beacon->setLaneID(traciVehicle->getLaneId().c_str());
    beacon->setRoadID(mobility->getRoadId().c_str());
    beacon->setPseudonymChangeFlag(changePseudonymFlag);
    changePseudonymFlag = 0;
    return beacon;
}

void PrivateVehicleApp::onBeacon(Beacon* beacon){
    tracker->setBeaconReceived(beacon);
}

void PrivateVehicleApp::onMixZoneAd(mixZoneAd* mixzonead){
    mixZoneInfo mzi;
    if (mixzonead->getSenderPos().distance(mobility->getPositionAt(simTime())) <= 50){
        mzi.mixZonePos = mixzonead->getSenderPos();
        mzi.mixZoneRange = 50;
        mixZoneMap[mixzonead->getMixZoneID()] = mzi; // 添加或更新混合区信息
    }
}


bool PrivateVehicleApp::isInMixZone() {
    bool currentlyInMixZone = false;

    // Check whether the vehicle is within any mix zone
    for (const auto& mixZoneEntry : mixZoneMap) {
        if (mixZoneEntry.second.mixZonePos.distance(mobility->getPositionAt(simTime())) <= mixZoneEntry.second.mixZoneRange) {
            currentlyInMixZone = true;
            break;
        }
    }

    if (currentlyInMixZone) {
        if (!isInMixZoneFlag) {
            // Just entered a mix zone
            isInMixZoneFlag = true;
            mxzStartTime = simTime();
            EV << "车辆进入混合区，位置：" << mobility->getPositionAt(simTime()) << std::endl;
            changePsynm();
            changePseudonymFlag = 2;
            tracker->setCountEnterMixZone();
        }
        // Continue staying in the mix zone (no pseudonym change)
    } else {
        if (isInMixZoneFlag) {
            string text = mxzStartTime.str() + "-" + simTime().str();
            if (mxzStartTime > 57600){
                tracker->setMixZoneTime(vehicleId,text);
            }
                     // 刚离开混合区
            isInMixZoneFlag = false;
            mxzStartTime = 0;
             EV << "Vehicle left mix zone, position: " << mobility->getPositionAt(simTime()) << std::endl;
            tracker->setCountExitMixZone();
        }
    }

    return currentlyInMixZone;
}



void PrivateVehicleApp::generatePsynmBaseline(){
    pcaID = intuniform(1,5);
}

void PrivateVehicleApp::generatePsynmSwitch(){

    pcaID1 = intuniform(1,5);
    do {
        pcaID2 = intuniform(1,5);
    } while (pcaID2 == pcaID1);
    pcaID = pcaID1;
}

void PrivateVehicleApp::finish(){
    DemoBaseApplLayer::finish();
}

std::vector<uint64_t> PrivateVehicleApp::rollingPCAID(int size, uint64_t PCAID1, uint64_t PCAID2){
    int num_x = size / 2;
    int num_y = size / 2;

    // If the count is odd, randomly pick one of x or y to increment
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 1);

    if (size % 2 != 0) {
        if (dist(gen) == 0) {
            num_x++;
        } else {
            num_y++;
        }
    }

    // Create an initial alternating array of x and y
    std::vector<uint64_t> result;
    result.reserve(size);

    bool toggle = true;  // Used to alternate insertion of x and y
    while (num_x > 0 || num_y > 0) {
        if (toggle && num_x > 0) {
            result.push_back(PCAID1);
            num_x--;
        } else if (!toggle && num_y > 0) {
            result.push_back(PCAID2);
            num_y--;
        }
        toggle = !toggle;  // Switch which element to insert
    }

    // Shuffle the sequence by randomly swapping elements multiple times 
    std::uniform_int_distribution<> swap_dist(0, size - 1);
    for (int i = 0; i < size; ++i) {
        int idx1 = swap_dist(gen);
        int idx2 = swap_dist(gen);
        std::swap(result[idx1], result[idx2]);
    }

    return result;
}

void PrivateVehicleApp::handlePositionUpdate(cObject* obj)
{
    DemoBaseApplLayer::handlePositionUpdate(obj);
}

