#include "potato.h"

int main(int argc, char *argv[]){
	/* step 1, sanity check part, Check to see if input arguments are valid */
	if (argc != 3){
		fprintf(stderr, "Input Error! Usage: player <machine_name> <port_num>\n");
		return EXIT_FAILURE;
	}

	char* machine_name = argv[1];
	char* port_num = argv[2];
	
#ifdef DEBUG
	if (atoi(argv[2]) < 1 || atoi(argv[2]) > 65535) {
		fprintf(stderr, "Error: Port# out of range!\n");
		return EXIT_FAILURE;
	}
	fprintf(stdout, "debug info: Player inputs IP = %s\tPort = %s\t\n", machine_name, port_num);
#endif

	/* step 2, Create sockets between current player and the master */
	int player_socket_fd = create_player_and_connect_to_master(machine_name, port_num);
	
	/* step 3, tell master player's own info, and receive complete info */
	// get player's own h_p
	// referred to https://stackoverflow.com/questions/504810
	// gethostbyname is deprecated, however I still use it for its simplicity.
	host_port temp_h_p;
	temp_h_p.port = 0;

	char player_hostname[LEN_HOSTNAME];
	player_hostname[LEN_HOSTNAME - 1] = 0;  // or '/0', same
	gethostname(player_hostname, LEN_HOSTNAME);
	struct hostent* h;
	h = gethostbyname(player_hostname);

#ifdef DEBUG
	fprintf(stdout, "debug info: player's hostname: %s\n", player_hostname);
	fprintf(stdout, "debug info: player's h_name: %s\n", h -> h_name);
#endif

	if (h == NULL) {
		fprintf(stderr, "Error: get hostname failed!\n");
    	return EXIT_FAILURE;
  	}
  	// acquire player's hostname
  	strncpy(temp_h_p.hostname, h -> h_name, LEN_HOSTNAME);
  	// acquire player's port number as an integer
  	// actually... we need to find a free port and bind it

  	int player_as_server_socket_fd = create_player_socket();

  	int i = BIND_PORT_LOWER_BOUND;
  	char port_str[LEN_PORT];  // only 20000-20999 in header, or 65535 at max
  	memset(port_str, 0, sizeof(port_str));  // clear the array hc way ;-)
  	int status;
    while (i < BIND_PORT_UPPER_BOUND) {
  		memset(port_str, 0, sizeof(port_str));
  		my_itoa(port_str, i);
  		// fprintf(stdout, "debug - port %s\n", port_str);
  		status = bind_player_and_listen(player_as_server_socket_fd, port_str);
  		if (status != EXIT_FAILURE) {
  			break;
  		}
  		++i;
  	}


  	if (i == BIND_PORT_UPPER_BOUND) {
  		fprintf(stderr, "Error: Cannot bind player port. maybe all port used between %d - %d!\n", BIND_PORT_LOWER_BOUND, BIND_PORT_UPPER_BOUND);
		return EXIT_FAILURE;
  	}
  	temp_h_p.port = i;
  	// fprintf(stdout, "debug - port %d, fd %d\n", i, player_as_server_socket_fd);
	send_hostname_port_to_master(player_socket_fd, temp_h_p);

	// receive player full information from master
	player current_player;
	current_player = receive_player_full_info(player_socket_fd);

#ifdef DEBUG
	print_player(current_player);
#endif

	/* step 4, try to establish connection with left and right */
	int l_fd = create_player_socket();
	int r_fd = create_player_socket();
	// struct hostent* l_h = gethostbyname(current_player.l_hostname);
	// struct hostent* r_h = gethostbyname(current_player.r_hostname);
	// if (l_h == NULL || r_h == NULL ) {
	// 	fprintf(stderr, "Error: get left/right hostname failed!\n");
 //    	return EXIT_FAILURE;
 //  	}
  	// first, connect to left neighbor
  	memset(port_str, 0, sizeof(port_str));  // clear the array hc way ;-)
  	my_itoa(port_str, current_player.l_port);
  	int l_host_fd = connect_to_neighbor_and_accept(l_fd, player_as_server_socket_fd, current_player.l_hostname, port_str);
  	if (l_host_fd == EXIT_FAILURE) {
  		fprintf(stderr, "Error: Cannot connect to left neighbor.\n");
  		return EXIT_FAILURE;
  	}
  	// then, connect to right neighbor
  	memset(port_str, 0, sizeof(port_str));  // clear the array hc way ;-)
  	my_itoa(port_str, current_player.r_port);
  	int r_host_fd = connect_to_neighbor_and_accept(r_fd, player_as_server_socket_fd, current_player.r_hostname, port_str);
  	if (r_host_fd == EXIT_FAILURE) {
  		fprintf(stderr, "Error: Cannot connect to right neighbor.\n");
  		return EXIT_FAILURE;
  	}

  	// after all settings done, send "INIT_DONE" signal to master.
  	const char done[ACK_MSG_LEN] = "INIT_DONE";
  	send(player_socket_fd, done, ACK_MSG_LEN, 0);



  	/* ------------------------------------------------------ */
	// assume all settings are done, start the game
	// wait for potato
	fd_set set;
	FD_ZERO(&set);
	FD_SET(l_host_fd, &set);
	FD_SET(r_host_fd, &set);
	FD_SET(player_socket_fd, &set);  // also listen from master

	
	srand((unsigned int) time(NULL));
	int maxfdp = max(l_host_fd, max(r_host_fd, player_socket_fd)) + 1;

	// i = 0;
	// while (i < MAX_HOPS) {
	// 	poo.trace[i] = -1;  // init trace to -1 for convenience.
	// 	++i;
	// }

	while (true) {  // always wait for potato unless receive a fake to end
		status = select(maxfdp, &set, NULL, NULL, NULL);
		if (status == -1) {  // from beej's guide
		    perror("select"); // error occurred in select()
		    return EXIT_FAILURE;
		}
		else if (status == 0) {
			fprintf(stderr, "Error: Timeout occurred!\n");
		    return EXIT_FAILURE;
		}
		else {  // select succeeded
			potato poo;
		    if (FD_ISSET(l_host_fd, &set)) {
				fprintf(stdout, "left selected\n");
				recv(l_host_fd, &poo, sizeof(potato), MSG_WAITALL);
				print_trace(poo);
				if (poo.hops_remaining == 1) {  // only 1 hop from left
					
					fprintf(stdout, "I'm it l\n");
					// do some other logics here
					// TODO: 
					poo.signal_value = END_OF_GAME;
					poo.trace[poo.hops_count - poo.hops_remaining] = current_player.id;
					poo.hops_remaining--;
					fprintf(stdout, "Warning: sent to master\n");
					send_out_potato_to_players(player_socket_fd, poo);
					break;
				}
				else if (poo.signal_value == END_OF_GAME) {
					fprintf(stdout, "Error: sth wrong, just break here.\n");
					return EXIT_FAILURE;
				}
				else if (poo.hops_remaining < 1) {
					fprintf(stderr, "Error: hops not correct!\n");
					return EXIT_FAILURE;
				}
				else {
					// this is the real potato! do something to it
					poo.trace[poo.hops_count - poo.hops_remaining] = current_player.id;
					poo.hops_remaining--;
					fprintf(stdout, "%d hops remaining\n", poo.hops_remaining);
				}
			}
			else if (FD_ISSET(r_host_fd, &set)) {
				fprintf(stdout, "right selected\n");
				recv(r_host_fd, &poo, sizeof(potato), MSG_WAITALL);
				print_trace(poo);
				if (poo.hops_remaining == 1 && poo.signal_value != END_OF_GAME) {  // only 1 hop from left
					
					fprintf(stdout, "I'm it r\n");
					// do some other logics here
					// TODO: 
					poo.signal_value = END_OF_GAME;
					poo.trace[poo.hops_count - poo.hops_remaining] = current_player.id;
					poo.hops_remaining--;
					fprintf(stdout, "Warning: sent to master\n");
					send_out_potato_to_players(player_socket_fd, poo);
					break;
				}
				else if (poo.signal_value == END_OF_GAME) {
					fprintf(stdout, "Error: sth wrong, just break here.\n");
					return EXIT_FAILURE;
				}
				else if (poo.hops_remaining < 1) {
					fprintf(stderr, "Error: hops not correct!\n");
					return EXIT_FAILURE;
				}
				else {
					// this is the real potato! do something to it
					poo.trace[poo.hops_count - poo.hops_remaining] = current_player.id;
					poo.hops_remaining--;
					fprintf(stdout, "%d hops remaining\n", poo.hops_remaining);
				}

			}
			else if (FD_ISSET(player_socket_fd, &set)) {  // write from master
				fprintf(stdout, "master selected\n");

				recv(player_socket_fd, &poo, sizeof(potato), MSG_WAITALL);
				
				if (poo.signal_value == END_OF_GAME) {
					// this is the fake potato, end the game and garbage collect
					fprintf(stdout, "end of the game\n");
					break;
				}
				// else if (poo.signal_value == START_OF_GAME && poo.hops_remaining < 0) {
				// 	// this is the fake potato, start the game, just wait
				// 	continue;
				// }
				else if (poo.hops_remaining == 0) {  // only 1 hop from master
					fprintf(stdout, "I'm it m\n");
					poo.signal_value = END_OF_GAME;
					fprintf(stdout, "Warning: sent to master\n");
					send_out_potato_to_players(player_socket_fd, poo);
					// do some other logics here
					break;
				}
				else {
					// this is the real potato! do something to it
					poo.trace[poo.hops_count - poo.hops_remaining] = current_player.id;
					poo.hops_remaining--;
					fprintf(stdout, "%d hops remaining\n", poo.hops_remaining);
				}
			}
			else {
				fprintf(stderr, "Error: Just impossible! select failed.\n");
	  			return EXIT_FAILURE;
			}
			// reset the fd set
			FD_ZERO(&set);
			FD_SET(l_host_fd, &set);
			FD_SET(r_host_fd, &set);
			FD_SET(player_socket_fd, &set);

			// randomize the direction to send
			// TODO: should that be host_fd?
			if(rand() % 2 == 0){
				// send to left
				fprintf(stdout, "send to left\n");
				fprintf(stdout, "------------\n");
				send_out_potato_to_players(l_fd, poo);  //  
			}		
			else{
				// send to right
				fprintf(stdout, "send to right\n");
				fprintf(stdout, "------------\n");
				send_out_potato_to_players(r_fd, poo);  //  
			}
		}		
	}
	
	close(player_socket_fd);
	close(l_host_fd);
	close(r_host_fd);
	close(player_as_server_socket_fd);
	close(l_fd);
	close(r_fd);

	return EXIT_SUCCESS;
}
