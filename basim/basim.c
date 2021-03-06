/*
pa02: Message Digest & Signature using Pipes
FILE: basim.c

Written By:
  1 - Ethan Putt
  2 - Matthew Jennings

Submitted On:
*/

#include "../myCrypto.h"

int main(int argc, char *argv[]) {
  /* Initialize the crypto library */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);

  if(argc < 3) {
    printf("Missing command-line arguments: %s <ctrlFD> <dataFD>\n", argv[0]);
    exit(-1);
  }
  int fd_ctrl = atoi(argv[1]);
  int fd_data = atoi(argv[2]);

  FILE* log = fopen("basim/logBasim.txt", "w");
  if(!log) {
    fprintf(stderr, "This is Basim. Could not create log file\n");
    exit(-1);
  }
  fprintf(log, "This is Basim. Will receive digest from FD %d and file from FD %d\n", fd_ctrl, fd_data);

  int fd_out = open("basim/bunny.mp4", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if(fd_out == -1) {
    fprintf(stderr, "This is Basim. Could not open output file\n");
    exit(-1);
  }
  fprintf(log, "This is Basim. Starting to receive incoming file and compute its digest\n");

  //WRITE CODE HERE

  EVP_cleanup();
  ERR_free_strings();

  fclose(log);
  close(fd_ctrl);
  close(fd_data);

  return 0;
}
