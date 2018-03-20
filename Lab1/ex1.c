#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main (){
	pid_t child_pid;

	child_pid = fork ();
	if (child_pid > 0) {
	  	printf("parent process, pid = %i\n", getpid());
	    sleep (20);
		exit(0);
	}
	else {
	  	printf("child process, pid = %i\n", getpid());
	    exit (0);
	}
	return 0;
}

