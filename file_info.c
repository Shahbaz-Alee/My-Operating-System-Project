#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main() {
    char filename[100];
    struct stat fileStat;

    printf("Enter the file name: ");
    scanf("%s", filename);

    // Check if the file exists
    if (stat(filename, &fileStat) < 0) {
        printf("Error: Unable to stat file.\n");
        return 1;
    }

    // Print file information
    printf("File Information:\n");
    printf("File Name: %s\n", filename);
    printf("Size: %ld bytes\n", fileStat.st_size);
    printf("Permissions: ");
    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");
    printf("Last Modified: %s", ctime(&fileStat.st_mtime));

    return 0;
}
