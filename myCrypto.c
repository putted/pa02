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

#define INPUT_CHUNK 16384

size_t fileDigest(int fd_in, uint8_t *digest, int fd_save) {
	
  // Declare mdCtx and buffer to be used	
  EVP_MD_CTX* mdCtx;
  char *buff[INPUT_CHUNK];
  
  if((mdCtx = EVP_MD_CTX_create()) == NULL)
	handleErrors("Failed to create mdCtx.");

  if (1 != EVP_DigestInit_ex(mdCtx, EVP_sha256(), NULL))
	handleErrors("Failed to initialize mdCtx.");
  
  // Read all the incoming data from 'fd_in' file descriptor
  int iter; 
  while (iter = read(fd_in, buff, INPUT_CHUNK), iter != 0)
  {
  // Compute the SHA256 hash value of this incoming data into the array 'digest'	  
	if (1 != EVP_DigestUpdate(mdCtx, buff, iter));
	  handleErrors("Failed to update digest.");  
  }
	
  int digest_len = strlen(digest);
  if (1 != EVP_DigestFinal_ex(mdCtx, digest, &digest_len))
    handleErrors("Failed to finalize digest.");

  EVP_MD_CTX_destroy(mdCtx);
  
  // If the file descriptor 'fd_save' is > 0, store a copy of the incoming data to 'fd_save'
  if (fd_save > 0)
  {
	  fd_save = fd_in;
  }
  
  // Returns actual size in bytes of the computed hash value
  return digest_len;
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
