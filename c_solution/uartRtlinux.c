#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

#define UART_DEVICE "/dev/ttyUSB0"

int uart_fd;

void *uart_reader(void *arg) {
    char buffer[255];
    ssize_t bytes_read;
    
    while (1) {
        bytes_read = read(uart_fd, buffer, sizeof(buffer));
        if (bytes_read > 0) {
            printf("Received: %.*s\n", (int)bytes_read, buffer);
        }
    }
    
    return NULL;
}

int main() {
    struct termios options;
    pthread_t reader_thread;
    
    // Open UART device
    uart_fd = open(UART_DEVICE, O_RDWR | O_NOCTTY);
    if (uart_fd < 0) {
        perror("Error opening UART device");
        exit(EXIT_FAILURE);
    }
    
    // Configure UART
    tcgetattr(uart_fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag &= ~CSIZE; /* Clear data size bits */
    options.c_cflag |= CS8;    /* Set data size to 8 bits */
    options.c_cflag &= ~PARENB; /* No parity */
    options.c_cflag &= ~CSTOPB; /* One stop bit */
    options.c_cflag &= ~CRTSCTS; /* Disable hardware flow control */
    options.c_cc[VTIME] = 0; // No timeout (0 deciseconds)
    options.c_cc[VMIN] = 0; // Return immediately with available characters
    tcsetattr(uart_fd, TCSANOW, &options);
    
    // Create UART reader thread
    if (pthread_create(&reader_thread, NULL, uart_reader, NULL)) {
        perror("Error creating UART reader thread");
        exit(EXIT_FAILURE);
    }
    
    // Main thread writes data to UART
    char tx_buffer[] = "Hello UART!\n";
    ssize_t bytes_written = write(uart_fd, tx_buffer, strlen(tx_buffer));
    if (bytes_written < 0) {
        perror("Error writing to UART");
        exit(EXIT_FAILURE);
    }
    
    // Main thread continues doing other work or waits
    
    // Wait for the reader thread to finish (which it won't, as it's an infinite loop)
    pthread_join(reader_thread, NULL);
    
    // Close UART device
    close(uart_fd);
    
    return 0;
}
