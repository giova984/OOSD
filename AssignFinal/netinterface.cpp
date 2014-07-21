#include <algorithm>

#include <metasim.hpp>

#include "link.hpp"
#include "message.hpp"
#include "netinterface.hpp"
#include "node.hpp"

using namespace std;
using namespace MetaSim;

NetInterface::NetInterface(const char *name, Node &n) :
        Entity(name), _node(&n)
{
        _node->setNetInterface(*this);
}

NetInterface::~NetInterface()
{
}

WifiInterface::WifiInterface(const char *name, Node &n, std::pair<double, double> pos2D, double radius, WifiLink &l) :
        NetInterface(name,n), position2D(pos2D), radius(radius),_link(&l), _queue(), _received(), _blocked(),
        _trans_evt()
{
        register_handler(_trans_evt, this, &WifiInterface::onTransmit);

        _cont_per = _link->getContentionPeriod();
        _backoff = _cont_per;
        _coll = 0;
}

WifiInterface::~WifiInterface()
{}

void WifiInterface::newRun()
{
        vector<Message *>::iterator i;

        _queue.clear();
        for (i = _received.begin(); i != _received.end(); ++i) delete (*i);

        _received.clear();
        _blocked.clear();

}
 
void WifiInterface::endRun()
{
}

bool WifiInterface::isNear(WifiInterface *i){
    if (sqrt(position2D.first*position2D.first + i->getPosition2D().second*i->getPosition2D().second) < (radius + i->getRadius()))
            return true;
    return false;
}

void WifiInterface::send(Message *m)
{
        DBGENTER(_WIFIINTER_DBG);

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

Tick WifiInterface::nextTransTime()
{
        DBGTAG(_WIFIINTER_DBG, getName() + "::nextTransTime()");

        _coll++;

        if (_coll <= 10) _backoff *= 2;

        UniformVar a(1, _backoff);

        return (Tick) a.get();
}

void WifiInterface::onMessageReceived(Message *m)
{
        DBGENTER(_WIFIINTER_DBG);

        vector<Node *>::iterator i = find(_blocked.begin(), _blocked.end(), m->getDestNode());

        if (i != _blocked.end()) {
                (*i)->onMessageReceived(m);
                _blocked.erase(i);
        }
        else 
                _received.push_back(m);

        
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

