#ifndef __LINK_HPP__
#define __LINK_HPP__

#include <vector>

#include <entity.hpp>
#include <event.hpp>
#include <map>

#include "netinterface.hpp"

const char* const _WIFILINK_DBG = "WifiLink";

class Message;

class Link : public MetaSim::Entity {
public:
        Link(const char *name);
        virtual ~Link();

        virtual void send(Message *m) = 0;
};

class WifiLink : public Link {
        std::vector<WifiInterface *> _interfaces;
 //       std::vector<WifiInterface *> _contending;

        std::map<WifiInterface *, bool> _isBusy;
        std::map<WifiInterface *, bool> _isContending;
        std::map<WifiInterface *, bool> _isCollision;
        int _contention_period;

        std::map<WifiInterface *, Message *> _message;

        std::map<WifiInterface *, std::unique_ptr<MetaSim::Event>> _end_contention_evts;
        std::map<WifiInterface *, std::unique_ptr<MetaSim::Event>> _collision_evts;
        std::map<WifiInterface *, std::unique_ptr<MetaSim::Event>> _end_transmission_evts;
public:

        MetaSim::GEvent<WifiLink> _link_end_contention_evt;
        MetaSim::GEvent<WifiLink> _link_collision_evt;
        MetaSim::GEvent<WifiLink> _link_end_transmission_evt;
        MetaSim::GEvent<WifiLink> _link_dst_not_reachable_evt;
        MetaSim::GEvent<WifiLink> _link_hidden_terminal_evt;


        WifiLink(const char *name);
        virtual ~WifiLink();

        bool isBusy(WifiInterface *in);

        virtual void send(Message *m);

        void contend(WifiInterface *wifi, Message *m);
        void onEndContention(MetaSim::Event *e);
        void onCollision(MetaSim::Event *e);
        void onEndTransmission(MetaSim::Event *e);
        void onDstNotReachable(MetaSim::Event *e);
        void onHiddenTerminal(MetaSim::Event *e);

        void setContentionPeriod(int p) { _contention_period = p; }
        int getContentionPeriod() { return _contention_period; }

        void registerInterface(WifiInterface * interf);
        WifiInterface * getEventInterface(MetaSim::Event * e);

        void newRun(); 
        void endRun(); 
};

#endif
