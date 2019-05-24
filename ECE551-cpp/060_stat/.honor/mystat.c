#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
//my include to process strings
#include <string.h>

//This function is for Step 4
char * time2str(const time_t * when, long ns) {
	char * ans = malloc(128 * sizeof(*ans));
	char temp1[64];
	char temp2[32];
	const struct tm * t= localtime(when);
	strftime(temp1,512,"%Y-%m-%d %H:%M:%S",t);  
	strftime(temp2,32,"%z",t);
	snprintf(ans,128,"%s.%09ld %s", temp1, ns, temp2);
	return ans;
}

//additional function for step 1, to convert status number into readable text
char *step1_decide_st_mode(struct stat *buf) {
	char *fileType = NULL;
	switch (buf -> st_mode & S_IFMT) {
		case S_IFBLK:  fileType = strdup("block special file");		break;
		case S_IFCHR:  fileType = strdup("character special file");	break;
		case S_IFDIR:  fileType = strdup("directory");					break;
		case S_IFIFO:  fileType = strdup("fifo");							break;
		case S_IFLNK:  fileType = strdup("symbolic link");				break;
		case S_IFREG:  fileType = strdup("regular file");				break;
		case S_IFSOCK: fileType = strdup("socket");						break;
		default:        fileType = strdup("unknown?");					break;
    }
	return fileType; //no need to free here, free in main
}

//additional function for step 2, to convert human readable description of the permissions
//!!!!WARNING!!!! this function could be caught by Drew for some tricky corner case!
char *step2_permission_description(struct stat *buf) {
	char *outputString = NULL;
	outputString = malloc(11 * sizeof(*outputString)); //since the string is strictly 10 chars, we can declare a 11 length array in heap.
	//0
	if (outputString == NULL) {
			fprintf(stderr, "Insufficient memory in step 2, outputString!\n");
			exit(EXIT_FAILURE);
	}
	switch (buf -> st_mode & S_IFMT) {
		case S_IFBLK:  outputString[0] = 'b';		break;
		case S_IFCHR:  outputString[0] = 'c';		break;
		case S_IFDIR:  outputString[0] = 'd';		break;
		case S_IFIFO:  outputString[0] = 'p';		break;
		case S_IFLNK:  outputString[0] = 'l';		break;
		case S_IFREG:  outputString[0] = '-';		break;
		case S_IFSOCK: outputString[0] = 's';		break;
		default: 		{printf("File type not valid!\n");exit(EXIT_FAILURE);break;}
    }
	//1
	if (buf -> st_mode & S_IRUSR) {
		outputString[1] = 'r';
	}
	else {
		outputString[1] = '-';
	}
	//2
	if (buf -> st_mode & S_IWUSR) {
		outputString[2] = 'w';
	}
	else {
		outputString[2] = '-';
	}
	//3
	if (buf -> st_mode & S_IXUSR) {
		outputString[3] = 'x';
	}
	else {
		outputString[3] = '-';
	}
	//4
	if (buf -> st_mode & S_IRGRP) {
		outputString[4] = 'r';
	}
	else {
		outputString[4] = '-';
	}
	//5
	if (buf -> st_mode & S_IWGRP) {
		outputString[5] = 'w';
	}
	else {
		outputString[5] = '-';
	}
	//6
	if (buf -> st_mode & S_IXGRP) {
		outputString[6] = 'x';
	}
	else {
		outputString[6] = '-';
	}
	//7
	if (buf -> st_mode & S_IROTH) {
		outputString[7] = 'r';
	}
	else {
		outputString[7] = '-';
	}
	//8
	if (buf -> st_mode & S_IWOTH) {
		outputString[8] = 'w';
	}
	else {
		outputString[8] = '-';
	}
	//9
	if (buf -> st_mode & S_IXOTH) {
		outputString[9] = 'x';
	}
	else {
		outputString[9] = '-';
	}
	//10, end the string
	outputString[10] = '\0';
	return outputString;
}


//additional function for step 3, to sum up uid and gid
struct passwd *step3_uid(struct stat *buf) {
	struct passwd *pwd;
	pwd = getpwuid(buf -> st_uid);
	return pwd;
}

struct group *step3_gid(struct stat *buf) {
	struct group *grp;
	grp = getgrgid(buf -> st_gid);
	return grp;
}

