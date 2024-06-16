#include <stdio.h>
#include <time.h>

int main() {
    // Get current time
    time_t currentTime;
    struct tm *localTime;

    time(&currentTime);
    localTime = localtime(&currentTime);

    // Display current time
    printf("Current time: %02d:%02d:%02d\n", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

    return 0;
}

