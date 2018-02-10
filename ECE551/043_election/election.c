#include "election.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
//include any other headers you need here...

state_t parseLine(const char * line) {
  //STEP 1: write me
	int i = 0;
	int length = 0;
	/*flag = 1, number; flag = -1, invalid chars, flag = 0,else */
	int count_colon = 0;
	int length_population = 0;
	int length_num_votes = 0;
	long int pos_neg_test = 0;
	//char *population = NULL;
	//char *num_votes = NULL;
	char *end = NULL;
	state_t election_data = {"", 0, 0};
	//strlen not including \0 at the last
	length = strlen(line);
	//printf("%d\n", length);
	while (i < length + 1){
		if ((line[i] == '\n') || (line[i] == '\0')) {
			fprintf(stderr,"Improper input! Exit program now.\n");
			exit(EXIT_FAILURE);
		}
		
		if (line[i] == ':') {
			if (line[i + 1] == ':') {
				//reach \0 at the end, not exceeding limits.
				fprintf(stderr,"Consecutive colons, invalid input! Exit.\n");
				exit(EXIT_FAILURE);
			}
			count_colon++;
			i++;
		}
		
		//fisrt state name
		if (count_colon == 0) {
			if (i > 63){
				fprintf(stderr,"Overflow the max length of state name! Exit.\n");
				exit(EXIT_FAILURE);
			}
			election_data.name[i] = line[i];
		}
		//then population
		else if (count_colon == 1) {
			//put an end to name
			election_data.name[i - 1] = '\0';
			pos_neg_test = (strtoul((line + i), &end, 0));
			if (pos_neg_test < 0) {
				fprintf(stderr,"No smaller than 0! Exit.\n");
				exit(EXIT_FAILURE);
			}
			election_data.population = strtol((line + i), &end, 0);
			
			//printf("population is %zu\n", election_data.population);
			length_population = end - (line + i);
			if(length_population == 0) {
				fprintf(stderr,"0 length of population! Exit.\n");
				exit(EXIT_FAILURE);
			}
			end = NULL;
			i += length_population - 1;
			//next symbol must be colon
			if (line[i + 1] != ':'){
				fprintf(stderr,"Wrong format for population! Exit.\n");
				exit(EXIT_FAILURE);
			}
		}
		//then num of votes
		else if (count_colon == 2) {
			pos_neg_test = (strtoul((line + i), &end, 0));
			if (pos_neg_test < 0) {
				fprintf(stderr,"No smaller than 0! Exit.\n");
				exit(EXIT_FAILURE);
			}
			election_data.electoralVotes = strtoul((line + i), &end, 0);
			length_num_votes = end - (line + i);
			if(length_num_votes == 0) {
				fprintf(stderr,"0 length of electoralVotes! Exit.\n");
				exit(EXIT_FAILURE);
			}
			end = NULL;
			//printf("electoralVotes is %u\n", election_data.electoralVotes);
			i += length_num_votes - 1;
			if (election_data.electoralVotes > election_data.population) {
				fprintf(stderr,"Votes cannot be greater than population! Exit.\n");
				exit(EXIT_FAILURE);
			}
			if (line[i + 1] != '\0'){
				fprintf(stderr,"Wrong format for electoralVotes or ending! Exit.\n");
				exit(EXIT_FAILURE);
			}
			break; //if succeed should already have the data
		}
		//other invalid maybe nothing
		else {
			fprintf(stderr,"Overflow the valid number of colons! Exit.\n");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return election_data;
}

unsigned int countElectoralVotes(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 2: write me
	int i = 0;
	int totalElectoralVotes = 0;
	double ratio;
	while(i < nStates) {
		if(stateData[i].population < 1) {
			fprintf(stderr,"Wrong population! Exit.\n");
			exit(EXIT_FAILURE);
		}
		if (voteCounts[i] > stateData[i].population){
			fprintf(stderr,"VoteCounts more than eligible electoralVotes! Exit.\n");
			exit(EXIT_FAILURE);
		}
		ratio = voteCounts[i] / (double)stateData[i].population;
		if(voteCounts[i] < 0 || ratio > 1) {
			fprintf(stderr,"Wrong voteCounts! Exit.\n");
			exit(EXIT_FAILURE);
		}
		
		if(ratio > 0.5) {
			totalElectoralVotes += stateData[i].electoralVotes;
		}
		i++;
	}
	return totalElectoralVotes;
}

void printRecounts(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 3: write me
	int i = 0;
	double ratio = 0;
	while(i < nStates) {
		if(stateData[i].population < 1) {
			fprintf(stderr,"Wrong population! Exit.\n");
			exit(EXIT_FAILURE);
		}
		if (voteCounts[i] > stateData[i].population){
			fprintf(stderr,"VoteCounts more than eligible electoralVotes! Exit.\n");
			exit(EXIT_FAILURE);
		}
		ratio = voteCounts[i] / (double)stateData[i].population;
		if(voteCounts[i] < 0 || ratio > 1) {
			fprintf(stderr,"Wrong voteCounts! Exit.\n");
			exit(EXIT_FAILURE);
		}
		
		if(ratio >= 0.495 && ratio <= 0.505) {
			printf("%s requires a recount (Candidate A has %.2f%% of the vote)\n", stateData[i].name, ratio * 100);
		}
		i++;
	}
}

void printLargestWin(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 4: write me
	int i = 0;
	double ratio = 0;
	double largest_ratio = 0;
	char winnerName[MAX_STATE_NAME_LENGTH];
	while(i < nStates) {
		if(stateData[i].population < 1) {
			fprintf(stderr,"Wrong population! Exit.\n");
			exit(EXIT_FAILURE);
		}
		if (voteCounts[i] > stateData[i].population){
			fprintf(stderr,"VoteCounts more than eligible electoralVotes! Exit.\n");
			exit(EXIT_FAILURE);
		}
		ratio = voteCounts[i] / (double)stateData[i].population;
		if(voteCounts[i] < 0 || ratio > 1) {
			fprintf(stderr,"Wrong voteCounts! Exit.\n");
			exit(EXIT_FAILURE);
		}
		
		if (ratio >largest_ratio) {
			largest_ratio = ratio;
			strcpy(winnerName, stateData[i].name);
		}
		i++;
	}
	printf("Candidate A won %s with %.2f%% of the vote\n", winnerName, largest_ratio * 100);
}
