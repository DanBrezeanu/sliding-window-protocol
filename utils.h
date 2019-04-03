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

#define SEND_MSG(t) t.len = strlen(t.payload)+1; \
    send_message(&t);

#define RECV_ACK(t) if(recv_message(&t) >= 0)\
    printf("ACK %s\n", t.payload);

#define RECV_MSG(r)  if (recv_message(&r)<0){ \
   perror("Receive message"); \
   return -1; \
 }

 #define SEND_ACK(t, seq) sprintf(t.payload,"%d", seq);\
    t.len = strlen(t.payload+1);\
    send_message(&t);

#define MIN(X,Y) ((X) < (Y)) ? (X) : (Y)

#define MESSAGE_SIZE 1392

typedef struct {
    char message[MESSAGE_SIZE];
    int chksum;
    int seq_number;
} __attribute__((packed)) message;

int compute_checksum(char message[MESSAGE_SIZE]);

#endif
