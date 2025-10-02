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
    char *buffer = malloc(sizeof(char) * bufsize);
    int position = 0;
    int c;

    while (1) {
        c = getchar();

        if (c == '\n' || c == EOF) {
            break;
        } else {
            buffer[position] = c;
        }

        position++;
    }

    return buffer;
}