//step 1 print out the first THREE lines of the output
//!!!!WARNING!!!! fancy quotes might not be right, check before grade!
void step1_print_three_lines (const char * inputName, struct stat *buf) {
	char *temp = step1_decide_st_mode(buf);//compare that result to several defined constants
	if (S_ISLNK(buf -> st_mode)) {
		//for this part of code, I REFERRED to manpage of readlink and used dynamic alloc instead of [256]
		ssize_t link;
		char *linkname;
		linkname = malloc(buf -> st_size + 1);
		if (linkname == NULL) {
			fprintf(stderr, "Insufficient memory in step 1, linkname!\n");
			exit(EXIT_FAILURE);
		}
		link = readlink(inputName, linkname, buf -> st_size + 1);
		if (link == -1) {
			perror("readlink");
			exit(EXIT_FAILURE);
		}
		if (link > buf -> st_size) {
			fprintf(stderr, "Symlink increased in size between lstat() and readlink()\n");
			exit(EXIT_FAILURE);
		}
		linkname[link] = '\0';
		printf("  File: '%s' -> '%s'\n", inputName, linkname);
		free(linkname);
	}
	else {
		printf("  File: '%s'\n", inputName);
	}
	printf("  Size: %-10lu\tBlocks: %-10lu IO Block: %-6lu %s\n", buf -> st_size, buf -> st_blocks, buf -> st_blksize, temp);
	free(temp);
	if ((S_ISCHR(buf -> st_mode)) || (S_ISBLK(buf -> st_mode))) { //step 6
		printf("Device: %lxh/%lud\tInode: %-10lu  Links: %-5lu Device type: %x,%x\n", buf -> st_dev, buf -> st_dev, buf -> st_ino, buf -> st_nlink, major(buf -> st_rdev), minor(buf -> st_rdev));
	}
	else {
		printf("Device: %lxh/%lud\tInode: %-10lu  Links: %lu\n", buf -> st_dev, buf -> st_dev, buf -> st_ino, buf -> st_nlink);
	}
}

//step 2 and 3 add the FOURTH line of output from stat
void step2and3_print_fourth_line_of_output (struct stat *buf) {
	char *temp_access = step2_permission_description(buf);
	struct passwd *temp_uid = step3_uid(buf);
	struct group *temp_gid = step3_gid(buf);
	printf("Access: (%04o/%s)  Uid: (%5d/%8s)   Gid: (%5d/%8s)\n", ((buf -> st_mode) & ~S_IFMT), temp_access, temp_uid -> pw_uid, temp_uid -> pw_name, temp_gid -> gr_gid, temp_gid -> gr_name);
	free(temp_access);
}

//step 4 add the last four lines of output
void step4_print_last_four_lines(struct stat *buf) {
	char *timestr_access = time2str(&(buf -> st_atime), buf -> st_atim.tv_nsec);
	char *timestr_modify = time2str(&(buf -> st_mtime), buf -> st_mtim.tv_nsec);
	char *timestr_change = time2str(&(buf -> st_ctime), buf -> st_ctim.tv_nsec);
	printf("Access: %s\n", timestr_access);
	printf("Modify: %s\n", timestr_modify);
	printf("Change: %s\n", timestr_change);
	printf(" Birth: -\n");
	free(timestr_access);
	free(timestr_modify);
	free(timestr_change);
}


int main(int argc, char * argv[]) {
	size_t i = 1;
	//step 1, check only one filename ===> commented by step 5
	// if(argc != 2) {
		// fprintf(stderr,"Please provide exactly ONE filename!\n");
		// return EXIT_FAILURE;
	// }
	if(argc < 2) {
		printf("mystat: missing operand\nTry to call Drew for more information.\n");
    }
	//step 5
	while(i < argc) {
		struct stat sb;
		//stat error, also ensure that sb is not empty so no need to check buf == null
		//in order to make it as similar as possible, I added several lines of codes 
		//to create a mimicking string of stat error message: "cannot blahblahblah".
		
		//create string first to ensure errno is not changed.
		char* cannot_string;
		cannot_string = malloc(strlen(argv[i])+strlen("stat: cannot stat '")+2);
		if (cannot_string == NULL) {
			fprintf(stderr, "Insufficient memory in main, cannot_string!\n");
			return EXIT_FAILURE;
		}
		cannot_string[0] = '\0';
		strcat(cannot_string, "stat: cannot stat '");
		strcat(cannot_string, argv[i]);
		strcat(cannot_string,"'\0");
		if (lstat(argv[i], &sb) == -1) {
			perror(cannot_string);
			//if error occurs, free it anyway even if it's not required.
			free(cannot_string);
			return EXIT_FAILURE;
		}
		//if no error occurs, free it.
		free(cannot_string);
		
		//create a pointer to point the struct stat sb, so the passing of parameter will require less memory
		struct stat *buf = &sb;
		step1_print_three_lines(argv[i], buf);
		//step 2 and 3
		step2and3_print_fourth_line_of_output(buf);
		//step 4
		step4_print_last_four_lines(buf);
		//step 6, mend step 1
		//step 7, mend step 1
		i++;
	}
	return EXIT_SUCCESS;
}




