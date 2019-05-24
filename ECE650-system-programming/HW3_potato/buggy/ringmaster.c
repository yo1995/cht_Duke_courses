#include "potato.h"

int main(int argc, char *argv[]){

	/* step 1, sanity check part, Check to see if input arguments are valid */
	if(argc != 4){
		fprintf(stderr, "Input Error! Usage: ringmaster <port_num> <num_players> <num_hops>\n");
		return EXIT_FAILURE;
	}
	char* port_num = argv[1];
	int num_players = atoi(argv[2]);
	int num_hops = atoi(argv[3]);
	if(num_players <= 1) {
		fprintf(stderr, "Input out of range! Players must be more than 1\n");
		return EXIT_FAILURE;
	}
	if(num_hops < 0 || num_hops > 512) {
		fprintf(stderr, "Input out of range! 0 <= num_hops <= 512\n");
		return EXIT_FAILURE;
	}
	
#ifdef DEBUG
	if(atoi(argv[1]) < 1 || atoi(argv[1]) > 65535) {
		fprintf(stderr, "Port# out of range!\n");
		return EXIT_FAILURE;
	}
	fprintf(stdout, "debug info: Ringmaster inputs Port = %s\tPlayers = %d\tHops = %d\n", port_num, num_players, num_hops);
#endif

	fprintf(stdout, "Potato Ringmaster\nPlayers = %d\nHops = %d\n", num_players, num_hops);

	/* step 2, Create sockets between all players and the master */
	int master_socket_fd = create_master_and_listen(port_num);  // create the master as server, keep listening to incoming connections
	
	player players[num_players];  // create an array of player structs
	
	int i = 0;
	while (i < num_players) {
		players[i].id = i;
		players[i].fd = accept_and_receive_ping_from_player(master_socket_fd);  // block until all ping recvd
		fprintf(stdout, "Player %d is ready to play\n", i);
		++i;
	}
	// after this loop, all player should've connected to the master

	/* step 3, Create sockets between players to their left and right */

	// first, let players get to know each other
	i = 0;
	host_port temp;
	while (i < num_players) {
		temp = receive_hostname_port_from_player(players[i].fd);
		players[i].port = temp.port;  // why its 256x???   / 256
		// fprintf(stdout, "debug - hostname %s\n", temp.hostname);
		// fprintf(stdout, "debug - port %d\n", temp.port);
		strncpy(players[i].hostname, temp.hostname, LEN_HOSTNAME);
		++i;
	}
	// then, set up all left and right ring opponents
	// be aware of the corner case of both end of the ring!
	i = 0;
	while (i < num_players) {  // the head
		if (i == 0) {
			players[i].l_id = players[num_players-1].id;
			strncpy(players[i].l_hostname, players[num_players-1].hostname, LEN_HOSTNAME);
			players[i].l_port = players[num_players-1].port;
			players[i].r_id = players[i+1].id;
			strncpy(players[i].r_hostname, players[i+1].hostname, LEN_HOSTNAME);
			players[i].r_port = players[i+1].port;
		}
		else if (i == num_players - 1) {  // the tail
			players[i].l_id = players[i-1].id;
			strncpy(players[i].l_hostname, players[i-1].hostname, LEN_HOSTNAME);
			players[i].l_port = players[i-1].port;
			players[i].r_id = players[0].id;
			strncpy(players[i].r_hostname, players[0].hostname, LEN_HOSTNAME);
			players[i].r_port = players[0].port;
		}
		else {
			players[i].l_id = players[i-1].id;
			strncpy(players[i].l_hostname, players[i-1].hostname, LEN_HOSTNAME);
			players[i].l_port = players[i-1].port;
			players[i].r_id = players[i+1].id;
			strncpy(players[i].r_hostname, players[i+1].hostname, LEN_HOSTNAME);
			players[i].r_port = players[i+1].port;
		}
		++i;
	}
	// till now, all the fields in player struct are completed
	// next, tell each player their full info
	i = 0;
	while (i < num_players) {
		send_player_full_info_back(players[i]);
		++i;
	}
	// wait until all players are connected to right and left and of course binded.
	// 
	const char done_flag[ACK_MSG_LEN] = "INIT_DONE";
	char done[ACK_MSG_LEN];

	i = 0;
	while (i < num_players) {
		memset(done, 0, sizeof(done));
		recv(players[i].fd, &done, sizeof(done), MSG_WAITALL);
		if(strncmp(done, done_flag, ACK_MSG_LEN)==0) {
			// received one signal
			fprintf(stdout, "debug info: init done for %d.\n", i);
		}
		++i;
	}


#ifdef DEBUG
	fprintf(stdout, "debug info: all connections established.\n");
#endif


	/* ------------------------------------------------------ */
	// assume all settings are done, start the game
	
	
#ifdef DEBUG
	fprintf(stdout, "debug info: will start game\n");
#endif
	if (num_hops == 0) {  // shutdown game immediately
		// TODO: checkout what to clean before exit
		// tell all processes to exit
		fprintf(stdout, "info: no hops, just end the game\n");
		close(master_socket_fd);
		i = 0;
		while (i < num_players) {
			close(players[i].fd);
			++i;
		}
		return EXIT_SUCCESS;
	}

	// the randome seed to start
	srand((unsigned int) time(NULL));
	int starting_player_id = rand() % num_players;

	potato poo;
	potato fake_potato;  // use for calling the game
	potato final;

	poo.hops_remaining = num_hops;
	poo.hops_count = num_hops;  // to calculate the index of trace
	poo.signal_value = START_OF_GAME;

	fake_potato.hops_remaining = -1;
	fake_potato.hops_count = -1;
	fake_potato.signal_value = START_OF_GAME;
	
	i = 0;
	while (i < MAX_HOPS) {
		poo.trace[i] = -1;  // init trace to -1 for convenience.
		fake_potato.trace[i] = -1;
		final.trace[i] = -1;
		++i;
	}

#ifdef DEBUG
	i = 0;
	fprintf(stdout, "debug info: check player fd\n");
	while (i < num_players) {
		printf("%d, ", players[i].fd);
		++i;
	}
	fprintf(stdout, "\n");
	fprintf(stdout, "debug info: check master fd\n");
	printf("%d, ", master_socket_fd);
	fprintf(stdout, "\n");
	fprintf(stdout, "info: start the game with uid %d\n", starting_player_id);
#endif

	// add all players to the set, and listen for end
	fd_set set;
	FD_ZERO(&set);
	i = 0;
	int maxfdp = -1;
	int status;
	int final_hop_index = -1;
	while (i < num_players) {
		FD_SET(players[i].fd, &set);
		maxfdp = max(maxfdp, players[i].fd);
		++i;
	}
	maxfdp++;

	if (num_hops == 1) {  // shutdown game and trace the only id
		// TODO: checkout what to clean before exit
		fprintf(stdout, "info: 1 hop, just end the game\n");
		poo.hops_remaining--;
		poo.trace[0] = starting_player_id;
		// TODO: print something here
		return EXIT_SUCCESS;
	}
	else {
		// poo.hops_remaining--;
		// poo.trace[0] = starting_player_id;

		i = 0;
		send_out_potato_to_players(players[starting_player_id].fd, poo);
		// wait for a potato from last player
		// 
		status = select(maxfdp, &set, NULL, NULL, NULL);
		if (status == -1) {  // from beej's guide
		    perror("select"); // error occurred in select()
		    return EXIT_FAILURE;
		}
		else if (status == 0) {
			fprintf(stderr, "Error: Timeout occurred!\n");
		    return EXIT_FAILURE;
		}
		else {
			i = 0;
			while (i < num_players) {
				if(FD_ISSET(players[i].fd, &set)) {
					final = receive_potato_from_players(players[i].fd);
					final_hop_index = i;
					break;
				}
				++i;
			}
			// without set, the line below cannot work. maybe use listen later.
			// final = accept_and_receive_potato_from_player(master_socket_fd);
		}
	}
	// print out the trace in potato

	print_trace(final);
	// tell players the game ends
	i = 0;
	fake_potato.signal_value = END_OF_GAME;
	while (i < num_players) {
		if (i == final_hop_index) {
			++i;
			continue;
		}
		send_out_potato_to_players(players[i].fd, fake_potato);
		close(players[i].fd);
		++i;
	}
	
	// do some cleaning below
	
	close(master_socket_fd);
	

	return EXIT_SUCCESS;
}
