#include <algorithm>
#include <simul.hpp>
#include <memory>
#include <QApplication>

#include "link.hpp"
#include "message.hpp"
#include "netinterface.hpp"
#include "node.hpp"
#include "routing.hpp"

#include "wifi.hpp"

#define __DEBUG__

using namespace std;
using namespace MetaSim;

const double UMIN = 0.5;
const double UMAX = 0.9;
const double USTEP = 0.1;

const unsigned AVG_LEN = 800;
const Tick SIM_LEN = (int) (AVG_LEN * 1000);

#include<sstream>
 template <typename T>
 std::string to_string(T value)
 {
   std::ostringstream os ;
   os << value ;
   return os.str() ;
 }

class LinkCollisionStat : public StatCount {
public:
        LinkCollisionStat(const char *name) : StatCount(name) {}

        void probe(Event *e) 
        {
                record(1);
        }
  
        void attach(Entity *e) 
        {
                WifiLink *l = dynamic_cast<WifiLink *>(e);
                if (l == nullptr)
                        throw BaseExc("Please, specify a Wifi Link!");
    
                l->_link_collision_evt.addStat(this);
        }
};

class LinkHiddenTerminalStat : public StatCount {
public:
        LinkHiddenTerminalStat(const char *name) : StatCount(name) {}

        void probe(Event *e)
        {
                record(1);
        }

        void attach(Entity *e)
        {
                WifiLink *l = dynamic_cast<WifiLink *>(e);
                if (l == nullptr)
                        throw BaseExc("Please, specify a Wifi Link!");

                l->_link_hidden_terminal_evt.addStat(this);
        }
};

class LinkMessageLostStat : public StatCount {
public:
        LinkMessageLostStat(const char *name) : StatCount(name) {}

        void probe(Event *e)
        {
                record(1);
        }

        void attach(Entity *e)
        {
                WifiLink *l = dynamic_cast<WifiLink *>(e);
                if (l == nullptr)
                        throw BaseExc("Please, specify a Wifi Link!");

                l->_link_dst_not_reachable_evt.addStat(this);
                l->_link_hidden_terminal_evt.addStat(this);
        }
};

class LinkMessageReceivedStat : public StatCount {
public:
        LinkMessageReceivedStat(const char *name) : StatCount(name) {}

        void probe(Event *e)
        {
                record(1);
        }

        void attach(Entity *e)
        {
                WifiLink *l = dynamic_cast<WifiLink *>(e);
                if (l == nullptr)
                        throw BaseExc("Please, specify a Wifi Link!");

                l->_link_msg_received_evt.addStat(this);
        }
};

class NodeMessageReceivedStat : public StatCount {
public:
        NodeMessageReceivedStat(const char *name) : StatCount(name) {}

        void probe(Event *e)
        {
                record(1);
        }

        void attach(Entity *e)
        {
                Node *n = dynamic_cast<Node *>(e);
                if (n == nullptr)
                        throw BaseExc("Please, specify a Node!");

                n->_recv_evt.addStat(this);
        }
};


