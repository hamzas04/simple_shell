#include "main.h"
/**
 * main - Entry point for the simple shell program
 *
 * Return: Always 0
 */
int main(void)
{
	char buffer[BUFFER_SIZE];
	pid_t child_pid;
	int status;

	while (1)
	{
		printf("#cisfun$ ");
		fflush(stdout);

		if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
		{
			printf("\n");
			break;
		}

		buffer[strcspn(buffer, "\n")] = '\0';

		child_pid = fork();

		if (child_pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}

		if (child_pid == 0)
		{
			if (execlp(buffer, buffer, (char *)NULL) == -1)
			{
				perror(buffer);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			waitpid(child_pid, &status, 0);

			if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
				continue;
			else
				printf("%s: Command not found\n", buffer);
		}
	}

	return 0;
}
