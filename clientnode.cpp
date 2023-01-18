#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include "clientnode.h"

std::map <int, std::unique_ptr<ClientNode>> ClientNode::clients;

void ClientNode::newClient(int s)
{
    clients[s] = std::unique_ptr<ClientNode>(new ClientNode(s));
    printf("ClientNode connect, total clients connected %ld\n", clients.size());
}

void ClientNode::delClient(int s)
{
    auto it = clients.find(s);
    if (it != clients.end())
        clients.erase(it);
}

ClientNode::ClientNode(int s) : sfd(s)
{
    if (fcntl(s, F_SETFL, fcntl(s, F_GETFL, 0) | O_NONBLOCK) < 0)
        throw "Error fcntl ClientNode";
    io.set<ClientNode, &ClientNode::read_cb>(this);
    io.start(s, ev::READ);
}

void ClientNode::read_cb(ev::io &watcher, int revents)
{
    char  buffer[1024];
    ssize_t nread = recv(watcher.fd, buffer, sizeof(buffer), 0);

    if (nread < 0)
        printf("ClientNode Error recv < 0\n");
    else if (nread > 0) {
        std::string str(buffer, nread);

        printf("ClientNode recv[%ld]: %s\n", nread, str.c_str());

        nread = send(watcher.fd, buffer, nread, 0);
        if (nread > 0) {
            str.clear();
            str.insert(0, buffer, nread);
            printf("ClientNode send[%ld]: %s\n", nread, str.c_str());
        } else
            printf("ClientNode Error send < 0\n");
    }
    else
        delClient(this->sfd);
}

ClientNode::~ClientNode()
{
    io.stop();
    shutdown(sfd, SHUT_RDWR);
    close(sfd);
    printf("ClientNode disconnect, total clients connected %ld\n", clients.size() - 1);
}
