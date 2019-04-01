#include "struc.h"

#define PORT 10001


int main(int argc,char** argv){
  msg r,t;
  init(HOST,PORT);

  int fd = open("recv_fileX", O_CREAT | O_WRONLY, PERMISSIONS);

  while(1) {
      memset(r.payload, 0, sizeof(r.payload));

      RECV_MSG(r);
      write(fd, r.payload, sizeof(r.payload));

      SEND_ACK(t);

  }

  return 0;
}
