#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int fd[2];
    pid_t pid1, pid2;
    char buffer[100];

    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(1);
    }

    pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid1 == 0)
    {
        // Este es el proceso hijo 1
        close(fd[0]); // Cierra la lectura
        sprintf(buffer, "Hola, soy el proceso hijo 1");
        write(fd[1], buffer, sizeof(buffer));
        exit(0);
    }

    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid2 == 0)
    {
        // Este es el proceso hijo 2
        close(fd[1]); // Cierra la escritura
        read(fd[0], buffer, sizeof(buffer));
        printf("Proceso hijo 2 recibió: %s\n", buffer);
        exit(0);
    }

    // El proceso padre cierra ambos extremos de la tubería y espera que los
    // procesos hijos terminen
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
