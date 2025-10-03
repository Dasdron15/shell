#include <stdio.h>
#include <stdlib.h>

void shell_loop(void);
char *shell_read(void);

int main(void) {
    shell_loop();

    return 0;
}

void shell_loop(void) {
    while (1) {
        printf("> ");

        char *line = shell_read();
        printf("%s\n", line);

        free(line);
    }
}

char *shell_read(void) {
    int bufsize = 1024;
    int position = 0;
    int c;

    char *buffer = malloc(bufsize);
    if (!buffer) {
        perror("Memory allocation failed");
        exit(1);
    }

    while (1) {
        c = getchar();

        if (c == '\n' || c == EOF) {
            break;
        } else {
            buffer[position] = c;
        }

        position++;

        if (position >= bufsize) {
            bufsize *= 2;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                perror("Memory reallocation failed");
                exit(1);
            }
        }
    }

    return buffer;
}
