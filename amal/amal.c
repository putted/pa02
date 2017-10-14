/*
pa02: Message Digest & Signature using Pipes
FILE: amal.c

Written By:
  1 - Ethan Putt
  2 - Matthew Jennings

Submitted on:
*/

#include "../myCrypto.h"

int main(int argc, char *argv[]) {
  /* Initialize the crypto library */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);

  if(argc < 3) {
    printf("Missing command-line arguments %s <ctrlFD> <dataFD>\n", argv[0]);
    exit(-1);
  }
  int fd_ctrl = atoi(argv[1]);
  int fd_data = atoi(argv[2]);

  FILE* log = fopen("amal/logAmal.txt", "w");
  if(!log) {
    fprintf(stderr, "This is Amal. Could not create log file\n");
    exit(-1);
  }
  fprintf(log, "This is Amal. Will send digest to FD %d and file to FD %d\n", fd_ctrl, fd_data);

  char *filename = "amal/bunny.mp4";
  int fd_in = open(filename, O_RDONLY, S_IRUSR | S_IWUSR);
  if(fd_in == -1) {
    fprintf(stderr, "This is Amal. Could not open input file\n");
    exit(-1);
  }
  fprintf(log, "This is Amal. Starting to digest the input file\n");

  /* --------------------- Added Code ---------------------------- */

  //get hash value
  uint8_t digest_output[INPUT_CHUNK];
  int dlen = fileDigest(fd_in, digest_output, fd_data);

  //print the digest of the file
  fprintf(log, "This is Amal. Here is the digest of the file:\n");
  BIO *bio_log = BIO_new_fp(log, BIO_NOCLOSE);
  BIO_dump(bio_log, digest_output, dlen);

  /*
  //getRSAfromFile
  RSA *private = getRSAfromFile("amal/amal_priv_key.pem", 0);

  //encrypt hash value using RSA private key
  uint8_t *encrypt_output = (uint8_t *)malloc(RSA_size(private));
  int encrypt_len = RSA_private_encrypt(dlen, digest_output, encrypt_output, private, RSA_PKCS1_PADDING);
  if(encrypt_len < 0) {
    fprintf(stderr, "ERROR: Encryption failed\n");
    exit(-1);
  }

  BIO_dump(bio_log, encrypt_output, encrypt_len);

  //send a copy of the file through the data pipe
  uint8_t read_output[INPUT_CHUNK];
  int read_len;
  while((read_len = read(fd_in, read_output, INPUT_CHUNK)) > 0) {
    if(write(fd_data, read_output, read_len) != read_len) {
      fprintf(stderr, "ERROR: Amal failed to write to data pipe\n");
      exit(-1);
    }
  }

  //send the encrypted hash value over the control pipe
  if(write(fd_ctrl, encrypt_output, encrypt_len) != encrypt_len) {
    fprintf(stderr, "ERROR: Amal failed to write to control pipe\n");
    exit(-1);
  }

  BIO_flush(bio_log);
  free(encrypt_output);

  /* -------------------- End Added Code ---------------------------*/

  EVP_cleanup();
  ERR_free_strings();

  fclose(log);

  return 0;
}
