/*
 * --------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tomenglund26@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Tom Englund
 * --------------------------------------------------------------------------------
 */

#define IRC_PORT 6667
#define IRC_ADRESS "irc.freenode.net"
#define IRC_PASSWORD ""
#define IRC_CHANNELS "#tomtest"
#define IRC_NICK "trigghurd"
#define IRC_SSL false // for now

//
// below is the dont touch section unless you know what you are doing
//

#define BUFFER_SIZE 512 // buffer size for reading
#define MAX_LINE_SIZE 510 // according to irc rfc, including CRL-LF
#define PRINT_TO_STDOUT true // print all recieved and sent data to stdout
