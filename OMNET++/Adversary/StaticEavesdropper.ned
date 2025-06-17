package org.car2x.veins.modules.application.Thesis_src.Adversary;
import org.car2x.veins.base.modules.IBaseApplLayer;

simple StaticEavesdropper like IBaseApplLayer
{
    parameters:
        @class(veins::StaticEavesdropper);
        @display("i=block/app2");
        int headerLength @unit("bit");
        bool dumpEncrypted;     
        bool localDump;    
        @signal[nbcn](type=unsigned long); // # of eavesdropped beacons
        @signal[npsynm](type=unsigned long); // # of distinct pseudonyms
    	  
    	@statistic[nBeacons](source=nbcn;title="#receivedBeacons";record=last);
    	@statistic[nPseudonyms](source=npsynm;title="#distinctPseudonyms";record=last);
    	
    gates:
        input lowerLayerIn; // from network layer
        output lowerLayerOut; // to network layer
        input lowerControlIn; // control from network layer
        output lowerControlOut; // control to network layer     

}