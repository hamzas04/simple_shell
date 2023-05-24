#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 100

/**
 * main - Entry point of the shell program
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	char command[MAX_COMMAND_LENGTH];
	char prompt[] = "#cisfun$ ";

	while (1)
	{
		printf("%s", prompt);

		if (fgets(command, sizeof(command), stdin) == NULL)
		{
			if (feof(stdin))
			{
				printf("\n");
				break;
			}
			else
			{
				perror("fgets");
				exit(EXIT_FAILURE);
			}
		}

		command[strcspn(command, "\n")] = '\0';

		char *token = strtok(command, " ");
		if (token == NULL)
		{
			continue;
		}

		pid_t pid = fork();

		if (pid < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) // Child process
		{
			if (execlp(token, token, (char *)NULL) == -1)
			{
				perror("execlp");
				exit(EXIT_FAILURE);
			}
		}
		else // Parent process
		{
			int status;
			waitpid(pid, &status, 0);
			printf("\n");
		}

		if (access(token, F_OK) == -1)
		{
			printf("Command '%s' not found\n", token);
		}
	}

	return 0;
}
