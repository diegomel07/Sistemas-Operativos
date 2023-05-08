#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    pid_t id = fork();
    if (id == 0)
    {
        printf("Hola desde el proceso hijo, pid: %d\n", getpid());
    } else 
    {
        printf("Hola desde el proceso padre, pid: %d\n", getpid());
    }
    
    printf("Hola desde ambos\n");
}
