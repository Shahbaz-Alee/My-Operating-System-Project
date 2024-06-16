#include <stdio.h>

int main() {
    FILE *filePointer;

    // Open file in write mode
    filePointer = fopen("newfile.txt", "w");

    // Check if file was opened successfully
    if (filePointer == NULL) {
        printf("Failed to create file.\n");
        return 1;
    }

    // File created successfully, close it
    fclose(filePointer);
    printf("File created successfully.\n");

    return 0;
}
