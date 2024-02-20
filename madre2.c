#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char *argv[])
{

    int pipe_fds[2];
    int result = pipe(pipe_fds);

    char pipe_error[] = "Error creating pipe\n";
    if (result < 0)
    {
        write(2, pipe_error, sizeof(pipe_error));
        return 1;
    }

    char fork_error[] = "Error forking off a child\n";
    char fork_success[] = "I am a newly created child\n";
    pid_t pid = fork();

    if (pid < 0)
    {
        write(2, fork_error, sizeof(fork_error));
        return 1;
    }
    else if (pid == 0)
    {
        close(pipe_fds[1]);

        char buffer[64];
        read(pipe_fds[0], buffer, sizeof(buffer));
        printf("%s", buffer);

        return 0;
    }
    else
    {
        pid_t pid2 = fork();

        if (pid2 < 0)
        {
            write(2, fork_error, sizeof(fork_error));
            return 1;
        }
        else if (pid2 == 0)
        {
            close(pipe_fds[0]);
            int num_wrote = write(pipe_fds[1], fork_success, sizeof(fork_success));
            return 0;
        }
        else
        {
            close(pipe_fds[0]);
            close(pipe_fds[1]);
            if (waitpid(pid2, NULL, 0) < 0)
            {
                char child2_error[] = "Error waiting for child 2 to die\n";
                write(2, child2_error, sizeof(child2_error));
                return 1;
            }

            if (waitpid(pid, NULL, 0) < 0)
            {
                char child_error[] = "Error waiting for child 1 to die\n";
                write(2, child_error, sizeof(child_error));
                return 1;
            }

            char parent_meassage[] = "I have waited for both my children to die. I am a bad mom.\n";
            printf("%s", parent_meassage);
        }
    }

    return 0;
}
