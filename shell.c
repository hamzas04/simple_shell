#include "main.h"

/**
 * main - Entry point for the simple shell program
 *
 * Return: Always 0
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
			printf("\n");
			break;
		}

		command[strcspn(command, "\n")] = '\0';

		pid_t pid = fork();

		if (pid < 0)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			execlp(command, command, NULL);
			printf("%s: No such file or directory\n", command);
			exit(EXIT_FAILURE);
		}
		else
		{
			wait(NULL);
		}
	}

	return 0;
}
