#include "wrappers.h"

pid_t Fork(void) {
  pid_t pid;
  if((pid = fork()) < 0) {
    perror("Fork error");
  }
  return pid;
}

int Pipe(int fdArr[2]) {
  int result = pipe(fdArr);
  if(result == -1) {
    perror("Pipe failed");
    exit(-1);
  }
  return result;
}

//Read exactly 'n' bytes from a descriptor, or fail
ssize_t Read(int fd, void *buf, size_t count) {
  size_t nleft;
  ssize_t nread;
  unsigned char *ptr;

  ptr = buf;
  nleft = count;
  while(nleft > 0) {
    if((nread = read(fd, ptr, nleft)) < 0) {
      if(errno == EINTR) {
        read = 0;
      }
      else {
        printf("Error with descriptor. Could read only %d bytes from requested %d\n", (int)(count - nleft), (int)count);
        perror("Reason");
        exit(-1);
      }
    }
    else if(nread == 0) {
      break;
    }

    nleft -= nread;
    ptr += nread;
  }

  if(nleft != 0) {
    printf("Unable to complete. Could read only %d bytes from requested %d\n", (int)(count - nleft), (int)count);
  }
  return count - nleft;
}

//Write exactly 'n' bytes to a descriptor, or fail
ssize_t Write(int fd, const void *buf, size_t count) {
  size_t nleft;
  ssize_t nwritten;
  const unsigned char *ptr;

  ptr = buf;
  nleft = count;

  while(nleft > 0) {
    if((nwritten = write(fd, ptr, nleft)) <= 0) {
        if(nwritten < 0 && errno == EINTR) {
          nwritten = 0;
        }
        else {
          printf("Error with descriptor. Could write only %d bytes from requested %d\n", (int)(count - nleft), (int)count);
          perror("Reason");
          exit(-1);
        }
    }
    nleft -= nwritten;
    ptr += nwritten;
  }
  return count - nleft;
}
