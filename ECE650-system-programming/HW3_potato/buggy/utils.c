#include "potato.h"

/**
 * create the ringmaster as a server. nearly entirely borrowed from tcp_example
 * @param  port the port to listen, for the potato service
 * @return      the socket file descriptor for further use
 */
int create_master_and_listen(char* port) {
	int status;
    int socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    const char *hostname = NULL;

    memset(&host_info, 0, sizeof(host_info));

    host_info.ai_family   = AF_INET;
    host_info.ai_socktype = SOCK_STREAM;
    host_info.ai_flags    = AI_PASSIVE;

    fprintf(stdout, "Info: This is master socket.\n");

    status = getaddrinfo(hostname, port, &host_info, &host_info_list);
    if (status != 0) {
    	fprintf(stderr, "Error: cannot get address info for host\n");
    	fprintf(stderr, "(%s, %s))\n", hostname, port);
    	return EXIT_FAILURE;
    } //if

    socket_fd = socket(host_info_list->ai_family,
                       host_info_list->ai_socktype,
                       host_info_list->ai_protocol);

    if (socket_fd == -1) {
        fprintf(stderr, "Error: cannot create socket\n");
    	fprintf(stderr, "(%s, %s))\n", hostname, port);
    	return EXIT_FAILURE;
    } //if

    int yes = 1;
    status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    
    if (status == -1) {
        fprintf(stderr, "Error: cannot bind socket\n");
    	fprintf(stderr, "(%s, %s))\n", hostname, port);
    	return EXIT_FAILURE;
    } //if

    status = listen(socket_fd, 100);

    if (status == -1) {
        fprintf(stderr, "Error: cannot listen on socket\n");
    	fprintf(stderr, "(%s, %s))\n", hostname, port);
    	return EXIT_FAILURE;
    } //if

    fprintf(stdout, "Info: Waiting for connection on port %s\n", port);
    freeaddrinfo(host_info_list);
    return socket_fd;
}


int create_player_and_connect_to_master(char* hostname, char* port) {
    int status;
    int socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    
    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family   = AF_INET;
    host_info.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(hostname, port, &host_info, &host_info_list);
    
    if (status != 0) {
        fprintf(stderr, "Error: cannot get address info for master\n");
        fprintf(stderr, "(%s, %s))\n", hostname, port);
        return EXIT_FAILURE;
    } //if

    socket_fd = socket(host_info_list->ai_family, 
             host_info_list->ai_socktype, 
             host_info_list->ai_protocol);

    if (socket_fd == -1) {
        fprintf(stderr, "Error: cannot create socket\n");
        fprintf(stderr, "(%s, %s))\n", hostname, port);
        return EXIT_FAILURE;
    } //if

    fprintf(stdout, "Info: Connecting to master on port %s\n", port);
    status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);

    if (status == -1) {
        fprintf(stderr, "Error: cannot connect to socket\n");
        fprintf(stderr, "(%s, %s))\n", hostname, port);
        return EXIT_FAILURE;
    } //if

    const char *message = "Ping: Hi there from player!";
    send(socket_fd, message, strlen(message), 0);

    freeaddrinfo(host_info_list);
    return socket_fd;
    // dont forget to close the socket after game is done.
}

int accept_and_receive_ping_from_player(int socket_fd) {
    // below receiving msg
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    int client_connection_fd;
    client_connection_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);

    if (client_connection_fd == -1) {
        fprintf(stderr, "Error: cannot accept connection on master socket\n");
        return EXIT_FAILURE;
    } //if

    char buffer[32];
    recv(client_connection_fd, buffer, 27, MSG_WAITALL);
    buffer[27] = 0;

    fprintf(stdout, "Pong: Server received: %s\n", buffer);
    return client_connection_fd;
}

potato accept_and_receive_potato_from_player(int socket_fd) {
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    int client_connection_fd;
    client_connection_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);

    if (client_connection_fd == -1) {
        fprintf(stderr, "Error: cannot accept connection on master socket\n");
        exit(EXIT_FAILURE);
    } //if

    char buffer[sizeof(potato)];
    recv(client_connection_fd, &buffer, sizeof(potato), MSG_WAITALL);
    potato poo;
    memcpy(&poo, buffer, sizeof(potato));

    fprintf(stdout, "Pong: Server received potato\n");
    return poo;
}

host_port receive_hostname_port_from_player(int fd) {
	char buffer1[LEN_HOSTNAME];  // have a byte array buffer
    char buffer2[sizeof(int)];
    host_port s;
    int temp;
	recv(fd, &buffer1, LEN_HOSTNAME, MSG_WAITALL);
	recv(fd, &buffer2, sizeof(int), MSG_WAITALL);

    strncpy(s.hostname, buffer1, LEN_HOSTNAME);
	memcpy(&temp, buffer2, sizeof(int));
    s.port = temp;

	return s;
}

