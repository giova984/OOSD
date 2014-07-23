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
        : Link(name), _interfaces(),
          //_contending(),
          //_isBusy(false),
          //_isContending(false),
          //_isCollision(false),
          _contention_period(10),
  //        _message(0),
          _link_end_contention_evt(),
          _link_collision_evt(),
          _link_end_transmission_evt(),
          _link_hidden_terminal_evt(),
          _link_msg_received_evt()
{
        register_handler(_link_end_contention_evt, this, nullptr);
        register_handler(_link_collision_evt, this, nullptr);
        register_handler(_link_end_transmission_evt, this, nullptr);
        register_handler(_link_dst_not_reachable_evt, this, &WifiLink::onDstNotReachable);
        register_handler(_link_hidden_terminal_evt, this, &WifiLink::onHiddenTerminal);
        register_handler(_link_msg_received_evt, this, nullptr);
}

WifiLink::~WifiLink()
{
}

void WifiLink::newRun()
{
        //_contending.clear();
        //_isBusy = false;
        //_isContending = false;
        //_isCollision = false;
        //_message = 0;
    for (auto& i : _isBusy)
        _isBusy[i.first] = false;
    for (auto& i : _isContending)
        _isContending[i.first] = false;
    for (auto& i : _isCollision)
        _isCollision[i.first] = false;
    for (auto& i : _message)
        _message[i.first] = 0;
}
 
void WifiLink::endRun()
{
}

void WifiLink::registerInterface(WifiInterface * interf){
    auto e = new MetaSim::GEvent<WifiLink>();
    register_handler(*e, this, &WifiLink::onEndContention);
    _end_contention_evts[interf] = unique_ptr<MetaSim::Event>(e);
    e = new MetaSim::GEvent<WifiLink>();
    register_handler(*e, this, &WifiLink::onCollision);
    _collision_evts[interf] = unique_ptr<MetaSim::Event>(e);
    e = new MetaSim::GEvent<WifiLink>();
    register_handler(*e, this, &WifiLink::onEndTransmission);
    _end_transmission_evts[interf] = unique_ptr<MetaSim::Event>(e);
    _isBusy[interf] = false;
    _isContending[interf] = false;
    _isCollision[interf] = false;
    _message[interf] = 0;
}

bool WifiLink::isBusy(WifiInterface *in)
{
    //return _isBusy;
    for (auto& t : _isBusy){
        if (t.second && in->interfereWith(t.first))
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

        bool _isContendingNear = false;
        for (auto& i : _isContending){
            if (i.second && wifi->interfereWith(i.first))
                _isContendingNear = true;
        }

        if (_isContendingNear) {
                //dropping near events
                for (auto& i : _isContending)
                    if(i.second && wifi->interfereWith(i.first))
                        _end_contention_evts[i.first]->drop();
                //_end_contention_evt.drop();

                bool _isCollisionNear = false;
                for (auto& i : _isCollision)
                    if(i.second && wifi->interfereWith(i.first))
                        _isCollisionNear = true;
                if (!_isCollisionNear) {
                        _isCollision[wifi] = true;
                        _collision_evts[wifi]->post(SIMUL.getTime() + 3);
                }
        }
        else {
                //_isContending[wifi] = true;
                _message[wifi] = m;
                _end_contention_evts[wifi]->post(SIMUL.getTime() + _contention_period);
        }
        _isContending[wifi] = true;
//        _contending.push_back(wifi);


}

WifiInterface * WifiLink::getEventInterface(MetaSim::Event * e){
    for (auto& i : _end_contention_evts)
        if (e == i.second.get())
            return i.first;
    for (auto& i : _collision_evts)
        if (e == i.second.get())
            return i.first;
    for (auto& i : _end_transmission_evts)
        if (e == i.second.get())
            return i.first;
    return nullptr;
}

void WifiLink::onEndContention(Event *e)
{
        DBGENTER(_WIFILINK_DBG);

        _link_end_contention_evt.process(); //used to update statistics

        auto wifi = getEventInterface(e);
        _isContending[wifi] = false;
        _isBusy[wifi] = true;
        _end_transmission_evts[wifi]->post(SIMUL.getTime() + _message[wifi]->getLength());

        //_contending.clear();
}

void WifiLink::onHiddenTerminal(Event *e){
    //TODO
    //std::cout << "HT detected" << std::endl;
}

void WifiLink::onCollision(Event *e)
{
    DBGENTER(_WIFILINK_DBG);

    _link_collision_evt.process(); //used to update statistics

    auto wifi = getEventInterface(e);
    for (auto& i : _isContending)
        if (i.second && wifi->interfereWith(i.first)){
            _isContending[i.first] = false;
            _isCollision[i.first] = false;
            _isBusy[i.first] = false;
            _message[i.first] = 0;
            i.first->onCollision();
        }
}

void WifiLink::onEndTransmission(Event *e)
{
    _link_end_transmission_evt.process(); //used to update statistics

    auto wifi = getEventInterface(e);
    Message *m = _message[wifi];

    DBGENTER(_WIFILINK_DBG);

    NetInterface *dst = m->getDestInterface();
    NetInterface *src = m->getSourceInterface();

    _isBusy[wifi] = false;
    _message[wifi] = 0;

    WifiInterface *d = dynamic_cast<WifiInterface *>(dst);
    //WifiInterface *s = dynamic_cast<WifiInterface *>(src);

    if( d != NULL && wifi->interfereWith(d) ){
        // detecting if hidden terminal event occurred
        bool ht_detected = false;
        for (auto& i : _isContending){
            //TODO decidere se anche la contesa interferisce con la trasmissione nel rilevamento dell'ht problem
            if (i.second && i.first->interfereWith(d))
                ht_detected = true;
        }
        for (auto& i : _isBusy){
            if (i.second && i.first->interfereWith(d))
                ht_detected = true;
        }
        if (!ht_detected){
            dst->onMessageReceived(m);
            //src->onMessageSent(m);
            //std::cout << "HT NOT detected" << std::endl;
        }else{
            _link_hidden_terminal_evt.process();
            //src->onMessageLost(m);
        }
    }else{
        _link_dst_not_reachable_evt.process();
        //src->onMessageLost(m);
    }
    src->onMessageSent(m);

}

void WifiLink::onDstNotReachable(Event *e){
    //std::cout << "dst not reachable" << std::endl;
}
