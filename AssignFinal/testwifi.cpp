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
        LinkMessageLostStat lost("");
        lost.attach(&link);

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


    GIVEN( "nodes are too far to communicate" ) {

        // (simulation objects creation and initialization)
        WifiLink link("2.412Ghz");
        Node n1("Node1");
        Node n2("Node2");
        WifiInterface i1("Int1", n1, std::make_pair(0,0), 1, link, 1);
        WifiInterface i2("Int2", n2, std::make_pair(0,2), 1, link, 2);

        n1.setInterval(std::unique_ptr<RandomVar>(new DeltaVar(10)));
        n2.setInterval(std::unique_ptr<RandomVar>(new DeltaVar(10)));

        NodeMessageReceivedStat rec("");
        rec.attach(&n1);
        rec.attach(&n2);
        LinkCollisionStat coll("");
        coll.attach(&link);
        LinkMessageLostStat lost("");
        lost.attach(&link);

        WHEN( "A packet is sent to a connected node" ) {
            n1.addDestNode(n2);
            n1.setMessageToSend(1);

            runDebugSimul();
//            while (!rec.getValue() == 1)
//                SIMUL.sim_step();      // run one step
            SIMUL.endSingleRun();

            THEN( "the packet is lost" ) {
                REQUIRE ( rec.getValue() == 0 );
            }
        }

        WHEN( "100 packets are sent to the reachable node" ) {
            n1.addDestNode(n2);
            n1.setMessageToSend(100);

            runDebugSimul();

            THEN( "100 packets are lost" ) {
                REQUIRE ( rec.getValue() == 0 );
            }
            THEN( "and no collision happened" ) {
                REQUIRE ( coll.getValue() == 0 );
            }
        }

        WHEN( "100 packets are sent by each node" ) {
            n1.addDestNode(n2);
            n2.addDestNode(n1);
            n1.setMessageToSend(100);
            n2.setMessageToSend(100);

            runDebugSimul();

            THEN( "all packets are lost" ) {
                REQUIRE ( rec.getValue() == 0 );
            }
            THEN( "and no collision happened" ) {
                REQUIRE ( coll.getValue() == 0 );
            }
        }
    }

    GIVEN( "Only one node can transmit to the other (more power)" ) {

        // (simulation objects creation and initialization)
        WifiLink link("2.412Ghz");
        Node n1("Node1");
        Node n2("Node2");
        WifiInterface i1("Int1", n1, std::make_pair(0,0), 2, link, 1);
        WifiInterface i2("Int2", n2, std::make_pair(0,2), 1, link, 2);

        n1.setInterval(std::unique_ptr<RandomVar>(new DeltaVar(5000)));
        n2.setInterval(std::unique_ptr<RandomVar>(new DeltaVar(10)));

        NodeMessageReceivedStat rec("");
        rec.attach(&n1);
        rec.attach(&n2);
        LinkCollisionStat coll("");
        coll.attach(&link);
        LinkMessageLostStat lost("");
        lost.attach(&link);

        WHEN( "100 packets are sent by each node (with a controlled temporization)" ) {
            n1.addDestNode(n2);
            n2.addDestNode(n1);
            n1.setMessageToSend(100);
            n2.setMessageToSend(100);
            n1.setInterval(std::unique_ptr<RandomVar>(new DeltaVar(5000)));
            n2.setInterval(std::unique_ptr<RandomVar>(new DeltaVar(10)));

            runDebugSimul();

            THEN( "some packets are received" ) {
                REQUIRE ( rec.getValue() > 0 );
            }
            THEN( "at least 100 packets are lost" ) {
                REQUIRE ( lost.getValue() >= 100 );
            }
        }
    }
}


SCENARIO( "Three wifi nodes sharing a single channel placed like a chain (first and last cannot directly reach each other)" ) {

    GIVEN( "A routing table is created" ) {

        // (simulation objects creation and initialization)
        WifiLink link("2.412Ghz");
        Node n1("Node1");
        Node n2("Node2");
        Node n3("Node3");
        WifiRoutingTable routing{};
        WifiInterface i1("Int1", n1, std::make_pair(0,0), 1, link, 1, &routing);
        WifiInterface i2("Int2", n2, std::make_pair(0,1), 1, link, 2, &routing);
        WifiInterface i3("Int3", n3, std::make_pair(0,2), 1, link, 3, &routing);

        n1.setInterval(std::unique_ptr<RandomVar>(new DeltaVar(100)));
        n2.setInterval(std::unique_ptr<RandomVar>(new DeltaVar(200)));
        n3.setInterval(std::unique_ptr<RandomVar>(new DeltaVar(300)));

        NodeMessageReceivedStat rec("");
        rec.attach(&n1);
        rec.attach(&n2);
        rec.attach(&n3);
        LinkCollisionStat coll("");
        coll.attach(&link);
        LinkMessageLostStat lost("");
        lost.attach(&link);
        LinkHiddenTerminalStat hidden("");
        hidden.attach(&link);

        WHEN( "1 sends to 3 100 packets" ) {
            n1.addDestNode(n3);
            n1.setMessageToSend(100);

            runDebugSimul();
            SIMUL.endSingleRun();

            THEN( "all packets are received" ) {
                REQUIRE ( rec.getValue() == 100 );
            }
        }

        WHEN( "1 send to 3 and viceversa simultaneously 100 packets each" ) {
            n1.addDestNode(n3);
            n3.addDestNode(n1);
            n1.setMessageToSend(100);
            n3.setMessageToSend(100);
            n1.setInterval(std::unique_ptr<RandomVar>(new DeltaVar(100)));
            n2.setInterval(std::unique_ptr<RandomVar>(new DeltaVar(100)));
            n3.setInterval(std::unique_ptr<RandomVar>(new DeltaVar(100)));

            runDebugSimul();

            THEN( "Hidden terminal problem detected" ) {
                REQUIRE ( hidden.getValue() > 0 );
            }
            THEN( "some packets are lost" ) {
                REQUIRE ( lost.getValue() > 0 );
            }
        }
    }
}
