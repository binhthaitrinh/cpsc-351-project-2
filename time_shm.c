#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int status;
    struct timeval *ptr = (struct timeval *)malloc(sizeof(struct timeval));

    struct timeval start_time, end_time, elapsed_time;

    const char *name = "Shared memory";

    int fd;

    fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    ftruncate(fd, sizeof(struct timeval));

    ptr = (struct timeval *)mmap(0, sizeof(struct timeval), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    close(fd);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork error\n");
    }

    if (pid == 0)
    {
        struct timeval start_time;
        gettimeofday(&start_time, 0);

        memcpy(ptr, &start_time, sizeof(struct timeval));

        execvp(argv[1], &argv[1]);

        exit(0);
    }

    wait(&status);
    gettimeofday(&end_time, 0);

    memcpy(&start_time, ptr, sizeof(struct timeval));

    timersub(&end_time, &start_time, &elapsed_time);

    printf("Elapsed time: %ld.%06ld\n", elapsed_time.tv_sec, elapsed_time.tv_usec);

    return 0;
}