#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1

void inverteCaixa(char* str){
	int n = strlen(str);
	int i;
	for(i = 0; i < n; i++){
		if(str[i] >= 'a'){
			int aux = str[i] - 'a';
			str[i] = 'A' + aux;
		} else {
			int aux = str[i] - 'A';
			str[i] = 'a' + aux;
		}
	}
}

int main(void){
	char write_msg[BUFFER_SIZE] = "Greetings";
	char read_msg[BUFFER_SIZE];
	pid_t pid;
	int fd[2], fd2[2];

	if (pipe(fd) == -1){
		fprintf(stderr,"Pipe failed");
		return 1;
	}

	if(pipe(fd2) == -1){
		fprintf(stderr, "Second Pipe failed");
	}

	pid = fork();

	if (pid < 0){
		fprintf(stderr, "Fork failed");
		return 1;
	}

	if (pid > 0){
		close(fd[READ_END]);

		write(fd[WRITE_END], write_msg, strlen(write_msg)+1); 

		close(fd[WRITE_END]);

		wait(NULL); 

		close(fd2[WRITE_END]);

		read(fd2[READ_END], read_msg, BUFFER_SIZE);

		printf("parent read: %s\n", read_msg);
		close(fd2[READ_END]);
	} else { 
		close(fd[WRITE_END]);

		read(fd[READ_END], read_msg, BUFFER_SIZE);
		printf("child read: %s\n",read_msg);

		close(fd[READ_END]);

		inverteCaixa(read_msg);
		
		write(fd2[WRITE_END], read_msg, strlen(read_msg)+1); 

		close(fd2[READ_END]);
		close(fd2[WRITE_END]);
		exit(0);
	}

	return 0;
}
