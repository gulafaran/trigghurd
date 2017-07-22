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
#include <stdbool.h>

#include "irc_connection.h"
#include "config.h"

int main(void) {
  struct irc_server_info srv;
  srv.port = IRC_PORT;
  srv.adress= IRC_ADRESS;
  srv.password = IRC_PASSWORD;
  srv.channels = IRC_CHANNELS;
  srv.nick = IRC_NICK;
  srv.ssl = IRC_SSL;
  srv.connected = false;

  create_connection(&srv);

  return EXIT_SUCCESS;
}
