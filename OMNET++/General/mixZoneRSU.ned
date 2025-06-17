package org.car2x.veins.modules.application.Thesis_src.General;
import org.car2x.veins.modules.application.ieee80211p.DemoBaseApplLayer;

network mixZoneRSU extends DemoBaseApplLayer
{
    parameters:
        @class(veins::mixZoneRSU);
        @display("i=block/app2");
        double advertiseInterval @unit("s");
}