#include <chrono>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

const uint16_t kPort = 3000;
const int kMaxBacklog = 128;
const char* kResponseBody = R"(HTTP/1.0 200 OK
Content-Type: text/plain

200)";
const size_t kResopnseSize = 45;

std::chrono::system_clock::time_point _StartTime;

#define LOG std::cout << std::setw(8) <<\
  std::chrono::duration_cast<std::chrono::microseconds>(\
  std::chrono::system_clock::now() - _StartTime).count() << " ";\
  std::cout

void StartTimer() {
  _StartTime = std::chrono::system_clock::now();
}

void PrintCpus() {
  LOG << "### CPUs: " << std::thread::hardware_concurrency() << std::endl;
}

int CreateSocket() {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  int flag = 1;
  setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&flag),
             sizeof(flag));
  LOG << "=== " << &sock << " socket0 created" << std::endl;
  return sock;
}

void BindSocket(int sock, struct sockaddr_in* addr) {
  addr->sin_family = AF_INET;
  addr->sin_port = htons(kPort);
  addr->sin_addr.s_addr = INADDR_ANY;
  bind(sock, reinterpret_cast<struct sockaddr*>(addr), sizeof(*addr));
  LOG << "=== " << &sock << " socket0 bound" << std::endl;
}

void ListenSocket(int sock) {
  listen(sock, kMaxBacklog);
  LOG << "=== " << &sock << " socket0 listened" << std::endl;
}

int AcceptSocket(int sock0, struct sockaddr* client, socklen_t* len) {
  int sock = accept(sock0, client, len);
  LOG << "--- " << &sock << " socket accepted" << std::endl;
  return sock;
}

void DoHeavyTask() {
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void WriteSock(int sock) {
  write(sock, kResponseBody, kResopnseSize);
  LOG << "--- " << &sock << " wrote" << std::endl;
}

void CloseSock(int sock) {
  close(sock);
  LOG << "--- " << &sock << " closed " << std::endl;
}
