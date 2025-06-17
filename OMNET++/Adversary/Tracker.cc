#include <cmodule.h>
#include "Tracker.h"
#include <veins/modules/application/Thesis_src/messages/Beacon_m.h>
#include <fstream>
using namespace veins;
Define_Module(veins::Tracker);
void Tracker::initialize(int stage)
{
    if (stage == 0) {
    }

}

void Tracker::finish() {
    recordingBeacon(recordAddress, recordPosX, recordPosY, recordSpeedX, recordSpeedY, recordPseudonym, recordTime, recordPCAID, recordEavesdropperIdVec, recordLaneID,recordEncrypted,"1645");
}

void Tracker::beaconReceived(Beacon* beacon, int EavesdropperId) {
    if(beacon->getTimestamp() > 58500 && recordFlag1600 == 0){
        recordingBeacon(recordAddress, recordPosX, recordPosY, recordSpeedX, recordSpeedY, recordPseudonym, recordTime, recordPCAID, recordEavesdropperIdVec, recordLaneID,recordEncrypted,"1600");
        recordAddress.clear();
        recordPosX.clear();
        recordPosY.clear();
        recordSpeedX.clear();
        recordSpeedY.clear();
        recordPseudonym.clear();
        recordTime.clear();
        recordPCAID.clear();
        recordEavesdropperIdVec.clear();
        recordLaneID.clear();
        recordEncrypted.clear();
        recordFlag1600 = 1;
    }
    if(beacon->getTimestamp() > 59400 && recordFlag1600 == 1 && recordFlag1615 == 0){
        recordingBeacon(recordAddress, recordPosX, recordPosY, recordSpeedX, recordSpeedY, recordPseudonym, recordTime, recordPCAID, recordEavesdropperIdVec, recordLaneID,recordEncrypted,"1615");

        recordAddress.clear();
        recordPosX.clear();
        recordPosY.clear();
        recordSpeedX.clear();
        recordSpeedY.clear();
        recordPseudonym.clear();
        recordTime.clear();
        recordPCAID.clear();
        recordEavesdropperIdVec.clear();
        recordLaneID.clear();
        recordEncrypted.clear();
        recordFlag1615 = 1;
    }
    if(beacon->getTimestamp() > 60300 && recordFlag1600 == 1 && recordFlag1615 == 1 && recordFlag1630 == 0){
        recordingBeacon(recordAddress, recordPosX, recordPosY, recordSpeedX, recordSpeedY, recordPseudonym, recordTime, recordPCAID, recordEavesdropperIdVec, recordLaneID,recordEncrypted,"1630");
        recordAddress.clear();
        recordPosX.clear();
        recordPosY.clear();
        recordSpeedX.clear();
        recordSpeedY.clear();
        recordPseudonym.clear();
        recordTime.clear();
        recordPCAID.clear();
        recordEavesdropperIdVec.clear();
        recordLaneID.clear();
        recordEncrypted.clear();
        recordFlag1630 = 1;
    }
    recordAddress.push_back(beacon->getSenderAddress());
    recordPosX.push_back(beacon->getSenderPos().x);
    recordPosY.push_back(beacon->getSenderPos().y);
    recordSpeedX.push_back(beacon->getSenderSpeed().x);
    recordSpeedY.push_back(beacon->getSenderSpeed().y);
    recordPseudonym.push_back(beacon->getPseudonym());
    recordTime.push_back(beacon->getTimestamp().dbl());
    recordPCAID.push_back(beacon->getPcaID());
    recordEavesdropperIdVec.push_back(EavesdropperId);
    recordLaneID.push_back(beacon->getLaneID());
    recordEncrypted.push_back(beacon->getEncrypted());
    EV << EavesdropperId << "Got" << std::endl;
    recordBeaconSet.insert(beacon);

}





