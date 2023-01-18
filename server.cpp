#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

#include "server.h"
#include "clientnode.h"
#include <arpa/inet.h>

Server::Server(const struct in_addr &ip_addr, unsigned port)
{
    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = ip_addr;

    s = socket(PF_INET, SOCK_STREAM, 0);
    if (s < 0)
        throw "Server Error socket";
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0 ) {
        close(s);
        throw "Server Error bind";
    }
    if (fcntl(s, F_SETFL, fcntl(s, F_GETFL, 0) | O_NONBLOCK) < 0) {
        close(s);
        throw "Server Error fcntl";
    }
    if (listen(s, 5) < 0) {
        close(s);
        throw "Serevr Error listen";
    }

    io.set<Server, &Server::accept_cb>(this);
    io.start(s, ev::READ);

    sio.set<Server, &Server::signal_cb>(this);
    sio.start(SIGINT);
    printf("Serevr start and listening ip %s, port %d\n", inet_ntoa(ip_addr), port);
}

Server::~Server()
{
    io.stop();
    sio.stop();
    shutdown(s, SHUT_RDWR);
    close(s);
    printf("Serevr stop and remove\n");
}

void Server::accept_cb(ev::io &watcher, int revents)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sock;

    client_sock = accept(watcher.fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock >= 0)
        ClientNode::newClient(client_sock);

}

void Server::signal_cb(ev::sig &signal, int revents)
{
    signal.loop.break_loop();
}
