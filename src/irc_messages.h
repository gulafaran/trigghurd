/*
 * --------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tomenglund26@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Tom Englund
 * --------------------------------------------------------------------------------
 */

enum msg_type {
  NONE, PRIVMSG, PONG, PING,
  RPL_ENDOFMOTD = 376
};

struct irc_msg {
  enum msg_type type;
  char *raw;
  char *msg;
  char *nick_sent;
  char *channel;
};

void create_msg(struct irc_socket *sock, struct irc_msg *msg);
void handle_msg(struct irc_socket *sock, struct irc_server_info *srv, struct irc_msg *msg);
void reset_msg(struct irc_msg *msg);
void send_ident(struct irc_socket *sock, struct irc_server_info *srv);
