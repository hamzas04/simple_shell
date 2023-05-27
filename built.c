#include "main.h"

int shellby_alias(char **args, char __attribute__((__unused__)) **front);
void set_alias(char *var_name, char *value);
void print_alias(alias_t *alias);

void aid_all(void);
void aid_alias(void);
void aid_cd(void);
void aid_exit(void);
void aid_help(void);

void aid_env(void);
void aid_setenv(void);
void aid_unsetenv(void);
void aid_history(void);

/**
 * shellby_alias - Builtin command that either prints all aliases, specific
 * aliases, or sets an alias.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int shellby_alias(char **args, char __attribute__((__unused__)) **front)
{
	alias_t *temp = aliases;
	int i,ret = 0;
	char *value;
	
	if (!args[0])
	{
		while (temp)
		{
			print_alias(temp);
			temp = temp->next;
		}
		return (ret);
	}

	for (i = 0; args[i]; i++)
	{
		temp = aliases;
		value = _strchr(args[i], '=');

		if (!value)
		{
			while (temp)
			{
				if (_strcmp(args[i], temp->name) == 0)
				{
					print_alias(temp);
					break;
				}
				temp = temp->next;
			}

			if (!temp)
				ret = create_erreur(args + i, 1);
		}
		else
			set_alias(args[i], value);
	}

	return (ret);

}

/**
 * set_alias - Will either set an existing alias 'name' with a new value,
 * 'value' or creates a new alias with 'name' and 'value'.
 * @var_name: Name of the alias.
 * @value: Value of the alias. First character is a '='.
 */
void set_alias(char *var_name, char *value)
{
	alias_t *temp = aliases;
	int j,len = 0, k = 0;
	char *new_value = NULL;

	*value = '\0';
	value++;

	len = _strlen(value);
	new_value = malloc(sizeof(char) * (len + 1));
	if (!new_value)
		return;

	for (j = 0; value[j]; j++)
	{
		if (value[j] != '\'' && value[j] != '"')
			new_value[k++] = value[j];
	}

	new_value[k] = '\0';

	while (temp)
	{
		if (_strcmp(var_name, temp->name) == 0)
		{
			free(temp->value);
			temp->value = new_value;
			break;
		}
		temp = temp->next;
	}

	if (!temp)
		add_alias_end(&aliases, var_name, new_value);

}

/**
 * print_alias - Prints the alias in the format name='value'.
 * @alias: Pointer to an alias.
 */
void print_alias(alias_t *alias)
{
	int len = _strlen(alias->name) + _strlen(alias->value) + 5;

	char *alias_string = malloc(sizeof(char) * len);
	if (!alias_string)
		return;

	sprintf(alias_string, "%s='%s'\n", alias->name, alias->value);

	write(STDOUT_FILENO, alias_string, len - 1);

	free(alias_string);

}
/**
 * replace_aliases - Goes through the arguments and replace any matching alias
 * with their value.
 * @args: 2D pointer to the arguments.
 *
 * Return: 2D pointer to the arguments.
 */
char **replace_aliases(char **args)
{
	alias_t *temp;
	int i;
	char *new_value;

	if (_strcmp(args[0], "alias") == 0)
		return (args);

	for (i = 0; args[i]; i++)
	{
		temp = aliases;
		while (temp)
		{
			if (_strcmp(args[i], temp->name) == 0)
			{
				size_t value_len = _strlen(temp->value);
				new_value = malloc(sizeof(char) * (value_len + 1));
				if (!new_value)
				{
					free_args(args, args);
					return NULL;
				}
				memcpy(new_value, temp->value, value_len + 1);
				free(args[i]);
				args[i] = new_value;
				break;
			}
			temp = temp->next;
		}
	}

	return (args);

}
/**
 * aid_all - Displays all possible builtin shellby commands.
 */
void aid_all(void)
{
	const char *msg1 = "Shellby\nThese shell commands are defined internally.\n";
	const char *msg2 = "Type 'help' to see this list.\nType 'help name' to find ";
	const char *msg3 = "out more about the function 'name'.\n\n  alias   \t";
	const char *msg4 = "alias [NAME[='VALUE'] ...]\n  cd    \tcd   ";
	const char *msg5 = "[DIRECTORY]\n  exit    \texit [STATUS]\n  env     \tenv";
	const char *msg6 = "\n  setenv  \tsetenv [VARIABLE] [VALUE]\n  unsetenv\t";
	const char *msg7 = "unsetenv [VARIABLE]\n";

	write(STDOUT_FILENO, msg1, strlen(msg1));
	write(STDOUT_FILENO, msg2, strlen(msg2));
	write(STDOUT_FILENO, msg3, strlen(msg3));
	write(STDOUT_FILENO, msg4, strlen(msg4));
	write(STDOUT_FILENO, msg5, strlen(msg5));
	write(STDOUT_FILENO, msg6, strlen(msg6));
	write(STDOUT_FILENO, msg7, strlen(msg7));

}

