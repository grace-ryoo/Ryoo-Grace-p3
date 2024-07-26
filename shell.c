#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFSIZE 4096

/* Retrieve the hostname and make sure that this program is not being run on the main odin server.
 * It must be run on one of the vcf cluster nodes (vcf0 - vcf3).
 */
void check()
{
        char hostname[10];
        gethostname(hostname, 9);
        hostname[9] = '\0';
        if (strcmp(hostname, "csci-odin") == 0) {
                fprintf(stderr, "WARNING: TO MINIMIZE THE RISK OF FORK BOMBING THE ODIN SERVER,\nYOU MUST RUN THIS PROGRAM ON ONE OF THE VCF CLUSTER NODES!!!\n");
                exit(EXIT_FAILURE);
        } // if
} // check

/**
 *
 *
 */
int main()
{
	check();
	setbuf(stdout, NULL); // makes printf() unbuffered
	int n;
	char cmd[BUFFSIZE];
	
	// Project 3 TODO: set the current working directory to the user home directory upon initial launch of the shell
	// You may use getenv("HOME") to retrive the user home directory

	chdir(getenv("HOME"));

	// infinite loop that repeated prompts the user to enter a command
	while (1) {
		printf("1730sh:");
		// Project 3 TODO: display the current working directory as part of the prompt
		printf("$ ");
		n = read(STDIN_FILENO, cmd, BUFFSIZE);

		// if user enters a non-empty command
		if (n > 1) {
			cmd[n-1] = '\0'; // replaces the final '\n' character with '\0' to make a proper C string
			
			
			// Lab 06 TODO: parse/tokenize cmd by space to prepare the
			// command line argument array that is required by execvp().
			// For example, if cmd is "head -n 1 file.txt", then the 
			// command line argument array needs to be
			// ["head", "-n", "1", "file.txt", NULL].
			
			char *argv[BUFFSIZE / 2];
			int argc = 0;
			char *inputf = NULL;
			char *outputf = NULL;	
			char *token = strtok(cmd, " ");

			while (token != NULL && argc < BUFFSIZE / 2 - 1) {
				if (strcmp(token, "<") == 0) {
					inputf = strtok(NULL, " ");
				} else if (strcmp(token, ">") == 0) {
					outputf = strtok(NULL, " ");
				} else {
					argv[argc++] = token;
				} // if
				
				token = strtok(NULL, " ");

			} // while
			argv[argc] = NULL; // last item of argv set to NULL


			// Lab 07 TODO: if the command contains input/output direction operators
			// such as "head -n 1 < input.txt > output.txt", then the command
			// line argument array required by execvp() needs to be
			// ["head", "-n", "1", NULL], while the "< input.txt > output.txt" portion
			// needs to be parsed properly to be used with dup2(2) inside the child process

			
			
			// Lab 06 TODO: if the command is "exit", quit the program
			 
			if (strcmp(cmd, "exit") == 0) {
       				break;	
	//		} else if (strcmp(cmd, "cd") == 0) {		
			 
			// Project 3 TODO: else if the command is "cd", then use chdir(2) to
			// to support change directory functionalities
			
			
			
			// Lab 06 TODO: else, for all other commands, fork a child process and let
			// the child process execute user-specified command with its options/arguments.
			// NOTE: fork() only needs to be called once. DO NOT CALL fork() more than one time.
			} else {
				pid_t pid;
			
				if ((pid = fork()) < 0) { // error
					perror("fork");
					return EXIT_FAILURE;
				} else if (pid == 0) { // in child process
					if (inputf) {
						int fd = open(inputf, O_RDONLY);
						if (fd == -1) {
							perror("open");
							exit(EXIT_FAILURE);
						} // if

						if (dup2(fd, STDIN_FILENO) == -1) {
							perror("dup2");
							exit(EXIT_FAILURE);
						} // if
						close(fd);
					} // if
					
					if (outputf) {
						int fd = open(outputf, O_WRONLY | O_CREAT | O_TRUNC, 0644);
						if (fd == -1) {
							perror("open");
							exit(EXIT_FAILURE);
						} // if

						if (dup2(fd, STDOUT_FILENO) == -1) {
							perror("dup2");
							exit(EXIT_FAILURE);
						} // if
						close(fd);
					} // if

					
					if (execvp(argv[1], argv + 1) == -1) {
						perror("execvp");
						return EXIT_FAILURE;
					} // if
				} else { // in parent process
					int status;
					wait(&status);
				} // if
				
				
				// Lab 07 TODO: inside the child process, use dup2(2) to redirect
				// standard input and output as specified by the user command
				
				
				
				// Lab 06 TODO: inside the child process, invoke execvp().
				// if execvp() returns -1, be sure to use exit(EXIT_FAILURE);
				// to terminate the child process


				
				// Lab 06 TODO: inside the parent process, wait for the child process
				// You are not required to do anything special with the child's 
				// termination status
			} // if

		} // if
	} // while

	return 0;

} // main
