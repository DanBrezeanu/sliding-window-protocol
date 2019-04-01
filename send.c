#include "struc.h"
#include <sys/types.h>
#include <sys/stat.h>

#define PORT 10000

int main(int argc,char** argv){
  init(HOST,PORT);
  msg t;
  int i;
  message tm;
  int current_frame = 0;
  int W_SIZE = atoi(argv[2]) * atoi(argv[3]) * 1000 / ( 8 * sizeof(msg));
  int fd = open(argv[1], O_RDONLY);
  int file_size = lseek(fd, 0, SEEK_END);
  int COUNT =  file_size / MESSAGE_SIZE + 1;
  lseek(fd, 0, SEEK_SET);

  char* file_buffer = calloc(file_size + 3, sizeof(char));
  int buffer_index = 0;

  read(fd, file_buffer, file_size);
  close(fd);
  int min = MIN(W_SIZE, COUNT);
  for (i = 0; i < min; ++i) {
      memset(t.payload, 0, sizeof(t.payload));
      memset(&tm, 0, sizeof(tm));

      printf("%d\n", MIN(MESSAGE_SIZE, file_size));
      memcpy(tm.message, file_buffer + buffer_index, MIN(MESSAGE_SIZE, file_size));
      tm.seq_number = ++current_frame;

      memcpy(t.payload, &tm, sizeof(tm));
      buffer_index += MIN(MESSAGE_SIZE, file_size);
      file_size -= MIN(MESSAGE_SIZE, file_size);

      SEND_MSG(t);
       printf("sent %d %d %d\n", current_frame, i, MIN(W_SIZE, COUNT));
  }

  for (i = 0; i < COUNT - min; ++i) {
      memset(t.payload, 0, sizeof(t.payload));
      memset(&tm, 0, sizeof(tm));

      memcpy(tm.message, file_buffer + buffer_index, MIN(MESSAGE_SIZE, file_size));
      tm.seq_number = ++current_frame;

      memcpy(t.payload, &tm, sizeof(tm));
      buffer_index += MIN(MESSAGE_SIZE, file_size);
      file_size -= MIN(MESSAGE_SIZE, file_size);

      SEND_MSG(t);
      RECV_ACK(t);
        printf("sent %d %d %d\n", current_frame, i, MIN(W_SIZE, COUNT));

  }

  for (i = 0; i < min; ++i) {
      memset(t.payload, 0, sizeof(t.payload));
      memset(&tm, 0, sizeof(tm));

      RECV_ACK(t);
  }

  return 0;
}
