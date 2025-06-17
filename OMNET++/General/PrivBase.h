#include <veins/base/modules/BaseApplLayer.h>
#include <veins/modules/application/Thesis_src/messages/Beacon_m.h>
#include <veins/modules/application/Thesis_src/Prext/include/kalmanTrack.h>
#include <ostream>
using namespace std;
namespace veins{
//TODO: implement dumpWAVEBeacon as a member function of a derived class of WAVEBeacon
inline void dumpBeacon(ostream & out, Beacon* beacon) {
    out << beacon->getTimestamp().trunc(SimTimeUnit::SIMTIME_MS) << "\t"
            << beacon->getSenderAddress() << "\t"
            << beacon->getSenderPos().x << "\t"
            << beacon->getSenderSpeed().x << "\t"
            << beacon->getSenderPos().y << "\t"
            << beacon->getSenderSpeed().y << "\t"
            << beacon->getPseudonym() << "\t"
            << beacon->getSenderAngle() << "\t"
            << beacon->getEncrypted() << endl;
}

}
