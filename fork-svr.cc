#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include "./common.h"

int main() {
  StartTimer();
  LOG << "### START fork-svr" << std::endl;
  PrintCpus();

  int sock0 = CreateSocket(true);

  struct sockaddr_in addr{};
  BindSocket(sock0, &addr);

  ListenSocket(sock0);

  struct sockaddr client{};
  socklen_t len = sizeof(client);
  while (true) {
    int sock = AcceptSocket(sock0, &client, &len);

    int pid = fork();
    if (pid == 0) {
      // Child process
      int mypid = getpid();
      LOG << "--- pid: " << mypid << " START child" << std::endl;

      DoHeavyTask();
      WriteSockBlocking(sock);
      return 0;
    } else {
      // Parent process
      int mypid = getpid();
      LOG << "=== pid: " << mypid << " START parent" << std::endl;

      int cpid, status;
      while ((cpid = waitpid(-1, &status, WNOHANG)) > 0) {}
      LOG << "=== pid: " << mypid << ", status: " << status <<
          ", errno: " << errno << ", cpid: " << cpid << std::endl;

      CloseSock(sock);
    }
  }

  CloseSock(sock0);

  return 0;
}
