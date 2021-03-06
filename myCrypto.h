#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>

/* OpenSSL headers */
#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/bio.h>

#define INPUT_CHUNK 16384

void handleErrors(char *msg);
RSA *getRSAfromFile(char *filename, int public);
size_t fileDigest(int fd_in, uint8_t *digest, int fd_save);
