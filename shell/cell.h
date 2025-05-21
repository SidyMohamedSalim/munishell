#ifndef SHELL_CELL_H
#define SHELL_CELL_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <string.h>
#include <errno.h>
#include <sysexits.h>
#include <stdbool.h>


/*
** ANSI Color codes for terminal output formatting:
** Y    - Yellow
** G    - Green
** C    - Cyan
** RED  - Red
** RST  - Reset to default color
*/
#define Y		"\033[1;33m"
#define G		"\033[1;32m"
#define C 		"\033[1;36m"
#define RED		"\033[1;31m"
#define RST 	"\033[0m"


#define p(...)	printf(__VA_ARGS__)
#define SPACE "\t\n\v\f\r "
#define CELL_JR	0

typedef struct s_builtin
{
    const char *builtin_name;
	int (*foo)(char **av);
} t_builtin;


enum{
	OK,     /* Operation succeeded */
	ERROR   /* Operation failed */
};

char	*Getcwd(char *buf, size_t size);


#endif