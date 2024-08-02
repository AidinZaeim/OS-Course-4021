#include "shell.h"
#include <unistd.h>
#include <pthread.h>

void exec_process(char *args)
{
    char *arguments = (char **)args;
	int id = fork();
	if (id < 0) {
		
	} else if (id == 0) {
		printf("%s\n",arguments[0]);
		execvp(arguments[0], arguments);
	    perror("execvp");
	} else {
		int *status;
		waitpid(id,status,WUNTRACED);
	}
    
}


int execute_args(char **args)
{
	pid_t pid;
	int status;
	char *builtin_func_list[] = {
		// you should add something here
		"exit",
		"cd",
	};
	int (*builtin_func[])(char **) = {
		// you should add something here
		&own_exit,
		&own_cd,
	};

	long unsigned int i = 0;
	for (; i < sizeof(builtin_func_list) / sizeof(char *); i++)
	{
		/* if there is a match execute the builtin command */
		if (strcmp(args[0], builtin_func_list[i]) == 0)
		{
			return ((*builtin_func[i])(args));
		}
	}

	// TODO
	
	// execvp(args[0], args);
	// perror("execvp");
	int id = fork();
	if (id < 0) {
		
	} else if (id == 0) {
		execvp(args[0], args);
	    perror("execvp");
	} else {
		int *status;
		waitpid(id,status,WUNTRACED);
	}
    


    // exec_process(args);

	return (-1); // this status code means everything is ok
}