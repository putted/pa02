/*
pa02: Message Digest & Signature using Pipes
FILE: myCrypto.c

Written By:
  1 - Ethan Putt
  2 - Matthew Jennings

Submitted on:
*/

#include "myCrypto.h"

void handleErrors(char *msg) {
  fprintf(stderr, "%s\n", msg);
  ERR_print_errors_fp(stderr);
  abort();
}

size_t fileDigest(int fd_in, uint8_t *digest, int fd_save) {

  // Declare mdCtx and buffer to be used
  EVP_MD_CTX* mdCtx;
  uint8_t buf[INPUT_CHUNK];
  int length;

  if((mdCtx = EVP_MD_CTX_create()) == NULL) {
    handleErrors("Failed to create mdCtx.");
  }

  if (EVP_DigestInit_ex(mdCtx, EVP_sha256(), NULL) != 1) {
    handleErrors("Failed to initialize mdCtx.");
  }

  // Read all the incoming data from 'fd_in' file descriptor
  int iter;
  while ((iter = read(fd_in, buf, INPUT_CHUNK)) > 0)
  {
    if (fd_save > 0)
    {
      write(fd_save, buf, iter);
    }

    if(EVP_DigestUpdate(mdCtx, buf, iter) != 1) {
      handleErrors("Failed to update digest");
    }
    
    length += iter;
  }

  if (EVP_DigestFinal_ex(mdCtx, digest, &length) != 1) {
    handleErrors("Failed to finalize digest.");
  }

  EVP_MD_CTX_destroy(mdCtx);

  // Returns actual size in bytes of the computed hash value
  return length;
}

RSA *getRSAfromFile(char *filename, int public) {
  FILE *fp = fopen(filename, "rb");
  if(fp == NULL) {
    printf("Unable to open RSA key file %s\n", filename);
    return NULL;
  }

  RSA *rsa = RSA_new();
  if(public) {
    rsa = PEM_read_RSA_PUBKEY(fp, &rsa, NULL, NULL);
  }
  else {
    rsa = PEM_read_RSAPrivateKey(fp, &rsa, NULL, NULL);
  }
  fclose(fp);
  return rsa;
}
