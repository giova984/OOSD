#include <algorithm>

//#include <metasim.hpp>

#include "link.hpp"
#include "message.hpp"
#include "netinterface.hpp"
#include "node.hpp"
#include "routing.hpp"

using namespace std;
using namespace MetaSim;

NetInterface::NetInterface(string const &name, Node &n) :
        Entity(name), _node(&n)
{
        _node->setNetInterface(*this);
}

NetInterface::~NetInterface()
{
}

WifiInterface::WifiInterface(string const &name, Node &n, std::pair<double, double> pos2D, double radius, WifiLink &l, MetaSim::RandNum cp_seed, WifiRoutingTable *rt) :
        NetInterface(name,n), position2D(pos2D), radius(radius),_link(&l), _cont_rand_gen(new RandomGen(cp_seed)), _routing(rt),
        _queue(), _received(), _blocked(),
        _trans_evt()
{
        register_handler(_trans_evt, this, &WifiInterface::onTransmit);

        _link->registerInterface(this);

        if(_routing)
            _routing->registerInterface(this);

        _cont_per = _link->getContentionPeriod();
        _backoff = _cont_per;
        _coll = 0;

        //_cont_rand_gen = std::unique_ptr<RandomGen>();
}

WifiInterface::~WifiInterface()
{
    for (auto m : _queue)
        delete m;
    for (auto m : _received)
        delete m;
}

void WifiInterface::newRun()
{
        vector<Message *>::iterator i;

        for (auto m : _queue)
            delete m;
        for (auto m : _received)
            delete m;
        _queue.clear();
        _received.clear();
        _blocked.clear();

        //std::cout << "seed " << _cont_rand_gen->getCurrSeed() << std::endl;
        _cont_rand_gen = std::unique_ptr<RandomGen>(new RandomGen(_cont_rand_gen->getCurrSeed()));

}
 
void WifiInterface::endRun()
{
}

/*
 * True if "this" can interfere with "i" (not viceversa if i has a lower transmission radius)
 */
double distance(const std::pair<double, double> &a, const std::pair<double, double> &b){
    double x = a.first - b.first;
    double y = a.second - b.second;
    return sqrt( x*x + y*y );
}

bool WifiInterface::canTransmitTo(WifiInterface *i){
//    if (distance(position2D, i->getPosition2D()) <= max(radius, i->getRadius()))
//            return true;
    if (distance(position2D, i->getPosition2D()) <= radius)
        return true;
    return false;
}

void WifiInterface::send(Message *m)
{
        DBGENTER(_WIFIINTER_DBG);

        //Inserting Level 2 Routing
        m->setSourceInterface(this);
        if (_routing == nullptr)
            m->setDestInterface(m->getDestNode()->getNetInterface());
        else
            m->setDestInterface(_routing->getNextHop(this, m->getDestNode()->getNetInterface()));

        _queue.push_back(m);
  
        if (_queue.size() == 1) _trans_evt.process();
        else 
                DBGPRINT("Message enqueued");

        
}

void WifiInterface::onTransmit(Event *e)
{
        DBGENTER(_WIFIINTER_DBG);

        if (_link->isBusy(this)) onCollision();
        else _link->contend(this, _queue.front());

        
}

void WifiInterface::onCollision()
{
        DBGENTER(_WIFIINTER_DBG);

        _trans_evt.post(SIMUL.getTime() + nextTransTime());

        
}

void WifiInterface::onMessageSent(Message *m)
{
        DBGENTER(_WIFIINTER_DBG);

        _queue.pop_front();

        _coll = 0;
        _backoff = _cont_per;

        if (!_queue.empty()) _trans_evt.process();
        
}

//void WifiInterface::onMessageLost(Message *m)
//{
//        DBGENTER(_WIFIINTER_DBG);

//        //destination is not reachable, the message is discarded (as if it was sent
//        onMessageSent(m);

//}

Tick WifiInterface::nextTransTime()
{
        DBGTAG(_WIFIINTER_DBG, getName() + "::nextTransTime()");

        _coll++;

        if (_coll <= 10) _backoff *= 2;

    //FIXED
        //return (Tick) (double)_cont_rand_gen->getCurrSeed();

        UniformVar a(1, _backoff, _cont_rand_gen.get());
        return (Tick) (int)a.get();

}

void WifiInterface::onMessageReceived(Message *m)
{
        DBGENTER(_WIFIINTER_DBG);

        _link->_link_msg_received_evt.process();

        // handling Routing Level 2
        if (m->getDestNode()->getNetInterface() == this){ //if interface is on the final destination node
            vector<Node *>::iterator i = find(_blocked.begin(), _blocked.end(), m->getDestNode());

            if (i != _blocked.end()) {
                    (*i)->onMessageReceived(m);
                    _blocked.erase(i);
            }
            else{
                if( _received.size() < RECV_BUFFER_SIZE )
                    _received.push_back(m);
                else
                    delete m;
            }
            _node->_recv_evt.process();
        }
        else // else forward packet to next hop
        {
            send(m);
        }

}

Message * WifiInterface::receive(Node *n)
{
        DBGTAG(_WIFIINTER_DBG, getName() + "::receive()");

        vector<Message *>::iterator i = _received.begin();
        Message *m = NULL;

        while (i != _received.end()) {
                if ((*i)->getDestNode() == n) {
                        m = *i;
                        _received.erase(i);
                        return m;
                }
                else ++i;
        }
        _blocked.push_back(n);
        return m;
}

