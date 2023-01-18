#ifndef SERVER_H
#define SERVER_H

#include "ev++.h"
#include <netinet/in.h>

class Server {
private:
  int s;
  ev::io  io;
  ev::sig sio;

public:
  Server(const struct in_addr &ip_addr, unsigned port);
  ~Server();

private:
  void accept_cb(ev::io &watcher, int revents);
  void signal_cb(ev::sig &signal, int revents);
};
#endif // SERVER_H
