#include "wifistatistics.hpp"
#include <simul.hpp>
#include "link.hpp"
#include "node.hpp"

using namespace MetaSim;

LinkCollisionStat::LinkCollisionStat(const char *name) : StatCount(name) {}

void LinkCollisionStat::probe(Event *e){ record(1); }

void LinkCollisionStat::attach(Entity *e){
    WifiLink *l = dynamic_cast<WifiLink *>(e);
    if (l == nullptr)
        throw BaseExc("Please, specify a Wifi Link!");
    l->_link_collision_evt.addStat(this);
}

LinkHiddenTerminalStat::LinkHiddenTerminalStat(const char *name) : StatCount(name) {}

void LinkHiddenTerminalStat::probe(Event *e){ record(1); }

void LinkHiddenTerminalStat::attach(Entity *e){
    WifiLink *l = dynamic_cast<WifiLink *>(e);
    if (l == nullptr)
        throw BaseExc("Please, specify a Wifi Link!");
    l->_link_hidden_terminal_evt.addStat(this);
}

LinkMessageLostStat::LinkMessageLostStat(const char *name) : StatCount(name) {}

void LinkMessageLostStat::probe(Event *e){ record(1); }

void LinkMessageLostStat::attach(Entity *e){
    WifiLink *l = dynamic_cast<WifiLink *>(e);
    if (l == nullptr)
        throw BaseExc("Please, specify a Wifi Link!");
    l->_link_dst_not_reachable_evt.addStat(this);
    l->_link_hidden_terminal_evt.addStat(this);
}

LinkMessageReceivedStat::LinkMessageReceivedStat(const char *name) : StatCount(name) {}

void LinkMessageReceivedStat::probe(Event *e){ record(1); }

void LinkMessageReceivedStat::attach(Entity *e){
    WifiLink *l = dynamic_cast<WifiLink *>(e);
    if (l == nullptr)
        throw BaseExc("Please, specify a Wifi Link!");
    l->_link_msg_received_evt.addStat(this);
}

NodeMessageReceivedStat::NodeMessageReceivedStat(const char *name) : StatCount(name) {}

void NodeMessageReceivedStat::probe(Event *e){ record(1); }

void NodeMessageReceivedStat::attach(Entity *e){
    Node *n = dynamic_cast<Node *>(e);
    if (n == nullptr)
        throw BaseExc("Please, specify a Node!");
    n->_recv_evt.addStat(this);
}


