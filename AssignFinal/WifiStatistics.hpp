#include <metasim.hpp>
using namespace MetaSim;

class LinkCollisionStat : public StatCount {
public:
        LinkCollisionStat(const char *name);
        void probe(Event *e);
        void attach(Entity *e);
};

class LinkHiddenTerminalStat : public StatCount {
public:
        LinkHiddenTerminalStat(const char *name);
        void probe(Event *e);
        void attach(Entity *e);
};

class LinkMessageLostStat : public StatCount {
public:
        LinkMessageLostStat(const char *name);
        void probe(Event *e);
        void attach(Entity *e);
};

class LinkMessageReceivedStat : public StatCount {
public:
        LinkMessageReceivedStat(const char *name);
        void probe(Event *e);
        void attach(Entity *e);
};

class NodeMessageReceivedStat : public StatCount {
public:
        NodeMessageReceivedStat(const char *name);
        void probe(Event *e);
        void attach(Entity *e);
};

