#include "struc.h"
#include <sys/types.h>
#include <sys/stat.h>

#define PORT 10000

int main(int argc,char** argv){
  init(HOST,PORT);
  msg t;
  int current_frame = 0;
  int fd = open(argv[1], O_RDONLY);
  int file_size = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);

  char* file_buffer = calloc(file_size + 3, sizeof(char));
  int buffer_index = 0;

  read(fd, file_buffer, file_size);
  while(file_size) {
      memset(t.payload, 0, sizeof(t.payload));

      memcpy(t.payload, file_buffer + buffer_index, MIN(MSGSIZE, file_size));
      buffer_index += MIN(MSGSIZE, file_size);
      file_size -= MIN(MSGSIZE, file_size);

      SEND_MSG(t);
      RECV_ACK(t);
  }

  return 0;
}
