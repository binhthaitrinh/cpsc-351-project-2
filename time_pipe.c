#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096
#define READ_END 0
#define WRITE_END 1

int main(int argc, char **argv)
{
    struct timeval start_time, end_time, elapsed_time;

    struct timeval *ptr = (struct timeval *)malloc(sizeof(struct timeval));

    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("pipe failed\n");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
    }

    if (pid == 0)
    {
        struct timeval start_time;
        gettimeofday(&start_time, 0);
        close(fd[READ_END]);
        write(fd[WRITE_END], &start_time, sizeof(struct timeval));

        close(fd[WRITE_END]);
        execvp(argv[1], &argv[1]);

        exit(0);
    }

    wait(NULL);
    close(fd[WRITE_END]);
    read(fd[READ_END], ptr, sizeof(struct timeval));
    gettimeofday(&end_time, 0);
    timersub(&end_time, ptr, &elapsed_time);

    printf("Elapsed time: %ld.%06ld\n", elapsed_time.tv_sec, elapsed_time.tv_usec);
    return 0;
}