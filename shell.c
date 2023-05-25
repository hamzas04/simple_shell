#include "main.h"

/**
 * main - Entry point for the simple shell program
 *
 * Return: Always 0
 */
int main(int argc, char *argv[]) {
	char command[MAX_COMMAND_LENGTH];

	if (argc == 1) {
		// Interactive mode
		while (1) {
			printf("($) ");
			if (fgets(command, sizeof(command), stdin) == NULL) {
				// Handle end of file condition (Ctrl+D)
				printf("\n");
				break;
			}

			// Remove the trailing newline character
			command[strcspn(command, "\n")] = '\0';

			pid_t pid = fork();
			if (pid == -1) {
				// Fork error
				perror(argv[0]);
				exit(EXIT_FAILURE);
			} else if (pid == 0) {
				// Child process

				// Execute the command
				execve(command, (char *[]){command, NULL}, NULL);

				// If execve returns, it means the command was not found
				fprintf(stderr, "%s: 1: %s: not found\n", argv[0], command);
				_exit(EXIT_FAILURE);
			} else {
				// Parent process

				// Wait for the child process to finish
				int status;
				waitpid(pid, &status, 0);

				if (WIFEXITED(status)) {
					int exit_status = WEXITSTATUS(status);
					printf("Exit status: %d\n", exit_status);
				} else if (WIFSIGNALED(status)) {
					int signal_number = WTERMSIG(status);
					printf("Terminated by signal: %d\n", signal_number);
				}
			}
		}
	} else if (argc == 2) {
		// Non-interactive mode
		FILE *file = fopen(argv[1], "r");
		if (file == NULL) {
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}

		while (fgets(command, sizeof(command), file)) {
			// Remove the trailing newline character
			command[strcspn(command, "\n")] = '\0';

			pid_t pid = fork();
			if (pid == -1) {
				// Fork error
				perror(argv[0]);
				exit(EXIT_FAILURE);
			} else if (pid == 0) {
				// Child process

				// Execute the command
				execve(command, (char *[]){command, NULL}, NULL);

				// If execve returns, it means the command was not found
				fprintf(stderr, "%s: 1: %s: not found\n", argv[0], command);
				_exit(EXIT_FAILURE);
			} else {
				// Parent process

				// Wait for the child process to finish
				int status;
				waitpid(pid, &status, 0);

				if (WIFEXITED(status)) {
					int exit_status = WEXITSTATUS(status);
					printf("Exit status: %d\n", exit_status);
				} else if (WIFSIGNALED(status)) {
					int signal_number = WTERMSIG(status);
					printf("Terminated by signal: %d\n", signal_number);
				}
			}
		}

		fclose(file);
	} else {
		fprintf(stderr, "Usage: %s [script]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	return 0;
}
