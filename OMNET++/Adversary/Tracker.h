#include <omnetpp.h>
#include <veins/modules/application/Thesis_src/Prext/base/PrivBase.h>
#include <map>
#include <fstream>
#include <vector>
#include <veins/modules/application/Thesis_src/Prext/include/NNPDA.h>
#include <ctime>
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include <veins/modules/application/Thesis_src/messages/Beacon_m.h>
#include <vector>
using namespace std;
namespace veins {
class VEINS_API Tracker : public DemoBaseApplLayer
{

  protected:
    void initialize(int stage) override;
    void finish() override;

    ofstream outfile;
    vector<int> recordAddress;
    vector<double> recordPosX;
    vector<double> recordPosY;
    vector<double> recordSpeedX;
    vector<double> recordSpeedY;
    vector<int> recordPseudonym;
    vector<double> recordTime;
    vector<int> recordPCAID;
    vector<int> recordEavesdropperIdVec;
    vector<std::string> recordLaneID;
    vector<int> recordEncrypted;
    int recordFlag1600 = 0;
    int recordFlag1615 = 0;
    int recordFlag1630 = 0;
    int recordFlag1645 = 0;
    std::set<Beacon*> recordBeaconSet;


    map<int, std::string>mixZoneTime;
    map<int, std::string>VLifeTime;
    map<int, int>VPCount;

  public:
    virtual void beaconReceived(Beacon* beacon, int EavesdropperId);
    void recordingBeacon(vector<int>, vector<double>, vector<double>, vector<double>, vector<double>, vector<int>, vector<double>, vector<int>, vector<int>, vector<std::string>, vector<int>,string time);
    void setBeaconReceived(Beacon* beacon);
    void setMixZoneTime(int, string);
    void setVLifeTime(int, string);
    void setVPCount(int);
    std::set<Beacon*> mySet;

};
}