/**
 * aid_alias - Displays information on the shellby builtin command 'alias'.
 */
void aid_alias(void)
{
	const char *msg1 = "alias: alias [NAME[='VALUE'] ...]\n\tHandles aliases.\n";
	const char *msg2 = "\n\talias: Prints a list of all aliases, one per line, in ";
	const char *msg3 = "the format NAME='VALUE'.\n\talias name [name2 ...]:prints";
	const char *msg4 = " the aliases name, name2, etc. one per line, in the ";
	const char *msg5 = "form NAME='VALUE'.\n\talias NAME='VALUE' [...]: Defines";
	const char *msg6 = " an alias for each NAME whose VALUE is given. If NAME ";
	const char *msg7 = "is already an alias, replace its value with VALUE.\n";

	write(STDOUT_FILENO, msg1, strlen(msg1));
	write(STDOUT_FILENO, msg2, strlen(msg2));
	write(STDOUT_FILENO, msg3, strlen(msg3));
	write(STDOUT_FILENO, msg4, strlen(msg4));
	write(STDOUT_FILENO, msg5, strlen(msg5));
	write(STDOUT_FILENO, msg6, strlen(msg6));
	write(STDOUT_FILENO, msg7, strlen(msg7));

}

/**
 * aid_cd - Displays information on the shellby builtin command 'cd'.
 */
void aid_cd(void)
{
	const char *msg1 = "cd: cd [DIRECTORY]\n\tChanges the current directory of the";
	const char *msg2 = " process to DIRECTORY.\n\n\tIf no argument is given, the ";
	const char *msg3 = "command is interpreted as cd $HOME. If the argument '-' is";
	const char *msg4 = " given, the command is interpreted as cd $OLDPWD.\n\n";
	const char *msg5 = "\tThe environment variables PWD and OLDPWD are updated ";
	const char *msg6 = "after a change of directory.\n";

	write(STDOUT_FILENO, msg1, strlen(msg1));
	write(STDOUT_FILENO, msg2, strlen(msg2));
	write(STDOUT_FILENO, msg3, strlen(msg3));
	write(STDOUT_FILENO, msg4, strlen(msg4));
	write(STDOUT_FILENO, msg5, strlen(msg5));
	write(STDOUT_FILENO, msg6, strlen(msg6));

}

/**
 * aid_exit - Displays information on the shellby builtin command 'exit'.
 */
void aid_exit(void)
{
	const char *msg1 = "exit: exit [STATUS]\n\tExits the shell.\n\n\tThe ";
	const char *msg2 = "STATUS argument is the integer used to exit the shell.";
	const char *msg3 = " If no argument is given, the command is interpreted as";
	const char *msg4 = " exit 0.\n";

	write(STDOUT_FILENO, msg1, strlen(msg1));
	write(STDOUT_FILENO, msg2, strlen(msg2));
	write(STDOUT_FILENO, msg3, strlen(msg3));
	write(STDOUT_FILENO, msg4, strlen(msg4));

}

/**
 * aid_help - Displays information on the shellby builtin command 'help'.
 */
void aid_help(void)
{
	const char *msg1 = "help: help\n\tSee all possible Shellby builtin commands.\n";
	const char *msg2 = "\n      help [BUILTIN NAME]\n\tSee specific information on each ";
	const char *msg3 = "builtin command.\n";

	write(STDOUT_FILENO, msg1, strlen(msg1));
	write(STDOUT_FILENO, msg2, strlen(msg2));
	write(STDOUT_FILENO, msg3, strlen(msg3));

}
/**
 * aid_env - Displays information on the shellby builtin command 'env'.
 */
void aid_env(void)
{
	const char *msg = "env: env\n\tPrints the current environment.\n";

	write(STDOUT_FILENO, msg, strlen(msg));

}

/**
 * aid_setenv - Displays information on the shellby builtin command 'setenv'.
 */
void aid_setenv(void)
{
	const char *msg1 = "setenv: setenv [VARIABLE] [VALUE]\n\tInitializes a new";
	const char *msg2 = "environment variable, or modifies an existing one.\n\n";
	const char *msg3 = "\tUpon failure, prints a message to stderr.\n";

	write(STDOUT_FILENO, msg1, strlen(msg1));
	write(STDOUT_FILENO, msg2, strlen(msg2));
	write(STDOUT_FILENO, msg3, strlen(msg3));

}

/**
 * aid_unsetenv - Displays information on the shellby builtin command
 * 'unsetenv'.
 */
void aid_unsetenv(void)
{
	const char *msg1 = "unsetenv: unsetenv [VARIABLE]\n\tRemoves an ";
	const char *msg2 = "environmental variable.\n\n\tUpon failure, prints a ";
	const char *msg3 = "message to stderr.\n";

	write(STDOUT_FILENO, msg1, strlen(msg1));
	write(STDOUT_FILENO, msg2, strlen(msg2));
	write(STDOUT_FILENO, msg3, strlen(msg3));

}
