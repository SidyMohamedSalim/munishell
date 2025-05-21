#include "cell.h"



//########################################################
//################## UTILS ###############################
//########################################################
int status = 0;


pid_t	Fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror(RED"Fork failed"RST);
		exit(EX_OSERR);
	}
	return (pid);
}

pid_t	Wait(int *status)
{
	pid_t	result;

	if (!status)
	{
		fprintf(stderr, RED"Wait: status argument required\n"RST);
		return (-1);
	}
	result = wait(status);
	if (result == -1)
		perror(RED"Wait failed"RST);
	if (WIFEXITED(*status))
		*status = WEXITSTATUS(*status); 
	return (result);
}


pid_t	Waitpid(pid_t pid, int *status, int options)
{
	pid_t	result;

	if (!status)
		return (-1);
	result = waitpid(pid, status, options);
	if (result == -1)
		perror(RED"Waitpid failed"RST);
	if (WIFEXITED(*status))
		*status = WEXITSTATUS(*status); 
	return (result);
}



void	Execvp(const char *file, char *const argv[])
{
	if (!file || !argv)
	{
		fprintf(stderr, RED"Execvp: invalid arguments\n"RST);
		exit(EXIT_FAILURE);
	}
	if (execvp(file, argv) == -1)
	{
		perror(RED"💥CELL_Jr failed💥"RST);
		exit(EX_UNAVAILABLE);
	}
}



char	*Getcwd(char *buf, size_t size)
{
	char	*result;

	result = getcwd(buf, size);
	if (!result)
		perror(RED"Getcwd failed"RST);
	return (result);
}


void printBanner(void) {
    p(G
        "         ____        _ _             ____  _          _ _ \n"
        "        / ___|  __ _| (_)_ __ ___   / ___|| |__   ___| | |\n"
        "        \\___ \\ / _` | | | '_ ` _ \\  \\___ \\| '_ \\ / _ | | |\n"
        "         ___) | (_| | | | | | | | |  ___) | | | |  __| | |\n"
        "        |____/ \\__,_|_|_|_| |_| |_| |____/|_| |_|\\___|_|_|\n"
        RST
    );
}

void	*Malloc(size_t size)
{
	void	*ptr;

	if (size == 0)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
	{
		perror(RED"Malloc failed"RST);
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

void	*Realloc(void *ptr, size_t size)
{
    void	*new_ptr;

    if (size == 0)
        return (NULL);
    new_ptr = realloc(ptr, size);
    if (!new_ptr)
    {
        perror(RED"Realloc failed"RST);
        exit(EXIT_FAILURE);
    }
    return (new_ptr);
}



//########################################################
//################## END UTILS ###############################
//########################################################


//########################################################
//################## BUILTINS ###############################
//########################################################
void	cell_launch(char **args)
{

	if (Fork() == CELL_JR)
		Execvp(args[0], args);
	else
		Wait(&status);
}

int cell_echo(char **av)
{
    int i = 1;

    if (av[1] == NULL)
        return (0);
    while (av[i])
    {
        p("%s ", av[i]);
        i++;
    }
    p("\n");
    return (0);
}


int cell_env(char **av)
{
    (void)av;
    extern char **environ;
    int i = 0;

    while (environ[i])
    {
        p("%s\n", environ[i]);
        i++;
    }
    return (0);
}

int cell_exit(char **av)
{
    (void)av;
    p(RED "Exiting shell...\n" RST);
    exit(EXIT_SUCCESS);
}
int cell_cd(char **av)
{
    (void)av;
    char cwd[BUFSIZ];
    char *path = NULL;

    path = Getcwd(cwd, sizeof(cwd));
    p("%s\n", path);
    return (0);
}


//########################################################
//################## END BUILTINS ###############################
//########################################################


t_builtin	g_builtin[] = 
{
	{.builtin_name = "echo", .foo=cell_echo},   /* Echo text to stdout */
	{.builtin_name = "env", .foo=cell_env},     /* Print environment */
	{.builtin_name = "exit", .foo=cell_exit},  /* Exit the shell */
	{.builtin_name = NULL},                    /* Sentinel */
};


void	cell_exec(char **args)
{

    int i ;
    const char *curr;
    i = 0;

    while ((curr = g_builtin[i].builtin_name))
    {
        if (strcmp(args[0], curr) == 0)
        {
            status = g_builtin[i].foo(args);
            return ;
        }
        i++;
    }   

    cell_launch(args);
    
}


char	**cell_split_line(char *line)
{
	size_t			bufsize;
	unsigned long	position;
	char			**tokens;

	bufsize = BUFSIZ;
	position = 0;
	tokens = Malloc(bufsize * sizeof *tokens);

   for (char *token = strtok(line, SPACE); token; token = strtok(NULL, SPACE))
	{
		tokens[position++] = token;	
		if (position >= bufsize)
		{
			bufsize *= 2;
			tokens = Realloc(tokens, bufsize * sizeof *tokens);
		}
	}
	tokens[position] = NULL;
	return (tokens);

}





char * cell_read_line(void) {
    char *buf;
    size_t len;
    char cwd[BUFSIZ];

    buf = NULL;

    Getcwd(cwd, sizeof(cwd));

    p(C "$ %s > " RST,cwd);

    // p("&> ");
    if (getline(&buf, &len, stdin) == -1) {
        free(buf);
        buf = NULL;
        if (feof(stdin)) 
            p(RED "EOF\n" RST);
        else 
            p(RED "getline error\n" RST);
        
    }

    return buf;
}


int main(int ac, char **av) {
    (void)ac;
    // int status;

    char *line;
    char **args;
    printBanner();


    while ((line = cell_read_line()))
    {
        p("%s\n", line);

        // 2 get the arguments
        args = cell_split_line(line);
        for (int i = 0; args[i]; i++)
            p("%s\n", args[i]);



        //  3 Execute the commands
        cell_exec(args);
        
        free(line);
        line = NULL;
        free(args);
        args = NULL;
    }
    

    return EXIT_SUCCESS;
}