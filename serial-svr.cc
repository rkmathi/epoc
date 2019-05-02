#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

#include "./common.h"

int main() {
  StartTimer();
  LOG << "### START serial-svr" << std::endl;
  PrintCpus();

  int sock0 = CreateSocket();

  struct sockaddr_in addr{};
  BindSocket(sock0, &addr);

  ListenSocket(sock0);

  struct sockaddr client{};
  socklen_t len = sizeof(client);
  while (true) {
    int sock = AcceptSocket(sock0, &client, &len);

    DoHeavyTask();
    WriteSock(sock);

    CloseSock(sock);
  }

  CloseSock(sock0);

  return 0;
}
