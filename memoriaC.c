#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    key_t key = 1234;
    int shmId;
    float a, b, c, *ap;
    int status, cpid, r;

    shmId = shmget(key, 3 * sizeof(float), 0666 | IPC_CREAT);

    if (shmId < 0)
    {
        perror("Error en shmget");
        exit(-1);
    }

    ap = (float *)shmat(shmId, 0, 0);
    if (ap < 0)
    {
        perror("Error en shmat");
        exit(-1);
    }

    *ap = 3.1415926535;

    pid = fork();

    if (pid < 0)
    {
        perror("Error en fork");
        exit(-1);
    }

    if (pid == 0)
    {
        *(ap + 1) = 0.707;
    }
    else
    {
        if (wait(&status) == pid)
        {
            printf("\n %f  %f ", *ap, *(ap + 1));
        }
    }

    r = shmdt(ap);

    if (r < 0)
    {
        perror("Error en shmdt");
        exit(-1);
    }

    return 0;
}