#include "utils.h"
#include <limits.h>

int compute_checksum(char message[MESSAGE_SIZE]) {
    /* calculeaza checksum-ul mesajului efectiv */
    int result = 0;

    for (int i = 0; i < MESSAGE_SIZE; ++i) {
        result += message[i] % INT_MAX;
    }

    return result;
}
