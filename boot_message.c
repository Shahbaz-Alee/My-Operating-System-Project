#include <stdio.h>

void boot_message() {
    printf("Welcome! System is booting up...\n");
}

void shutdown_message() {
    printf("Goodbye! System is shutting down...\n");
}

int main() {
    boot_message(); // Display boot message
    // Your main program logic goes here
    shutdown_message(); // Display shutdown message
    return 0;
}
