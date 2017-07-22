/*
 * --------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tomenglund26@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Tom Englund
 * --------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "irc_connection.h"
#include "irc_messages.h"
#include "config.h"
#include "utils.h"

bool send_raw(struct irc_socket *sock, char *string) {
  bool rval = true;
  int len = strlen(string) + 1;
  char buffer[len]; //space for \n
  memset(buffer, 0, sizeof(buffer));
  str_add_to((char *)&buffer, string, 0);
  buffer[len - 1] = '\n';
  buffer[len] = '\0';

  if(write(sock->sockfd, buffer, len) < 0) {
    fprintf(stderr, "ERROR writing to socket");
    rval = false;
  }
  if(PRINT_TO_STDOUT) {
    fprintf(stdout,"<-- %s", buffer);
  }
  return rval;
}

void create_connection(struct irc_server_info *srv) {
  int o = -1;
  struct irc_msg msg;
  struct irc_socket sock;

  memset(sock.buffer, 0, sizeof(sock.buffer));
  memset(sock.line, 0, sizeof(sock.line));
  sock.sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if(sock.sockfd < 0) {
    fprintf(stderr,"ERROR opening socket\n");
    exit(EXIT_FAILURE);
  }

  sock.server = gethostbyname(srv->adress);
  if(sock.server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(EXIT_FAILURE);
  }

  memset((char *) &sock.serv_addr, 0, sizeof(sock.serv_addr));
  sock.serv_addr.sin_family = AF_INET;
  memcpy((char *)&sock.serv_addr.sin_addr.s_addr, (char *)sock.server->h_addr, sock.server->h_length);
  sock.serv_addr.sin_port = htons(srv->port);

  if (connect(sock.sockfd,(struct sockaddr *) &sock.serv_addr,sizeof(sock.serv_addr)) < 0) {
    fprintf(stderr, "ERROR Connecting\n");
    exit(EXIT_FAILURE);
  }

  srv->connected = true;
  send_ident(&sock, srv);

  memset(msg.raw, 0, sizeof(msg.raw));

  while(srv->connected) {
    sock.bytes_read = read(sock.sockfd, sock.buffer, BUFFER_SIZE);

    if(sock.bytes_read <= 0) {
      srv->connected = false;
    }
    else {
      for(int i = 0; i < sock.bytes_read; i++) {
        o++;
        sock.line[o] = sock.buffer[i];

        if((i > 0 && sock.buffer[i] == '\n' && sock.buffer[i - 1] == '\r') || (o + 1) == MAX_LINE_SIZE) {
          sock.line[o] = '\n';
          sock.line[o + 1] = '\0';
          o = -1;

          create_msg(&sock, &msg);

          if(PRINT_TO_STDOUT == true) {
            fprintf(stdout, "--> %s", msg.raw);
          }

          handle_msg(&sock, srv, &msg);
          memset(sock.line, 0, sizeof(sock.line));
          reset_msg(&msg);
        }
      }
    }
  }
  close(sock.sockfd);
}

