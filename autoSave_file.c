#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FILENAME "data.txt"
#define INTERVAL 60 // Interval in seconds

void saveToFile(const char *data) {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    fprintf(file, "%s\n", data);
    fclose(file);
}

int main() {
    char data[100]; // Assuming data to be saved
    while (1) {
        // Get data to be saved, you can customize this part as per your requirement
        printf("Enter data to be saved: ");
        fgets(data, sizeof(data), stdin);

        // Save data to file
        saveToFile(data);
        
        printf("Data saved to file.\n");

        // Sleep for specified interval
        sleep(INTERVAL);
    }
    return 0;
}
