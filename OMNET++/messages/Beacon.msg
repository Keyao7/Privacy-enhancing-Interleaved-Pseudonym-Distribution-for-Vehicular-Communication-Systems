import veins.base.utils.SimpleAddress;
import veins.modules.messages.DemoSafetyMessage;

namespace veins;

packet Beacon extends DemoSafetyMessage {
    int senderAddress;
    uint64_t pseudonym;
    uint64_t pcaID;
    bool encrypted;
    double senderAngle;
    simtime_t timestamp = 0;
    bool CPN_readyFlag;
    string laneID;
    string roadID;
    int pseudonymChangeFlag;
}