#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    int fd;
    ssize_t bytes_written;
    const char *writefile;
    const char *writestr;

    openlog("writer", 0, LOG_USER);

    if (argc != 3)
    {
        syslog(LOG_ERR, "Invalid number of arguments");
        fprintf(stderr, "Usage: %s <file> <string>\n", argv[0]);
        closelog();
        return 1;
    }

    writefile = argv[1];
    writestr = argv[2];

    syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);

    fd = open(writefile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        syslog(LOG_ERR, "Error opening file %s: %s",
               writefile, strerror(errno));
        closelog();
        return 1;
    }

    bytes_written = write(fd, writestr, strlen(writestr));
    if (bytes_written == -1)
    {
        syslog(LOG_ERR, "Error writing to file %s: %s",
               writefile, strerror(errno));
        close(fd);
        closelog();
        return 1;
    }

    close(fd);
    closelog();

    return 0;
}