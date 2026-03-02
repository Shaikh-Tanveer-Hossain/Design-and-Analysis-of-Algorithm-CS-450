#include <stdio.h>
#include <windows.h> // For Sleep()

int main() {
    printf("Waiting for 3 seconds...\n");
    Sleep(3000); // Pauses execution for 3000 milliseconds (3 seconds)
    printf("Done.\n");
    return 0;
}
