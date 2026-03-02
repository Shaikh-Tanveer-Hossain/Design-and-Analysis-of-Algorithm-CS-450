#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file;
    char line[256];
    int line_count = 0;

    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        line_count++;
    }

    fclose(file);

    printf("Total lines in %s: %d\n", argv[1], line_count);

    printf("Press any key to continue...");
    getchar();

    return 0;
}
