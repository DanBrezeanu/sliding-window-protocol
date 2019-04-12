#include "utils.h"

#define PORT 10001

int main(int argc,char** argv){
  msg r,t;
  message rm, tm;
  init(HOST,PORT);

  RECV_MSG(r);  /* primim header-ul */
  memcpy(&rm, r.payload, sizeof(rm));

  int total_packages = rm.seq_number;
  int fd = open(rm.message, O_CREAT | O_WRONLY, PERMISSIONS);

  msg *file_buffer   = calloc(total_packages, sizeof(msg));
  int *sent_ack      = calloc(total_packages, sizeof(int));
  int sent_all       = 0;

  while(!sent_all) {
      memset(&r, 0, sizeof(r));
      memset(&rm, 0, sizeof(rm));
      memset(&tm, 0, sizeof(tm));

      RECV_MSG(r);
      memcpy(&rm, r.payload, sizeof(r.payload));

      if (compute_checksum(rm.message) != rm.chksum) { /* daca a fost corupt */
        sprintf(tm.message, "NACK");
        t.len = 4;
        tm.seq_number = rm.seq_number;
        memcpy(t.payload, &tm, sizeof(tm));
        send_message(&t);                              /* trimitem NACK */
      } else {                                         /* daca nu a fost corupt */
         file_buffer[rm.seq_number] = r;               /* adaugam in buffer */
         sent_ack[rm.seq_number] = 1;
         sprintf(tm.message, "ACK");
         t.len = 3;
         tm.seq_number = rm.seq_number;
         memcpy(t.payload, &tm, sizeof(tm));
         send_message(&t);                             /* trimitem ACK */
      }

      sent_all = 1;
      for (register int i = 0; i < total_packages; ++i)
        if (sent_ack[i] == 0) {
          sent_all = 0;
          break;
        }
  }

  for (register int i = 0; i < total_packages; ++i) { /* scriem in fisier */
    message x;
    memcpy(&x, file_buffer[i].payload, sizeof(file_buffer[i].payload));
    write(fd, x.message, file_buffer[i].len);
  }

  strncpy(t.payload, "END\0", 4);                     /* trimitem pachetul de sf */
  send_message(&t);
  close(fd);
  return 0;
}
