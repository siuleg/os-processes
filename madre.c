#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void child1(int *pipefd) {
  close(pipefd[0]); // Close read end

  // Redirect stdout to the write end of the pipe
  dup2(pipefd[1], STDOUT_FILENO);
  close(pipefd[1]); // Close write end

  // Replace executable with "./taco"
  char *args[] = {"./taco", NULL};
  execvp(args[0], args);
}

void child2(int pipe_fd[]) {
  // Close the write end of the pipe
  close(pipe_fd[1]);

  // Replace stdin with the read end of the pipe
  dup2(pipe_fd[0], 0);

  // Close the read end of the pipe
  close(pipe_fd[0]);

  // Replace the executable with "./gato"
  char *args[] = {"./gato", NULL};
  execvp(args[0], args);
}

int main() {
  int pipefd[2];
  if (pipe(pipefd) < 0) {
    return 1;
  }

  pid_t child1_pid, child2_pid;

  // Fork off child 1
  if ((child1_pid = fork()) == -1) {
    return 1;
  }

  if (child1_pid == 0) {
    // Child 1
    child1(pipefd);
  }

  // Fork off child 2
  if ((child2_pid = fork()) < 0) {
    return 1;
  }

  if (child2_pid == 0) {
    // Child 2
    child2(pipefd);
  }

  // Close both ends of the pipe in the parent
  close(pipefd[0]);
  close(pipefd[1]);

  // Wait for both children to finish
  waitpid(child1_pid, NULL, 0);
  waitpid(child2_pid, NULL, 0);

  return 0;
}
