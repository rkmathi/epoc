#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#include "./common.h"

int main() {
  StartTimer();
  LOG << "### START epoll-svr" << std::endl;
  PrintCpus();

  int epfd = epoll_create(16);
  if (epfd < 0) {
    std::cerr << "epoll_create failed!" << std::endl;
    return 1;
  }

  int sock0 = CreateSocket(true);

  struct sockaddr_in addr{};
  BindSocket(sock0, &addr);

  ListenSocket(sock0);

  struct epoll_event ev{};
  memset(&ev, 0, sizeof(ev));
  ev.events = EPOLLIN;
  ev.data.fd = sock0;
  if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock0, &ev) != 0) {
    std::cerr << "epoll_ctL failed!" << std::endl;
    return 1;
  }

  struct epoll_event events[16];
  while (true) {
    int nfds = epoll_wait(epfd, events, 16, -1);
    if (nfds <= 0) {
      std::cerr << "epoll_wait failed!" << std::endl;
      return 1;
    }

    for (int i = 0; i < nfds; i++) {
      if (events[i].data.fd == sock0) {
        struct sockaddr client{};
        socklen_t len = sizeof(client);
        int sock = AcceptSocket(sock0, &client, &len);

        DoHeavyTask();
        WriteSockBlocking(sock);

        CloseSock(sock);
      }
    }
  }

  CloseSock(sock0);

  return 0;
}
