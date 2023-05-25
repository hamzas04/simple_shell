#include "main.h"

/**
 * display_prompt - Displays the shell prompt
 */
void display_prompt(void)
{
	printf("#cisfun$ ");
}

/**
 * execute_command - Executes the given command
 * @command: The command to be executed
 */
void execute_command(char *command)
{
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("fork");
	}
	else if (pid == 0)
	{
		execlp(command, command, NULL);
		perror(command);
		exit(1);
	}
	else
	{
		wait(NULL);
	}
}

/**
 * handle_ctrl_d - Handles the Ctrl+D signal
 */
void handle_ctrl_d(void)
{
	printf("\n");
}

/**
 * main - Entry point of the shell program
 *
 * Return: Always 0
 */
int main(void)
{
	char command[MAX_COMMAND_LENGTH];

	while (1)
	{
		display_prompt();

		if (fgets(command, sizeof(command), stdin) == NULL)
		{
			handle_ctrl_d();
			break;
		}

		command[strcspn(command, "\n")] = '\0';

		if (strlen(command) == 0)
		{
			continue;
		}

		execute_command(command);
	}

	return 0;
}