int main_ex2(){
    std::vector<int> _m_sizes = {3, 4, 5};
    const int MAX_RAND = 100;

    for (auto _m : _m_sizes){ //running simulation on a different number of nodes

        WifiLink link{"Channel_1"};
        WifiRoutingTable routing_table;
        //std::unique_ptr<WifiRoutingTable> routing_table(new WifiRoutingTable());

        LinkCollisionStat c_stat("coll.txt");
        LinkHiddenTerminalStat ht_stat("hidden.txt");
        LinkMessageLostStat ml_stat("lost.txt");
        LinkMessageReceivedStat mr_stat("received.txt");
        NodeMessageReceivedStat nmr_stat("node_received.txt");

        c_stat.attach(&link);
        ht_stat.attach(&link);
        ml_stat.attach(&link);
        mr_stat.attach(&link);

        std::vector<std::vector<std::unique_ptr<Node>>> _nodes;
        _nodes.reserve(_m);

        std::vector<std::unique_ptr<WifiInterface>> _interfaces;
        _interfaces.reserve(_m*_m);

        for (int i=0; i<_m; ++i){ //creating nodes and WifiInterfaces
            _nodes.emplace_back(std::vector<std::unique_ptr<Node>> ());
            _nodes[i].reserve(_m);
            for (int j=0; j<_m; ++j){
                _nodes[i].emplace_back( new Node("Node_" + to_string(i) + "_" + to_string(j)) );
                _interfaces.emplace_back( new WifiInterface( "interface_" + to_string(i) + "_" + to_string(j),
                                                             *(_nodes[i].back()),
                                                             std::make_pair(i, j),
                                                             1,
                                                             link,
                                                             &routing_table
                                                             )
                                          );

                nmr_stat.attach(_nodes[i].back().get());
            }
        }

        for (int i=0; i<_m; ++i){ //adding destinations
            _nodes[i][0]->addDestNode(*_nodes[i][_m-1]);
            //_nodes[i][0]->addDestNode(*_nodes[i][1]);
            //_nodes[i][_m-1]->addDestNode(*_nodes[i][0]);
            _nodes[0][i]->addDestNode(*_nodes[_m-1][i]);
        }

        double u = 0.0;

        GnuPlotOutput output;
        output.init();

//        std::vector<std::unique_ptr<MetaSim::RandomGen>> _gens;
//        std::vector<std::unique_ptr<MetaSim::RandomVar>> _vars;
//        _gens.reserve(_m*_m);
//        _vars.reserve(_m*_m);
//        for (int i=0; i<_m; ++i){
//            for (int j=0; j<_m; ++j){
//                int seed = j + 100*i + 10000*_m;
//                _gens.emplace_back( std::unique_ptr<MetaSim::RandomGen>(new RandomGen(seed)) );
//                _vars.emplace_back( std::unique_ptr<RandomVar>( new UniformVar(1, l1, _gens.back().get()) );
//            }
//        }

        for (u=UMIN; u<= UMAX; u+=USTEP) {
            double l1 = MAX_RAND / u;
            double l2 = l1;
            double l3 = l1;

            std::vector<std::unique_ptr<MetaSim::RandomGen>> _gens;
            std::vector<std::unique_ptr<MetaSim::RandomVar>> _vars;
            _gens.reserve(_m*_m);
            _vars.reserve(_m*_m);
            for (int i=0; i<_m; ++i){
                for (int j=0; j<_m; ++j){
                    int seed = j + 100*i + 10000*_m;
                    _gens.emplace_back( std::unique_ptr<MetaSim::RandomGen>(new RandomGen(seed)) );
                    _vars.emplace_back( std::unique_ptr<RandomVar>( new UniformVar(1, l1, _gens.back().get()) ) );
                }
            }

            for (int i=0; i<_m; ++i){
                for (int j=0; j<_m; ++j){
                    int seed = j + 100*i + 10000*_m;
                    _nodes[i][j]->setInterval(std::move(_vars[i*_m + j]));
                }
            }

            SIMUL.dbg.setStream("log.txt");
            SIMUL.dbg.enable(_WIFILINK_DBG);
            SIMUL.dbg.enable(_WIFIINTER_DBG);
            SIMUL.dbg.enable(_NODE_DBG);

            try {
                cout << "U = " << u << " M = " << _m << endl;
                SIMUL.run(SIM_LEN, 5);
                output.write(u);
            } catch (BaseExc &e) {
                cout << e.what() << endl;
            }
        }
    }
}

int main_simple()
{
        double u = 0.0;

        Node n1("Node_1");
        Node n2("Node_2");
        Node n3("Node_3");

        n1.addDestNode(n2);
    //    n1.addDestNode(n3);
    //    n2.addDestNode(n1);
    //    n3.addDestNode(n1);
            n3.addDestNode(n2);

        WifiLink link("Channel_1");

        WifiInterface int1("Interface_1", n1, {0, 0}, 1, link);
        WifiInterface int2("Interface_2", n2, {1, 0}, 1, link);
        WifiInterface int3("Interface_3", n3, {2, 0}, 1, link);

        LinkCollisionStat stat("coll.txt");
        stat.attach(&link);
  
        GnuPlotOutput output;
        output.init();

        for (u=UMIN; u<= UMAX; u+=USTEP) {
    
                double l1 = 6 * AVG_LEN / u;
                double l2 = l1;
                double l3 = l1;

                n1.setInterval(std::unique_ptr<RandomVar>(new UniformVar(1,l1)));
                n2.setInterval(std::unique_ptr<RandomVar>(new UniformVar(1,l2)));
                n3.setInterval(std::unique_ptr<RandomVar>(new UniformVar(1,l3)));
    
                SIMUL.dbg.setStream("log.txt");
                SIMUL.dbg.enable(_WIFILINK_DBG);
                SIMUL.dbg.enable(_WIFIINTER_DBG);
                SIMUL.dbg.enable(_NODE_DBG);
 
                try {
                        cout << "U = " << u << endl;
                        SIMUL.run(SIM_LEN, 5);
                        output.write(u);
                } catch (BaseExc &e) {
                        cout << e.what() << endl;
                }
        }
}

int main_ui(int argc, char** argv){
    QApplication app(argc, argv);
     app.setApplicationName("Wifi Simulator");

     wifi mainWin;
     mainWin.setWindowTitle("Simple Wifi Simulator");
     mainWin.show();/*
    QLabel label("Hello Qt!");
    label.show();*/
    return app.exec();
    //ui->setupUi(this);
}


int main(int argc, char** argv)
{
    return main_ui(argc, argv);
    //return main_simple();
    //return main_ex2();
}
