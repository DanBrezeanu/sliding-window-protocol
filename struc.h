#ifndef _STRUCH_
#define _STRUCH_

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
    printf("ACK\n");

#define RECV_MSG(r)  if (recv_message(&r)<0){ \
   perror("Receive message"); \
   return -1; \
 }

 #define SEND_ACK(t) sprintf(t.payload,"ACK");\
    t.len = strlen(t.payload+1);\
    send_message(&t);

#define MIN(X,Y) ((X) < (Y)) ? (X) : (Y)

#define MESSAGE_SIZE 1396

typedef struct {
    char message[MESSAGE_SIZE];
    int seq_number;
} message;

#endif
