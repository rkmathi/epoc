#include <cerrno>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <thread>

constexpr uint16_t kPort = 3000;
constexpr int kMaxBacklog = 128;
const char* kResponseBody = R"(HTTP/1.0 200 OK
Content-Type: text/plain

200)";
const size_t kResopnseSize = 45;

int main() {
  std::cout << "### START fork-svr" << std::endl;
  std::cout << "### CPUs: " << std::thread::hardware_concurrency() << std::endl;

  int sock0 = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(kPort);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sock0, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));

  listen(sock0, kMaxBacklog);
  std::cout << "=== listening..." << std::endl;

  struct sockaddr client{};
  socklen_t len = sizeof(client);
  while (true) {
    int sock = accept(sock0, reinterpret_cast<struct sockaddr*>(&client), &len);
    std::cout << "=== accepted: " << &sock << std::endl;

    int pid = fork();
    if (pid == 0) {
      // Child process
      int mypid = getpid();
      std::cout << "--- pid: " << mypid << ", START child" << std::endl;

      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      write(sock, kResponseBody, kResopnseSize);
      std::cout << "--- pid: " << mypid << ", wrote" << std::endl;

      return 0;
    } else {
      // Parent process
      int mypid = getpid();
      std::cout << "=== pid: " << mypid << ", START parent" << std::endl;

      int cpid, status;
      while ((cpid = waitpid(-1, &status, WNOHANG)) > 0) {}
      std::cout << "=== pid: " << mypid << ", status: " << status <<
                ", errno: " << errno << std::endl;

      close(sock);
      std::cout << "=== pid: " << mypid << ", closed" << std::endl;
    }
  }

  close(sock0);

  return 0;
}
