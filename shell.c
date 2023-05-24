#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

/**
 * main - Entry point of the shell program
 *
 * Return: Always 0 (Success)
 */
int main() {
	char buffer[BUFFER_SIZE];
	char prompt[] = "#cisfun$ ";
	
	while (1) {
		printf("%s", prompt);
		fflush(stdout);

		if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
			printf("\n");
			break; // End of file condition (Ctrl+D)
		}
		
		buffer[strcspn(buffer, "\n")] = '\0';
		
		pid_t pid = fork();
		if (pid == 0) {
			// Child process
			execlp(buffer, buffer, NULL);
			
			printf("%s: command not found\n", buffer);
			exit(1);
		} else if (pid < 0) {
			printf("Fork failed\n");
			exit(1);
		} else {
			wait(NULL);
		}
	}
	
	return 0;
}
