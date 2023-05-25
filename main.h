#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 1024

void display_prompt(void);
void execute_command(char *command);
void handle_ctrl_d(void);

#endif /* _MAIN_H_ */
