#include <unistd.h>

void child2(int pipe_fd[]) {
    // Close the write end of the pipe
    close(pipe_fd[1]);

    // Replace stdin with the read end of the pipe
    dup2(pipe_fd[0],0);

    // Close the read end of the pipe
    close(pipe_fd[0]);

    // Replace the executable with "./gato"
    char *args[] = {"./gato", NULL};
    execvp(args[0], args);
}
