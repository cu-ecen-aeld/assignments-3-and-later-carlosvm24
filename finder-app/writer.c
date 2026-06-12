
#include <stdio.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
	openlog(NULL,0,LOG_USER);
	
	if (3 != argc)
	{
		syslog(LOG_ERR, "Invalid Number of Arguments: %d", argc);
		return 1;
	}
	
	FILE *fptr = fopen(argv[1], "w");

	if (NULL == fptr)
	{
		syslog(LOG_ERR, "%s file could not be created", argv[1]);
		return 1;
	}

	syslog(LOG_DEBUG, "Writing %s to %s file", argv[2], argv[1]);
	fprintf(fptr, "%s", argv[2]);

	fclose(fptr);
	closelog();
	return 0;
}

