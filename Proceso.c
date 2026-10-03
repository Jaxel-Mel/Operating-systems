#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define N_ITERACIONES 100000
#define FICHERO_SALIDA "salida_intercalada.txt"

int  abrir_fichero(const char *nombre);
void proceso_hijo(int fd);
void proceso_padre(int fd);
void escribir_numero(int fd, const char *etiqueta, int numero);

int main(){
    int   fd;
    pid_t pid;
    fd = abrir_fichero(FICHERO_SALIDA);
    if (fd == -1)
        return -1;
    pid = fork();
    if (pid == 0)
        proceso_hijo(fd);
    else if (pid > 0)
        proceso_padre(fd);
    else {
        perror("fork");
        return -1;
    }
    close(fd);
    return 0;
}

int abrir_fichero(const char *nombre){
    int fd;
    fd = open(nombre,
              O_WRONLY | O_CREAT | O_TRUNC | O_APPEND,
              0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    return fd;
}

void proceso_hijo(int fd){
    int i;
    for (i = N_ITERACIONES; i >= 1; i--)
        escribir_numero(fd, "HIJO  :", i);
}

void proceso_padre(int fd){
    int i;
    for (i = 1; i <= N_ITERACIONES; i++)
        escribir_numero(fd, "PADRE :", i);
    wait(NULL);
}

void escribir_numero(int fd, const char *etiqueta, int numero){
    char buffer[64];
    int  nbytes;
    nbytes = sprintf(buffer, "%s %d\n", etiqueta, numero);
    write(fd, buffer, nbytes);
}
