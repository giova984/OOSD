#include <metasim.hpp>

#include "message.hpp"
#include "netinterface.hpp"
#include "node.hpp"

using namespace std;
using namespace MetaSim;

/*-----------------------------------------------------*/

Node::Node(string const & name) 
        : Entity(name), _net_interf(0), _interval(nullptr), messages_to_send(100), sent_messages(0),
          _nodes(), _recv_evt(), _send_evt()
{
        register_handler(_recv_evt, this, &Node::onReceive);
        register_handler(_send_evt, this, &Node::onSend);
}

void Node::newRun()
{
    sent_messages = 0;
    if (_interval != nullptr)
        //FIXED_send_evt.post((int)1000);
        _send_evt.post((int)_interval->get());
    //else node doesn't send messages
}

void Node::endRun()
{
}


NetInterface *Node::getNetInterface()
{
        return _net_interf;
}

void Node::setNetInterface(NetInterface &n)
{
        _net_interf = &n;
}

void Node::onReceive(Event *e)
{
        // simply, record the fact that the message has succesfully been 
        // received.
 
        DBGTAG(_NODE_DBG, getName() + "::onReceive()");
}

void Node::onMessageReceived(Message *m)
{
        // simply, record the fact that the message has succesfully been 
        // received.
 
        DBGTAG(_NODE_DBG, getName() + "::onMessageReceived()");

}

void Node::setInterval(std::unique_ptr<RandomVar> i)
{
        _interval = std::move(i);
}

void Node::addDestNode(Node& n)
{
        _nodes.push_back(&n);
}

void Node::onSend(Event *e)
{
    ++sent_messages;

    if (sent_messages > messages_to_send)
        return;

    if (_nodes.size() <= 0)
        return;

    RandomGen gen(12345);
    UniformVar len(100,1500,&gen );
    UniformVar n(0, _nodes.size(), &gen);
    int i = (int)n.get();

    DBGENTER(_NODE_DBG);

    DBGPRINT("dest node = " << _nodes[i]->getName());
    // creates a new message and send it!!
    Message *m = new Message((int)len.get(), this, _nodes[i]);
    //FIXEDMessage *m = new Message(1000, this, _nodes[0]);
    _net_interf->send(m);
    if (_interval != nullptr)
        _send_evt.post(SIMUL.getTime() + (Tick)_interval->get());
    //FIXED_send_evt.post(SIMUL.getTime() + (Tick)1000);

}

