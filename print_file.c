#include <stdio.h>
#include <cups/cups.h>

void PrintFile(const char* filename) {
    cups_dest_t *dests;
    int num_dests;
    int job_id;
    FILE *file;
    char buffer[1024];

    // Get the list of destinations (printers)
    num_dests = cupsGetDests(&dests);

    if (num_dests <= 0) {
        printf("No printers found.\n");
        return;
    }

    // Open the file
    file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return;
    }

    // Print the file
    job_id = cupsPrintFile(dests->name, filename, "Print File", 0, NULL);
    if (job_id <= 0) {
        printf("Failed to print file.\n");
        fclose(file);
        return;
    }

    printf("File sent to printer successfully. Job ID: %d\n", job_id);

    // Close the file
    fclose(file);
}

int main() {
    // Replace "filename.txt" with the path of the file you want to print
    PrintFile("filename.txt");
    return 0;
}
