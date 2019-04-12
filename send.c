#include "utils.h"
#include <sys/types.h>
#include <sys/stat.h>

#define PORT 10000

msg form_header(char* file_name, int number_packets, int file_size) {
  /* Primul cadru ce contine numele fisierului ce trebuie trimis si dimensiunea acestuia */
  msg t;
  message tm;

  strcpy(tm.message, file_name);
  tm.seq_number = number_packets;
  tm.chksum = file_size;

  memcpy(t.payload, &tm, sizeof(tm));
  t.len = strlen(tm.message);

  return t;
}

int main(int argc,char** argv){
  init(HOST,PORT);
  msg t;
  message tm;
  message dm;

  int fd        = open(argv[1], O_RDONLY);
  int file_size = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);

  int W_SIZE           = atoi(argv[2]) * atoi(argv[3]) * 1000 / (8 * sizeof(msg));
  int timeout_t        = 2;
  int current_frame    = 0;
  int last_packet_size = 0;
  int number_packets   = file_size / MESSAGE_SIZE + 1;

  unsigned char* file_buffer = calloc(file_size + 1, sizeof(unsigned char));
  read(fd, file_buffer, file_size);
  close(fd);

  msg* file_packets = calloc(number_packets, sizeof(msg));
  int* got_ack      = calloc(number_packets, sizeof(int));

  /* creez buffer pentru pachete */
  for (register int i = 0; i < number_packets; ++i) {
    memset(&dm, 0, sizeof(dm));

    if (i == number_packets - 1) { /* daca este ultimul pachet */
      last_packet_size = file_size - MESSAGE_SIZE * i;
      memcpy(dm.message, file_buffer + MESSAGE_SIZE * i, last_packet_size);
      file_packets[i].len = last_packet_size;

    } else {
      memcpy(dm.message, file_buffer + MESSAGE_SIZE * i, MESSAGE_SIZE);
      file_packets[i].len = MESSAGE_SIZE;
    }

    dm.seq_number = i;
    dm.chksum = compute_checksum(dm.message); /* calculeaza crc-ul pachetului */

    memcpy(file_packets[i].payload, &dm, sizeof(dm)); /* adauga in buffer */
  }

  msg header_msg = form_header("recv_fileX", number_packets, file_size);
  send_message(&header_msg); /* trimite header-ul */

  int min = MIN(W_SIZE, number_packets);
  for (register int i = 0; i < min; ++i, ++current_frame) {
     /* umple legatura */
      send_message(&file_packets[i]);
  }

  while(current_frame < number_packets) { /* cat timp mai sunt pachete de trimis */
      if (recv_message_timeout(&t, timeout_t) >= 0) { /* daca nu a dat timeout */
        memcpy(&tm, t.payload, sizeof(tm));

        if (strcmp(tm.message, "ACK") == 0) {         /* am primit ACK */
          got_ack[tm.seq_number] = 1;                 /* validam pachetul */
          send_message(&file_packets[current_frame]); /* trimitem urmatorul pachet */
          ++current_frame;
        } else {                                      /* am primit NACK */
          send_message(&file_packets[tm.seq_number]); /* retrimitem pachetul */
        }
      }
      else {                                          /* a dat timeout */
        for (register int j = 0; j < number_packets; ++j)
          if (got_ack[j] == 0) {
            /* retrimitem ultim pachet pentru care nu am primit ACK*/
            send_message(&file_packets[j]);
            send_message(&file_packets[current_frame]);
            current_frame++;
            break;
        }
      }
  }

  while(1) {  /* cat timp mai primim ACK-uri */
      memset(t.payload, 0, sizeof(t.payload));
      memset(&tm, 0, sizeof(tm));

      if (recv_message_timeout(&t, timeout_t) >= 0) { /* daca nu a dat timeout */
        memcpy(&tm, t.payload, sizeof(tm));

        if (strcmp(t.payload, "END") == 0)           /* am primit pachetul de sfarsit */
          break;

        if (strcmp(tm.message, "ACK") == 0) {       /* am primit ACK */
          got_ack[tm.seq_number] = 1;               /* validam pachetul */
        } else {                                    /* am primit NACK */
          send_message(&file_packets[tm.seq_number]); /* retrimitem pachetul */
        }
      }
      else {                                        /* a dat timeout*/
        for (register int j = 0; j < number_packets; ++j)
          /* retrimitem ultimul pachet pentru care nu am primit ACK*/
          if (got_ack[j] == 0) {
            send_message(&file_packets[j]);
            break;
        }
      }
  }

  return 0;
}
