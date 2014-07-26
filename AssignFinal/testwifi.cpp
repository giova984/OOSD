#include <utility>
#include "catch.hpp"
#include "simul.hpp"
#include "link.hpp"
#include "netinterface.hpp"
#include "node.hpp"
#include "routing.hpp"
#include "wifistatistics.hpp"


using namespace MetaSim;

#define BIG_TIME_AMOUNT 1000000
void runDebugSimul(){
    SIMUL.initRuns();
    SIMUL.initSingleRun();
    SIMUL.run_to(BIG_TIME_AMOUNT);
    SIMUL.endSingleRun();
}

SCENARIO( "Two wifi nodes sharing a single channel" ) {

    GIVEN( "nodes are reachable each other" ) {

        // (simulation objects creation and initialization)
        WifiLink link("2.412Ghz");
        Node n1("Node1");
        Node n2("Node2");
        WifiInterface i1("Int1", n1, std::make_pair(0,0), 1, link, 1);
        WifiInterface i2("Int2", n2, std::make_pair(0,1), 1, link, 2);

        n1.setInterval(std::unique_ptr<RandomVar>(new DeltaVar(10)));
        n2.setInterval(std::unique_ptr<RandomVar>(new DeltaVar(10)));

        NodeMessageReceivedStat rec("");
        rec.attach(&n1);
        rec.attach(&n2);
        LinkCollisionStat coll("");
        coll.attach(&link);

        WHEN( "A packet is sent to a connected node" ) {
            n1.addDestNode(n2);
            n1.setMessageToSend(1);

            runDebugSimul();
//            while (!rec.getValue() == 1)
//                SIMUL.sim_step();      // run one step
            SIMUL.endSingleRun();

            THEN( "the packet is received" ) {
                REQUIRE ( rec.getValue() == 1 );
            }
        }

        WHEN( "100 packets are sent and only one node is transmitting" ) {
            n1.addDestNode(n2);
            n1.setMessageToSend(100);

            runDebugSimul();

            THEN( "100 packets are received" ) {
                REQUIRE ( rec.getValue() == 100 );
            }
            THEN( "and no collision happened" ) {
                REQUIRE ( coll.getValue() == 0 );
            }
        }

        WHEN( "Two nodes tries sends 100 packets (each) at the same period (different seeds)" ) {
            n1.addDestNode(n2);
            n2.addDestNode(n1);
            n1.setInterval(std::unique_ptr<RandomVar>(new DeltaVar(10)));
            n2.setInterval(std::unique_ptr<RandomVar>(new DeltaVar(10)));

            runDebugSimul();

            THEN( "collisions are detected" ) {
                REQUIRE (coll.getValue() > 0);
            }
            THEN( "packets are received" ) {
                REQUIRE ( rec.getValue() > 0 );
            }
            THEN( "no packets are lost" ) {
                REQUIRE ( rec.getValue() == 200 );
            }
        }
    }
}


