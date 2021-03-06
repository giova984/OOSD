#ifndef __NETINTERFACE_HPP__
#define __NETINTERFACE_HPP__

#include <deque>
#include <vector>
#include <utility>

#include <metasim.hpp>
//#include <entity.hpp>
//#include <event.hpp>
#include <randomvar.hpp>

#define _WIFIINTER_DBG "WifiInterface"

//class MetaSim;
class Node;
class Message;
class WifiLink;
class WifiRoutingTable;

#define RECV_BUFFER_SIZE 1000

class NetInterface : public MetaSim::Entity {
protected:
  Node *_node;
public:
  NetInterface(std::string const &name, Node &n);
  virtual ~NetInterface();
  
  virtual void send(Message *m) = 0;
  virtual Message * receive(Node *n) = 0;
  
  virtual void onMessageSent(Message *m) = 0;
//  virtual void onMessageLost();
  virtual void onMessageReceived(Message *m) = 0;
};

class WifiInterface : public NetInterface {
protected:
  WifiLink* _link;
  WifiRoutingTable *_routing;

  std::deque<Message*> _queue;
  std::vector<Message*> _received;
  std::vector<Node*> _blocked;

  std::pair<double, double> position2D;
  double radius;

  int _cont_per;
  int _backoff;
  int _coll;
  bool _isTransmitting;

  std::unique_ptr<MetaSim::RandomGen> _cont_rand_gen;

public:

  MetaSim::GEvent<WifiInterface> _trans_evt;

  WifiInterface(std::string const &name, Node& n, std::pair<double, double> pos2D, double radius, WifiLink& l, MetaSim::RandNum cp_seed = {0}, WifiRoutingTable* rt = nullptr);
  virtual ~WifiInterface();

  MetaSim::Tick nextTransTime();

  virtual void send(Message* m);
  virtual void onCollision();
  virtual void onTransmit(MetaSim::Event* e);
  virtual Message* receive(Node* n);
  virtual void onMessageSent(Message* m); 
//  virtual void onMessageLost(Message* m);
  virtual void onMessageReceived(Message* m);

  std::pair<double, double> getPosition2D(){ return position2D; }
  void setPosition2D(std::pair<double, double> pos ){ position2D = pos; }
  double getRadius(){ return radius; }
  void setRadius(double r){ radius = r; }
  bool canTransmitTo(WifiInterface *i);

  bool isTransmitting(){ return _isTransmitting; }

  void newRun();
  void endRun();
};

#endif