void send_hostname_port_to_master(int fd, host_port hp) {
    // fprintf(stdout, "debug - port %d, h %s\n", hp.port, hp.hostname);
    const char *message = hp.hostname;

	send(fd, message, LEN_HOSTNAME, 0);
    int temp = hp.port;
    send(fd, &temp, sizeof(temp), 0);

	return;
}

player receive_player_full_info(int fd) {
	char buffer[sizeof(player)];  // have a byte array buffer
	recv(fd, &buffer, sizeof(player), MSG_WAITALL);
	player s;
	memcpy(&s, buffer, sizeof(player));
	// do we need to do any cleaning right here?
	return s;
}

void send_player_full_info_back(player p) {
	char buffer[sizeof(player)];
	memcpy(buffer, &p, sizeof(player));
	send(p.fd, buffer, sizeof(player), 0);
	return;
}

void send_out_potato_to_players(int p_fd, potato poo) {
    char buffer[sizeof(potato)];
    memcpy(buffer, &poo, sizeof(potato));
    send(p_fd, buffer, sizeof(potato), 0);
    return;
}

potato receive_potato_from_players(int fd) {
    char buffer[sizeof(potato)];
    recv(fd, &buffer, sizeof(potato), MSG_WAITALL);
    potato poo;
    memcpy(&poo, buffer, sizeof(potato));
    return poo;
}

int create_player_socket() {
    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == -1) {
        fprintf(stderr, "Error: cannot create socket as player host.\n");
        return EXIT_FAILURE;
    } //if
    return socket_fd;
}

int bind_player_and_listen(int socket_fd, char* port) {
    int status;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    const char *hostname = NULL;

    memset(&host_info, 0, sizeof(host_info));

    host_info.ai_family   = AF_INET;
    host_info.ai_socktype = SOCK_STREAM;
    host_info.ai_flags    = AI_PASSIVE;

    fprintf(stdout, "Info: This is player socket.\n");

    status = getaddrinfo(hostname, port, &host_info, &host_info_list);
    if (status != 0) {
        fprintf(stderr, "Error: cannot get address info for player.\n");
        fprintf(stderr, "(%s, %s))\n", hostname, port);
        return EXIT_FAILURE;
    } //if

	// should put the loop inside here, only establish socket once
    int yes = 1;
    status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    
    if (status == -1) {
    	perror("Error: bind ");
        fprintf(stderr, "Error: cannot bind socket as player host.\n");
    	fprintf(stderr, "(%s, %s))\n", hostname, port);
    	return EXIT_FAILURE;
    } //if

    status = listen(socket_fd, 100);

    if (status == -1) {
        fprintf(stderr, "Error: cannot listen on socket as player host.\n");
    	fprintf(stderr, "(%s, %s))\n", hostname, port);
    	return EXIT_FAILURE;
    } //if

    fprintf(stdout, "Info: Waiting for connection on port %s as player host\n", port);
    freeaddrinfo(host_info_list);
    return socket_fd;
}


int connect_to_neighbor_and_accept(int socket_fd, int own_socket_fd, char* hostname, char* port) {
    int status;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    
    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family   = AF_INET;
    host_info.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(hostname, port, &host_info, &host_info_list);
    
    if (status != 0) {
        fprintf(stderr, "Error: cannot get address info for master\n");
        fprintf(stderr, "(%s, %s))\n", hostname, port);
        return EXIT_FAILURE;
    } //if

    status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);

    if (status == -1) {
        fprintf(stderr, "Error: cannot connect to neighbor's socket\n");
        fprintf(stderr, "(%s, %s))\n", hostname, port);
        return EXIT_FAILURE;
    } //if

    // below receiving msg
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    int client_connection_fd;
    client_connection_fd = accept(own_socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);

    if (client_connection_fd == -1) {
        fprintf(stderr, "Error: cannot accept connection on socket\n");
        return EXIT_FAILURE;
    } //if

    freeaddrinfo(host_info_list);
    return client_connection_fd;
    // dont forget to close the socket after game is done.
}


void print_player(player p) {
	fprintf(stdout, "==================================\n");
	fprintf(stdout, "debug info: print out player \n");
	fprintf(stdout, "id: %d, port: %d, hostname: %s\n", p.id, p.port, p.hostname);
	fprintf(stdout, "left id: %d, left port: %d, left hostname: %s\n", p.l_id, p.l_port, p.l_hostname);
	fprintf(stdout, "right id: %d, right port: %d, right hostname: %s\n", p.r_id, p.r_port, p.r_hostname);
	fprintf(stdout, "==================================\n");
}

void print_trace(potato poo) {
    int i = 0;
    while (poo.trace[i] != -1 && i < MAX_HOPS) {
        fprintf(stdout, "%d, ", poo.trace[i]);
        ++i;
    }
    fprintf(stdout, "\n");
}

char *my_itoa(char *str, int i) {
    sprintf(str, "%d", i);
    return str;
}

int max(int a, int b) {
    return a > b? a : b;
}
