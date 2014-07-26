#include "routing.hpp"
#include "netinterface.hpp"
#include <algorithm>

RoutingTable::RoutingTable()
{

}

RoutingTable::~RoutingTable()
{

}

WifiRoutingTable::WifiRoutingTable() : RoutingTable()
{

}

WifiRoutingTable::~WifiRoutingTable()
{

}

void WifiRoutingTable::registerInterface(WifiInterface *interf)
{
    if(std::find(begin(_interfaces), end(_interfaces), interf) == _interfaces.end())
        _interfaces.push_back(interf);
}

double distance(const std::pair<double, double> &a, const std::pair<double, double> &b);
NetInterface* WifiRoutingTable::getNextHop(NetInterface *from, NetInterface *to)
{
    WifiInterface *_from = dynamic_cast<WifiInterface *>(from);
    WifiInterface *_to = dynamic_cast<WifiInterface *>(to);
    NetInterface *_next = nullptr;
    if ( _from != nullptr && _to != nullptr ){

        double min_dist = distance(_from->getPosition2D(), _to->getPosition2D());
        for (auto i : _interfaces){
            if(_from->canTransmitTo(i)){
                double dist = distance( i->getPosition2D(), _to->getPosition2D());
                if ( dist < min_dist ){
                    min_dist = dist;
                    _next = i;
                }
            }
        }
    }
    //std::cout << _next << std::endl;
    return _next;
}




