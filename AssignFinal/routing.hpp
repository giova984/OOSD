#ifndef ROUTING_HPP
#define ROUTING_HPP

#include <vector>

class NetInterface;
class WifiInterface;
class Message;

class RoutingTable {
public:
    RoutingTable();
    virtual ~RoutingTable();

    //virtual void calculateRoute(Message *m);
    virtual NetInterface* getNextHop(NetInterface *from, NetInterface *to) = 0;
};

class WifiRoutingTable : public RoutingTable
{
    std::vector<WifiInterface *> _interfaces;
public:
    WifiRoutingTable();
    virtual ~WifiRoutingTable();
    virtual void registerInterface(WifiInterface *interf);
    //virtual std::vector<WifiInterface *> getPath(NetInterface *src, NetInterface *dst);
    //virtual calculateRoute(Message *m);
    virtual NetInterface* getNextHop(NetInterface *from, NetInterface *to);
};

#endif // ROUTING_HPP
