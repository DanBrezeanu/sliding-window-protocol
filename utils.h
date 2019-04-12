#ifndef _UTILSH_
#define _UTILSH_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PERMISSIONS 0777

#define RECV_MSG(r)  if (recv_message(&r)<0){ \
   perror("Receive message"); \
   return -1; \
 }

#define MIN(X,Y) ((X) < (Y)) ? (X) : (Y)

#define MESSAGE_SIZE 1392

typedef struct {
    char message[MESSAGE_SIZE]; /* measj efectiv */
    int chksum;                 /* checksum */
    int seq_number;             /* numar de secventa */
} __attribute__((packed)) message;

int compute_checksum(char message[MESSAGE_SIZE]);

#endif
