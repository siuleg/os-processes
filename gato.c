
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* This function writes len bytes from the buffer buf
   to the file descriptor fd.

   It uses write() to accomplish this job.

   It returns 0 if everything went well.
   It returns a negative number (-1) otherwise.

*/
int my_write(int fd, const void *buf, size_t len) {
  size_t bytes_to_be_written;
  size_t bytes_already_written;
  ssize_t write_res;
  size_t bytes_written_this_time;

  /* If less than 1 byte (hence 0 bytes) to write,
     we have nothing to do.
  */
  if (len < ((size_t)1))
    return 0;

  /* Loop until all bytes have been written */
  for (bytes_to_be_written = len, bytes_already_written = (size_t)0;
       bytes_to_be_written > (size_t)0;
       bytes_to_be_written -= bytes_written_this_time,
      bytes_already_written += bytes_written_this_time) {
    write_res =
        write(fd, &(((char *)buf)[bytes_already_written]), bytes_to_be_written);
    if (write_res < ((ssize_t)0)) {
      /* If write returns a negative value, we
         have a failure condition.
      */
      return -1;
    }
    bytes_written_this_time = (size_t)write_res;
  }
  return 0;
}

int main(int argc, char **argv) {
  char buffer[4096];
  ssize_t read_res;
  size_t bytes_to_write;

  for (;;) {
    read_res = read(0, buffer, sizeof(buffer));
    if (read_res < ((ssize_t)0)) {
      /* This breaks the layer-2 rule !!! */
      fprintf(stderr, "Error reading from stdin: %s\n", strerror(errno));
      return 1;
    }
    if (read_res == ((ssize_t)0))
      break;
    bytes_to_write = (size_t)read_res;

    /* Here goes the code to write bytes_to_write bytes to standard
       output
    */
    if (my_write(1, buffer, bytes_to_write) < 0) {
      fprintf(stderr, "Error writing to stdout: %s\n", strerror(errno));
      return 1;
    }
  }
  return 0;
}
