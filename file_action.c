#include "main.h"

int cant_open(char *file_path);
int proc_file_commands(char *file_path, int *exe_ret);

/**
 * cant_open - If the file doesn't exist or lacks proper permissions, print
 * a cant open error.
 * @file_path: Path to the supposed file.
 *
 * Return: 127.
 */

int cant_open(char *file_path)
{
	char *hist_str,*erreur;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (127);

	len = _strlen(name) + _strlen(hist_str) + _strlen(file_path) + 16;
	erreur = malloc(sizeof(char) * (len + 1));
	if (!erreur)
	{
		free(hist_str);
		return (127);
	}

	sprintf(erreur, "%s: %s: Can't open %s\n", name, hist_str, file_path);
	free(hist_str);

	write(STDERR_FILENO, erreur, len);
	free(erreur);

	return (127);

}

/**
 * proc_file_commands - Takes a file and attempts to run the commands stored
 * within.
 * @file_path: Path to the file.
 * @exe_ret: Return value of the last executed command.
 *
 * Return: If file couldn't be opened - 127.
 *	   If malloc fails - -1.
 *	   Otherwise the return value of the last command ran.
 */
int proc_file_commands(char *file_path, int *exe_ret)
{
	unsigned int line_size = 0;
	char *line = NULL;
	FILE *fp;
	int ret, i;
	ssize_t b_read;
	char **args; 
	
	hist = 0;
	fp = fopen(file_path, "r");
	if (!fp)
	{
		*exe_ret = cant_open(file_path);
		return (*exe_ret);
	}

	do {
		char *temp = NULL;
		size_t temp_size = 0;

		b_read = getline(&temp, &temp_size, fp);
		if (b_read == -1 && line_size == 0)
		{
			free(temp);
			return (*exe_ret);
		}

		line = _realloc(line, line_size, line_size + b_read + 1);
		_strcpy(line + line_size, temp);
		line_size += b_read;
		free(temp);

	} while (b_read != -1);

	fclose(fp);

	for (i = 0; i < (int)line_size; i++)
	{
		if (line[i] == '\n')
		{
			line[i] = ';';
			while (line[i + 1] == '\n')
				line[i++] = ' ';
		}
	}

	variable_replacement(&line, exe_ret);
	handle_line(&line, line_size);


	args = _strtok(line, " ");
	free(line);

	if (!args)
		return (0);

	if (check_args(args) != 0)
	{
		*exe_ret = 2;
		free_args(args, args);
		return (*exe_ret);
	}

	for (i = 0; args[i]; i++)
	{
		if (_strcmp(args[i], ";") == 0)
		{
			args[i] = NULL;
			ret = call_args(args, args, exe_ret);
			args = &args[i + 1];
			i = -1;
		}
	}

	ret = call_args(args, args, exe_ret);

	free_args(args, args);
	return (ret);
}
