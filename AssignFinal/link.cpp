#include <metasim.hpp>

#include "link.hpp"
#include "message.hpp"

#include <functional>
using namespace std;
using namespace MetaSim;

Link::Link(const char *name) : Entity(name)
{
}

Link::~Link()
{
}

WifiLink::WifiLink(const char *name)
        : Link(name), _interfaces(), _contending(), _transmitting(),
          _isContending(false),
          _isCollision(false),
          _contention_period(10),
          _message(0),
          _end_contention_evt(),
          _collision_evt(),
          _end_transmission_evt()
{
        register_handler(_end_contention_evt, this, &WifiLink::onEndContention);
        register_handler(_collision_evt, this, &WifiLink::onCollision);
        register_handler(_end_transmission_evt, this, &WifiLink::onEndTransmission);
}

WifiLink::~WifiLink()
{
}

void WifiLink::newRun()
{
        _contending.clear();
        _transmitting.clear();
        _isContending = false;
        _isCollision = false;
        _message = 0;
}
 
void WifiLink::endRun()
{
}

bool WifiLink::isBusy(WifiInterface *in)
{
    for (auto t : _transmitting){
        if (t!= in && t->isTransmitting() && in->isNear(t))
            return true;
    }
    return false;
}

void WifiLink::send(Message *m)
{
}

void WifiLink::contend(WifiInterface *wifi, Message *m)
{
        DBGENTER(_WIFILINK_DBG);
 
        if (_isContending) {
                _end_contention_evt.drop();
                if (!_isCollision) {
                        _isCollision = true;
                        _collision_evt.post(SIMUL.getTime() + 3);
                }
        }
        else {
                _isContending = true;
                _message = m;

                MetaSim::GEvent<WifiLink> actual_end_contention_evt();
                register_handler(actual_end_contention_evt, this, std::bind1st<void(Event*)> (WifiLink::onEndContention));
                _end_contention_evt.post(SIMUL.getTime() + _contention_period);
        }
        _contending.push_back(wifi);

        
}

void WifiLink::onEndContention(Event *e)
{
        DBGENTER(_WIFILINK_DBG);

        _isContending = false;
        _end_transmission_evt.post(SIMUL.getTime() + _message->getLength());

        _contending.clear();

        
}

void WifiLink::onCollision(Event *e)
{
        DBGENTER(_WIFILINK_DBG);

        _isContending = false;
        _isCollision = false;

        while (!_contending.empty()) {
                _contending.back()->onCollision();
                _contending.pop_back();
        }
        _message = 0;

}

void WifiLink::onEndTransmission(Event *e)
{
        Message *m = _message;

        DBGENTER(_WIFILINK_DBG);

        NetInterface *dst = _message->getDestInterface();
        NetInterface *src = _message->getSourceInterface();

        _message = 0;

        dst->onMessageReceived(m);
        src->onMessageSent(m);

        
}
