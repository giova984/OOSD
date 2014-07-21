#ifndef __LINK_HPP__
#define __LINK_HPP__

#include <vector>

#include <entity.hpp>
#include <event.hpp>

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
        std::vector<WifiInterface *> _contending;
        std::vector<WifiInterface *> _transmitting;

        bool _isContending;
        bool _isCollision;
        int _contention_period;

        Message *_message;

public:

        MetaSim::GEvent<WifiLink> _end_contention_evt;
        MetaSim::GEvent<WifiLink> _collision_evt;
        MetaSim::GEvent<WifiLink> _end_transmission_evt;

        std::vector<MetaSim::GEvent<WifiLink>> _end_contention_evts;
        std::vector<MetaSim::GEvent<WifiLink>> _collision_evts;
        std::vector<MetaSim::GEvent<WifiLink>> _end_transmission_evts;

        WifiLink(const char *name);
        virtual ~WifiLink();

        bool isBusy(WifiInterface *in);

        virtual void send(Message *m);

        void contend(WifiInterface *wifi, Message *m);
        void onEndContention(MetaSim::Event *e);
        void onCollision(MetaSim::Event *e);
        void onEndTransmission(MetaSim::Event *e);

        void setContentionPeriod(int p) { _contention_period = p; }
        int getContentionPeriod() { return _contention_period; }

        void newRun(); 
        void endRun(); 
};

#endif