void Tracker::recordingBeacon(vector<int> recordAddress, vector<double> recordPosX, vector<double> recordPosY, vector<double> recordSpeedX, vector<double> recordSpeedY, vector<int> recordPseudonym, vector<double> recordTime, vector<int> recordPCAID, vector<int> recordEavesdropperIdVec, vector<std::string> recordLaneID, vector<int> recordEncrypted,string time){
    // 生成文件名
    std::string fileNameAdress = "results/Listener/"+time+"/Baseline_"+time+"_PeriodicalPC_3PCA_Address.txt";
    std::string fileNamePosX = "results/Listener/"+time+"/Baseline_"+time+"_PeriodicalPC_3PCA_PosX.txt";
    std::string fileNamePosY = "results/Listener/"+time+"/Baseline_"+time+"_PeriodicalPC_3PCA_PosY.txt";
    std::string fileNameSpeedX = "results/Listener/"+time+"/Baseline_"+time+"_PeriodicalPC_3PCA_SpeedX.txt";
    std::string fileNameSpeedY = "results/Listener/"+time+"/Baseline_"+time+"_PeriodicalPC_3PCA_SpeedY.txt";
    std::string fileNamePseudonym = "results/Listener/"+time+"/Baseline_"+time+"_PeriodicalPC_3PCA_Pseudonym.txt";
    std::string fileNameTime = "results/Listener/"+time+"/Baseline_"+time+"_PeriodicalPC_3PCA_Time.txt";
    std::string fileNamePCAID = "results/Listener/"+time+"/Baseline_"+time+"_PeriodicalPC_3PCA_PCAID.txt";
    std::string fileEavesdropperId = "results/Listener/"+time+"/Baseline_"+time+"_PeriodicalPC_3PCA_EavesdropperId.txt";
    std::string fileLaneID = "results/Listener/"+time+"/Baseline_"+time+"_PeriodicalPC_3PCA_LaneID.txt";
    std::string fileEncrypted = "results/Listener/"+time+"/Baseline_"+time+"_PeriodicalPC_3PCA_Encrypted.txt";

    // 检查文件是否成功打开
    std::ofstream outFile1(fileNameAdress, std::ios::app);
    if (outFile1.is_open()) {
        for (const auto& item : recordAddress) {
            outFile1 << item << std::endl;
        }
        outFile1.close();
    }

    std::ofstream outFile2(fileNamePosX, std::ios::app);
    if (outFile2.is_open()) {
        for (const auto& item : recordPosX) {
            outFile2 << item << std::endl;
        }
        outFile2.close();
    }

    std::ofstream outFile3(fileNamePosY, std::ios::app);
    if (outFile3.is_open()) {
        for (const auto& item : recordPosY) {
            outFile3 << item << std::endl;
        }
        outFile3.close();
    }
    std::ofstream outFile4(fileNameSpeedX, std::ios::app);
    if (outFile4.is_open()) {
        for (const auto& item : recordSpeedX) {
            outFile4 << item << std::endl;
        }
        outFile4.close();
    }
    std::ofstream outFile5(fileNameSpeedY, std::ios::app);
    if (outFile5.is_open()) {
        for (const auto& item : recordSpeedY) {
            outFile5 << item << std::endl;
        }
        outFile5.close();
    }
    std::ofstream outFile6(fileNamePseudonym, std::ios::app);
    if (outFile6.is_open()) {
        for (const auto& item : recordPseudonym) {
            outFile6 << item << std::endl;
        }
        outFile6.close();
    }
    std::ofstream outFile7(fileNameTime, std::ios::app);
    if (outFile7.is_open()) {
        for (const auto& item : recordTime) {
            outFile7 << item << std::endl;
        }
        outFile7.close();
    }

    std::ofstream outFile8(fileNamePCAID, std::ios::app);
    if (outFile8.is_open()) {
        for (const auto& item : recordPCAID) {
            outFile8 << item << std::endl;
        }
        outFile8.close();
    }

    std::ofstream outFile9(fileEavesdropperId, std::ios::app);
    if (outFile9.is_open()) {
        for (const auto& item : recordEavesdropperIdVec) {
            outFile9 << item << std::endl;
        }
        outFile9.close();
    }

    std::ofstream outFile10(fileLaneID, std::ios::app);
    if (outFile10.is_open()) {
        for (const auto& item : recordLaneID) {
            outFile10 << item << std::endl;
        }
        outFile10.close();
    }
    std::ofstream outFile11(fileEncrypted, std::ios::app);
    if (outFile11.is_open()) {
        for (const auto& item : recordEncrypted) {
            outFile11 << item << std::endl;
        }
        outFile11.close();
    }


}


void Tracker::setBeaconReceived(Beacon* beacon){
    mySet.insert(beacon);
}

void Tracker::setMixZoneTime(int VID, string text){
    mixZoneTime[VID] = text;
}

void Tracker::setVLifeTime(int VID, string text){
    VLifeTime[VID] = text;
}

void Tracker::setVPCount(int VID){
    VPCount[VID] = VPCount[VID] +1;
}