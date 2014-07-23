#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <memory>
#include <vector>
#include <string>

using namespace std;

#include <metasim.hpp>

#define _NODE_DBG "Node"

class Message;
class NetInterface;

class Node : public MetaSim::Entity {

  NetInterface* _net_interf;

  std::unique_ptr<MetaSim::RandomVar> _interval;

  std::vector<Node*> _nodes;

  int message_to_send;
  int sent_messages;


public:

  MetaSim::GEvent<Node> _recv_evt;
  MetaSim::GEvent<Node> _send_evt;

  Node(string const &name);

  NetInterface *getNetInterface();
  void setNetInterface(NetInterface &n);
  void addDestNode(Node &n);
  void setInterval(std::unique_ptr<MetaSim::RandomVar> i);

  void onMessageReceived(Message *m);
  void onReceive(MetaSim::Event *e);
  void onSend(MetaSim::Event *e);

  void setMessageToSend(unsigned int m){ message_to_send = m; }

  void newRun();
  void endRun();
};

#endif
