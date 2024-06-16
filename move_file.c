#include <stdio.h>

int main() {
    char srcPath[100], destPath[100];

    // Ask user for source file path
    printf("Enter the source file name: ");
    scanf("%s", srcPath);

    // Ask user for destination file path
    printf("Enter the destination path: ");
    scanf("%s", destPath);

    // Rename function to move file
    if (rename(srcPath, destPath) == 0) {
        printf("File moved successfully.\n");
    } else {
        perror("Error moving file");
        return 1;
    }

    return 0;
}
