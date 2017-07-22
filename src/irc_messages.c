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
#include "irc_connection.h"
#include "irc_messages.h"
#include "utils.h"

void create_msg(struct irc_socket *sock, struct irc_msg *msg) {
  int splits;
  char **strings;

  msg->type = NONE;
  str_add_to((char *)&msg->raw, sock->line, 0);
  msg->msg = NULL;
  msg->nick_sent = NULL;
  msg->channel = NULL;

  strings = str_split(msg->raw, ":", 3, &splits);

  if(splits >= 2) {
    if(strcmp(strings[0], "PONG ") == 0) {
      msg->type = PONG;
      msg->msg = strdup(strings[1]);
    }
    else if(strcmp(strings[0], "PING ") == 0) {
      msg->type = PING;
      msg->msg = strdup(strings[1]);
    }
    else {
      int rpl_splits = 0;
      char **rpl_str = str_split(strings[0], " ", 3, &rpl_splits);
      for(int b = 0; b < rpl_splits; b++) {
        if(rpl_str[b] != NULL && b == 1) {
          if(strcmp(rpl_str[b], "376") == 0) {
            msg->type = RPL_ENDOFMOTD;
          }
          else if(strcmp(rpl_str[b], "PRIVMSG") == 0) {
            msg->type = PRIVMSG;
            msg->msg = strdup(strings[splits - 1]);

            if(str_startswith(rpl_str[rpl_splits - 1], "#") == true) {
              msg->channel = strdup(rpl_str[rpl_splits - 1]);
            }

            int h_len = strlen(rpl_str[0]);
            char nickbuf[strlen(rpl_str[0])];
            char *hostmask = rpl_str[0];

            for(int i = 0; i < h_len; i++) {
              if(hostmask[i] == '!') {
                nickbuf[i] = '\0';
                msg->nick_sent = strdup(nickbuf);
                break;
              }
              nickbuf[i] = hostmask[i];
            }
          }
        }
      }

      for(int i = 0; i < rpl_splits; i++) {
        safefree((void **)&rpl_str[i]);
      }
      safefree((void **)&rpl_str);
    }
  }

  if(splits > 0) {
    for(int i = 0; i < splits; i++) {
      safefree((void **)&strings[i]);
    }
  }
  safefree((void **)&strings);
}

void handle_msg(struct irc_socket *sock, struct irc_server_info *srv, struct irc_msg *msg) {
  char tmp[MAX_LINE_SIZE];
  memset(tmp, 0, sizeof(tmp));

  switch(msg->type) {
    case NONE:
    break;
    case PRIVMSG:
    if(msg->channel != NULL) {
      if(str_startswith(msg->msg, ",hi")) {
        str_add_to((char *)&tmp, "PRIVMSG ", 0);
        str_add_to((char *)&tmp, msg->channel, strlen(tmp));
        str_add_to((char *)&tmp, " :hi ", strlen(tmp));
        str_add_to((char *)&tmp, msg->nick_sent, strlen(tmp));
      }
    }
    break;
    case PONG:
      str_add_to((char *)&tmp, "PING :", 0);
      str_add_to((char *)&tmp, msg->msg, strlen(tmp));
    break;
    case PING:
      str_add_to((char *)&tmp, "PONG :", 0);
      str_add_to((char *)&tmp, msg->msg, strlen(tmp));
    break;
    case RPL_ENDOFMOTD:
      str_add_to((char *)&tmp, "JOIN ", 0);
      str_add_to((char *)&tmp, srv->channels, strlen(tmp));
    break;
  }

  if(strlen(tmp) > 0) {
    send_raw(sock, tmp);
  }
}

void reset_msg(struct irc_msg *msg) {
  msg->type = NONE;
  memset(msg->raw, 0 , strlen(msg->raw));
  safefree((void **)&msg->msg);
  safefree((void **)&msg->nick_sent);
  safefree((void **)&msg->channel);
}

void send_ident(struct irc_socket *sock, struct irc_server_info *srv) {
  char identmsg[MAX_LINE_SIZE];
  memset(identmsg, 0, sizeof(identmsg));

  str_add_to((char *)&identmsg, "PASS ", 0);
  str_add_to((char *)&identmsg, srv->password, strlen(identmsg));
  send_raw(sock, identmsg);
  memset(identmsg, 0, sizeof(identmsg));

  str_add_to((char *)&identmsg, "NICK ", 0);
  str_add_to((char *)&identmsg, srv->nick, strlen(identmsg));
  send_raw(sock, identmsg);
  memset(identmsg, 0, sizeof(identmsg));

  str_add_to((char *)&identmsg, "USER wat wat wat *:", 0);
  str_add_to((char *)&identmsg, srv->nick, strlen(identmsg));
  send_raw(sock, identmsg);
}
