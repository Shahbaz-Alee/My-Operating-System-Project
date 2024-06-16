#include <stdio.h>

int main() {
    char filename[100]; // Assuming a maximum filename length of 100 characters

    printf("Enter the filename to delete: ");
    scanf("%s", filename); // Allowing the user to input the filename

    if (remove(filename) == 0) {
        printf("%s file deleted successfully.\n", filename);
    } else {
        printf("Unable to delete the file %s.\n", filename);
    }

    return 0;
}
