#include "server.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include <cstring>
#include <string>
Server::Server(std::string program_name, pid_t current_pid)
    : program_name_(program_name), current_pid_(current_pid) {}
void Server::Start() {
  int len;
  struct sockaddr_un local;
  const std::string sock_path =
      "/tmp/" + program_name_ + std::to_string(current_pid_);
  if ((listen_socket_ = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    std::cerr << "socket" << std::endl;
    return;
  }
  local.sun_family = AF_UNIX;
  std::strcpy(local.sun_path, sock_path.data());
  unlink(local.sun_path);
  len = strlen(local.sun_path) + sizeof(local.sun_family);
  if (bind(listen_socket_, (struct sockaddr*)&local, len) == -1) {
    std::cerr << "bind" << std::endl;
    return;
  }
  if (listen(listen_socket_, 32) == -1) {
    std::cerr << "listen" << std::endl;
    return;
  }
  auto server_thread_function = [](void* parameter) -> void* {
    int* s = (int*)parameter;
    int s2;
    unsigned t;
    char buf[1];
    struct sockaddr_un remote;
    for (;;) {
      t = sizeof(remote);
      if ((s2 = accept(*s, (struct sockaddr*)&remote, &t)) == -1) {
        std::cerr << "accept" << std::endl;
        return nullptr;
      }
      if (recv(s2, buf, sizeof(buf), 0) == 0) {
        // Клиент отсоединился.
      }
      close(s2);
    }
  };
  pthread_create(&server_thread_id_, nullptr, server_thread_function,
                 &listen_socket_);
}
void Server::Stop() {
  pthread_cancel(server_thread_id_);
  pthread_join(server_thread_id_, nullptr);
}