package org.car2x.veins.modules.application.Thesis_src.scenarios;
import org.car2x.veins.nodes.RSU;
import org.car2x.veins.nodes.Scenario;
import org.car2x.veins.modules.application.Thesis_src.Adversary.*;

network ObserverScenarioLuST extends Scenario
{
    int nEavesdropper;
    int nMXRSU; 
    @display("bgl=2;");
    submodules:
        tracker: Tracker {
            @display("p=60,60;i=device/antennatower;is=l");
        }
        Eavesdropper[nEavesdropper]: RSU {
            @display("p=150,140;i=veins/node/truck,red;is=s");
        }
        MxZ[nMXRSU]: RSU {
            @display("p=233,163;r=300,lavender,,1;i=block/routing;is=s");
        }   
}
 