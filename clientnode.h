#ifndef CLIENTNODE_H
#define CLIENTNODE_H

#include <ev++.h>
#include <map>
#include <memory>

class ClientNode {
public:
    static void newClient(int s);
    static void delClient(int s);
    ~ClientNode();

private:
    ev::io io;
    int sfd;
    static std::map <int, std::unique_ptr<ClientNode>> clients;

    ClientNode(int s);
    void read_cb(ev::io &watcher, int revents);
};

#endif // CLIENTNODE_H
