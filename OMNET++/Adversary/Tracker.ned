package org.car2x.veins.modules.application.Thesis_src.Adversary;
import org.car2x.veins.modules.application.ieee80211p.DemoBaseApplLayer;

network Tracker extends DemoBaseApplLayer
{
   
    parameters:
        @class(veins::Tracker);
    	@display("i=block/app2");
    	
    	bool exportEncrypted  = default(false);
        bool exportBeacons  = default(true);
        bool enableTracking = default(true);        
        bool matlabfileformat = default(false);
        
        //Tracking algorithm parameters 
        double kalman_p0 = default(50);
        double kalman_q = default(1);
        double kalman_rp = default(5);
        double kalman_rv = default(1);                
        double gateSize = default(30);
        double anonomitySetPthreshold = default(0.01);
        
        //important parameters that should be carefully assigned
        double waitBfrDelete @unit(s) = default(2s);    	
    	double trackInterval @unit(s);
        double maxSilence @unit(s);
        int curiousPCAID = default(-1);
        
        @signal[vId](type=unsigned long);
    	@signal[maxEnt](type=double);
    	@signal[sumEnt](type=double);
    	@signal[maxAas](type=unsigned long);
    	@signal[conf](type=unsigned long);
    	@signal[psynmchng](type=unsigned long);
    	@signal[contTrackTimePer](type=double); //continous tracking time
    	@signal[monitorTime](type=double); // time difference between the last and first times a vehicle is seen by an eavesdropper
    	
    	@statistic[vehicleId](source=vId;title="vehicleId";record=vector);
    	@statistic[MonitorTime](source=monitorTime;title="MonitorTime";record=vector);
    	@statistic[maxEntropy](source=maxEnt;title="maximumEntropy";record=vector);
    	@statistic[sumEntropy](source=sumEnt;title="sumEntropy";record=vector);
    	@statistic[maxAnonymitySetSize](source=maxAas;title="maximumAnonymitySetSize";record=vector);
    	@statistic[ContTrackingTimePer](source=contTrackTimePer;title="ContinousTrackingTimePercentage";record=vector);
    	@statistic[nTrackerConfusion](source=conf;title="nTrackerConfusion";record=histogram);
    	@statistic[nPsynmChange](source=psynmchng;title="nPsynmChange";record=histogram);
}

