#include "shell.h"

/**
 * own_cd - changes the working dir of the current shell executon env
 * @args: target directory
 *
 * Return: -1 one success, 0 otherwise.
 */
int own_cd(char **args)
{
	// TODO
	 if (args[1] == NULL) {
        fprintf(stderr, "own_cd: expected argument to \"cd\"\n");
        return -1;
    } else {
        if (chdir(args[1]) != 0) {
            perror("own_cd");
            return -1;
        }
    }

	return (-1);
}
