#include "uils.h"

#define PORT 10001


int main(int argc,char** argv){
  msg r,t;
  message rm;
  int bytes_written = 0;
  init(HOST,PORT);

  int fd = open("recv_fileX", O_CREAT | O_WRONLY, PERMISSIONS);

  while(1) {
      memset(r.payload, 0, sizeof(r.payload));
      memset(rm.message, 0, sizeof(rm.message));

      RECV_MSG(r);

      memcpy(&rm, r.payload, sizeof(r.payload));

      if (compute_checksum(rm.message) != rm.chksum){
          SEND_NACK(t, rm.seq_number);
      }

      if(r.len < MESSAGE_SIZE)
        bytes_written = write(fd, rm.message, r.len - 1);
      else
        bytes_written = write(fd, rm.message, r.len - 2);


      SEND_ACK(t, rm.seq_number);

      // if(bytes_written < MESSAGE_SIZE)
      //   break;
  }
  close(fd);
  printf("here"); 
  return 0;
}
