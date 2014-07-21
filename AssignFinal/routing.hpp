#ifndef ROUTING_HPP
#define ROUTING_HPP

#include <vector>

#include "netinterface.hpp"

class RoutingTable {
    std::vector<NetInterface *> _interfaces;

public:
    NetInterface* getNextHop(NetInterface *src, NetInterface *dst);
};

#endif // ROUTING_HPP
