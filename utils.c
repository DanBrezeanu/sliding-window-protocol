#include "utils.h"
#include <limits.h>

int compute_checksum(char message[MESSAGE_SIZE]) {
    int result = 0;
    int len = strlen(message)

    for (int i = 0; i < len; ++i) {
        result += message[i] % INT_MAX;
    }

    return result;
}
