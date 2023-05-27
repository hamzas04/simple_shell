#include "main.h"

int count_digit_length(int number);
char *_itoa(int number);
int create_erreur(char **args, int err);

char *erreur_env(char **args);
char *erreur_1(char **args);
char *erreur_2_exit(char **args);
char *erreur_2_cd(char **args);
char *erreur_2_syntax(char **args);

char *erreur_126(char **args);
char *erreur_127(char **args);

/**
 * count_digit_length - Counts the digit lengthgth of a number.
 * @number: The number to measure.
 *
 * Return: The digit lengthgth.
 */
int count_digit_length(int number)
{
    int length = 1;

    if (number < 0) {
        length++;
        number = -number;
    }

    while (number != 0) {
        number /= 10;
        length++;
    }

    return (length);
}

/**
 * _itoa - Converts an integer to a string.
 * @number: The integer.
 *
 * Return: The converted string.
 */
char *_itoa(int number)
{
	char *buffer;
	int length = count_digit_length(number);
	unsigned int number1;

	buffer = malloc(sizeof(char) * (length + 1));
	if (!buffer)
		return (NULL);

	buffer[length] = '\0';

	if (number < 0)
	{
		number1 = number * -1;
		buffer[0] = '-';
	}
	else
	{
		number1 = number;
	}

	length--;
	do {
		buffer[length] = (number1 % 10) + '0';
		number1 /= 10;
		length--;
	} while (number1 > 0);

	return (buffer);
}


/**
 * create_erreur - Writes a custom erreur message to stderr.
 * @args: An array of arguments.
 * @err: The erreur value.
 *
 * Return: The erreur value.
 */
int create_erreur(char **args, int err)
{
	char *erreur;

	switch (err)
	{
	case -1:
		erreur = erreur_env(args);
		break;
	case 1:
		erreur = erreur_1(args);
		break;
	case 2:
		if (*(args[0]) == 'e')
			erreur = erreur_2_exit(++args);
		else if (args[0][0] == ';' || args[0][0] == '&' || args[0][0] == '|')
			erreur = erreur_2_syntax(args);
		else
			erreur = erreur_2_cd(args);
		break;
	case 126:
		erreur = erreur_126(args);
		break;
	case 127:
		erreur = erreur_127(args);
		break;
	}
	write(STDERR_FILENO, erreur, _strlen(erreur));

	if (erreur)
		free(erreur);
	return (err);

}
/**
 * erreur_env - Creates an erreur message for shellby_env erreurs.
 * @args: An array of arguments passed to the command.
 *
 * Return: The erreur string.
 */
char *erreur_env(char **args)
{
	char *erreur, *hist_str;
	int length;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	args--;
	length = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 45;
	erreur = malloc(sizeof(char) * (length + 1));
	if (!erreur)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(erreur, name);
	_strcat(erreur, ": ");
	_strcat(erreur, hist_str);
	_strcat(erreur, ": ");
	_strcat(erreur, args[0]);
	_strcat(erreur, ": Unable to add/remove from environment\n");

	free(hist_str);
	return (erreur);
}

/**
 * erreur_1 - Creates an erreur message for shellby_alias erreurs.
 * @args: An array of arguments passed to the command.
 *
 * Return: The erreur string.
 */
char *erreur_1(char **args)
{
	char *erreur;
	int length;

	length = _strlen(name) + _strlen(args[0]) + 13;
	erreur = malloc(sizeof(char) * (length + 1));
	if (!erreur)
		return (NULL);

	_strcpy(erreur, "alias: ");
	_strcat(erreur, args[0]);
	_strcat(erreur, " not found\n");

	return (erreur);
}

/**
 * erreur_2_exit - Creates an erreur message for shellby_exit erreurs.
 * @args: An array of arguments passed to the command.
 *
 * Return: The erreur string.
 */
char *erreur_2_exit(char **args)
{
	char *erreur, *hist_str;
	int length;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	length = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 27;
	erreur = malloc(sizeof(char) * (length + 1));
	if (!erreur)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(erreur, name);
	_strcat(erreur, ": ");
	_strcat(erreur, hist_str);
	_strcat(erreur, ": exit: Illegal number: ");
	_strcat(erreur, args[0]);
	_strcat(erreur, "\n");

	free(hist_str);
	return (erreur);
}

/**
 * erreur_2_cd - Creates an erreur message for shellby_cd erreurs.
 * @args: An array of arguments passed to the command.
 *
 * Return: The erreur string.
 */
char *erreur_2_cd(char **args)
{
	char *erreur, *hist_str;
	int length;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	if (args[0][0] == '-')
		args[0][2] = '\0';
	length = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 24;
	erreur = malloc(sizeof(char) * (length + 1));
	if (!erreur)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(erreur, name);
	_strcat(erreur, ": ");
	_strcat(erreur, hist_str);
	if (args[0][0] == '-')
		_strcat(erreur, ": cd: Illegal option ");
	else
		_strcat(erreur, ": cd: can't cd to ");
	_strcat(erreur, args[0]);
	_strcat(erreur, "\n");

	free(hist_str);
	return (erreur);
}

/**
 * erreur_2_syntax - Creates an erreur message for syntax erreurs.
 * @args: An array of arguments passed to the command.
 *
 * Return: The erreur string.
 */
char *erreur_2_syntax(char **args)
{
	char *erreur, *hist_str;
	int length;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	length = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 33;
	erreur = malloc(sizeof(char) * (length + 1));
	if (!erreur)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(erreur, name);
	_strcat(erreur, ": ");
	_strcat(erreur, hist_str);
	_strcat(erreur, ": Syntax erreur: \"");
	_strcat(erreur, args[0]);
	_strcat(erreur, "\" unexpected\n");

	free(hist_str);
	return (erreur);
}
/**
 * erreur_126 - Creates an erreur message for permission denied failures.
 * @args: An array of arguments passed to the command.
 *
 * Return: The erreur string.
 */
char *erreur_126(char **args)
{
	char *erreur, *hist_str;
	int length;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	length = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 24;
	erreur = malloc(sizeof(char) * (length + 1));
	if (!erreur)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(erreur, name);
	_strcat(erreur, ": ");
	_strcat(erreur, hist_str);
	_strcat(erreur, ": ");
	_strcat(erreur, args[0]);
	_strcat(erreur, ": Permission denied\n");

	free(hist_str);
	return (erreur);
}

/**
 * erreur_127 - Creates an erreur message for command not found failures.
 * @args: An array of arguments passed to the command.
 *
 * Return: The erreur string.
 */
char *erreur_127(char **args)
{
	char *erreur, *hist_str;
	int length;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	length = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 16;
	erreur = malloc(sizeof(char) * (length + 1));
	if (!erreur)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(erreur, name);
	_strcat(erreur, ": ");
	_strcat(erreur, hist_str);
	_strcat(erreur, ": ");
	_strcat(erreur, args[0]);
	_strcat(erreur, ": not found\n");

	free(hist_str);
	return (erreur);
}
