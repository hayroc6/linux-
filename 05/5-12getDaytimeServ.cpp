#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <assert.h>

int main(int argc, char **argv) {
  assert(argc == 2);
  char *host = argv[1];
  // 获取目标主机信息
  struct hostent *hostinfo = gethostbyname(host);
  assert(hostinfo);
  // 获取daytime服务信息
  struct servent *servinfo = getservbyname("daytime", "tcp");
  assert(servinfo);
  std::cout << "daytime port is " << ntohs(servinfo->s_port) << std::endl;

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = servinfo->s_port;
  address.sin_addr = *(struct in_addr*)*hostinfo->h_addr_list;

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  int result = connect(sockfd, (struct sockaddr*)&address, sizeof(address));
  assert(result != -1);

  char buffer[128];
  result = read(sockfd, buffer, sizeof(buffer));
  assert(result > 0);
  buffer[result] = '\0';
  std::cout << "the day item is: " << buffer << std::endl;
  close(sockfd);
  return 0;
}
