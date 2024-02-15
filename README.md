# os-processes
## Here is what we need to do:

### Grouped tasks:
* Make child 1 function - split the pipe through fork()
* Make child 2 function  - split the pipe through fork()
* Make parent - Makes the pipe

### Everyone's tasks:
* Make branch for group

## By Tuesday: Everything should be merged into main branch and completed

### Parent - madre.c

1. Create one pipe (ear/read, mouth/write)
    1. Pipe is used for communication between the children
2. Fork off child 1 → Child 1 inherits the pipe
3. Fork off child 2 → Child 2 inherits the same pipe
4. Close both ends of the pipe
5. Waits for both children to die

### Child 1 - in function

1. First closes the read end of the pipe
2. Uses `dup2()` to replace its existing stdout by the write end of the pipe
3. Closes the write end of the pipe
4. Replaces its executable by `./taco` using `execvp()`

### Child 2 - in function

1. Closes the write end of the pipe
2. Uses `dup2()` to replace it’s existing stdin by the read end of the pipe
3. Closes the read end of the pipe
4. Replaces its executable by `./gato` using `execvp()`
