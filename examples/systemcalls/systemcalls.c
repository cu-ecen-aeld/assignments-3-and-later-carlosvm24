#include "systemcalls.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
	bool ret = true;
	
	if (system(cmd) < 0)
	{
		ret = false;
		perror("system");
	}

	return ret;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    //command[count] = command[count];

    bool ret = false;
    int pid = fork();

    if (pid == 0) // Fork success: In Child Process
    {
	    if (execv(command[0], command) < 0)
	    {
		    printf("***ERROR: execv failed with return value -1\n");
		    perror("execv");
	    }
	    exit(1); // Only executed if execv fails
    }
    else if (pid > 0) // Parent Process: Wait for Child
    {
	    int status;
	    if (wait(&status) < 0)
	    {
		    printf("***ERROR: wait failed with return value -1\n");
		    perror("wait");
	    }
	    ret = (WEXITSTATUS(status) == 0); // status 0 means it was successful
    }
    else // Fork failed
    {
	    ret = false;
	    printf("***ERROR: fork failed with return value -1\n");
	    perror("fork");
    }

    va_end(args);
    
    return ret;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    //command[count] = command[count];

    bool ret = false;
    int fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0644);
    int pid = (fd < 0) ? -1 : fork();
    
    if (pid == 0) // Fork success: In Child Process
    {
	    if (dup2(fd, 1) < 0) // Duplicate file descriptor to stdout
	    {
		    printf("***ERROR: dup2 failed with return value -1\n");
		    perror("dup2");
	    }
	    if (execv(command[0], command) < 0)
	    {
		    printf("***ERROR: execv failed with return value -1\n");
		    perror("execv");
	    }
	    close(fd); // Closing file, no longer needed
	    exit(1); // Only executed if execv fails
    }
    else if (pid > 0) // Parent Process: Wait for Child
    {
	    int status;
	    close(fd); // Closing file, no longer needed
	    if (wait(&status) < 0)
	    {
		    printf("***ERROR: wait failed with return value -1\n");
		    perror("wait");
	    }
	    ret = (WEXITSTATUS(status) == 0); // status 0 means it was successful
    }
    else // Fork or Open failed
    {
	    if (fd < 0)
	    {
		    printf("***ERROR: open failed with return value -1\n");
		    perror("open");
	    }
	    else
	    {
		    printf("***ERROR: fork failed with return value -1\n");
		    perror("fork");
	    }
    }

    va_end(args);

    return ret;
}
