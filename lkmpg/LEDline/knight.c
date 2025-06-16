
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DELAY_US 50000

int main() {
    int fd = open("/dev/74hct164", O_WRONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    uint64_t pattern = 1;
    int direction = 1; // 1 = left, 0 = right

    while (1) {
        write(fd, &pattern, sizeof(pattern));
        usleep(DELAY_US);

        if (direction)
            pattern <<= 1;
        else
            pattern >>= 1;

        if (pattern == (1ULL << 63)) direction = 0;
        if (pattern == 1) direction = 1;
    }

    close(fd);
    return 0;
}

