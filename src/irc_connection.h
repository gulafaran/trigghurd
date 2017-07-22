/*
 * --------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tomenglund26@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Tom Englund
 * --------------------------------------------------------------------------------
 */

#include <stdbool.h>
#include <netinet/in.h>
#include "config.h"

struct irc_server_info {
  int port;
  char *adress;
  char *password;
  char *channels; // comma seperated.
  char *nick;
  bool ssl;
  bool connected;
};

struct irc_socket {
  int sockfd;
  struct hostent *server;
  struct sockaddr_in serv_addr;
  char buffer[BUFFER_SIZE];
  int bytes_read;
  char line[BUFFER_SIZE];
};

bool send_raw(struct irc_socket *sock, char *string);
int read_raw(struct irc_socket *sock);
void create_connection(struct irc_server_info *srv);
