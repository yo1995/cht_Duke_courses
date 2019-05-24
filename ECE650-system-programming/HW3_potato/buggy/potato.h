#ifndef _POTATO_H
#define _POTATO_H

#define MAX_HOPS 512
#define LEN_HOSTNAME 1024  // theoretically 255 should be enough, but...
#define LEN_PORT 16

// define some signal values for communicating between each other
// not to do it here, since send/recv deals better with byte array rather than int.
#define ACK_MSG_LEN 64
#define START_OF_GAME 10
#define END_OF_GAME 11

// define a range for inter-player socket binding
#define BIND_PORT_LOWER_BOUND 20000
#define BIND_PORT_UPPER_BOUND 20999

#define DEBUG  // add a macro debugging control flag, comment to disable

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>

/* the potato structure (object) */
struct _potato {
	int signal_value;  // since C does not have handy enum type, just use an int
	int hops_count;
	int hops_remaining;
	int trace[MAX_HOPS];  // to store the trace of players
};
typedef struct _potato potato;

/* the player structure, to keep track of each player's data */
/* to keep simplicity, just store port number as int to avoid strncpy */
struct _player {

	int id;
	int fd;
	char hostname[LEN_HOSTNAME];
	int port;

	int l_id;
	char l_hostname[LEN_HOSTNAME];
	int l_port;
	
	int r_id;
	char r_hostname[LEN_HOSTNAME];
	int r_port;
};
typedef struct _player player;

/* a middleware struct to transfer hostname and port of a player */
// it might be a bad idea to send struct through socket
// and IT DID CAUSE TROUBLE!!! due to some padding issues
// edit - not the cause. recv wrong size led to that problem
// https://stackoverflow.com/a/1577174
struct _host_port {

	char hostname[LEN_HOSTNAME];
	int port;

};
typedef struct _host_port host_port;

/* some helper function prototypes below */

int create_master_and_listen(char* port);
int create_player_and_connect_to_master(char* hostname, char* port);
int accept_and_receive_ping_from_player(int socket_fd);
host_port receive_hostname_port_from_player(int fd);
void send_hostname_port_to_master(int fd, host_port hp);
player receive_player_full_info(int fd);
void send_player_full_info_back(player p);
void print_player(player p);
int create_player_socket();
int bind_player_and_listen(int socket_fd, char* port);
int connect_to_neighbor_and_accept(int socket_fd, int own_socket_fd, char* hostname, char* port);
potato accept_and_receive_potato_from_player(int socket_fd);
potato receive_potato_from_players(int fd);
void send_out_potato_to_players(int p_fd, potato poo);
void print_trace(potato poo);
char *my_itoa(char *str, int i);
int max(int a, int b);

#endif
