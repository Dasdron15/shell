#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void shell_loop(void);
char *shell_read(void);
char **shell_parse(char *line);

int main(void) {
    shell_loop();

    return 0;
}

void shell_loop(void) {
    while (1) {
        printf("> ");

        char *line = shell_read();
        char **line_split = shell_parse(line);

        printf("%s\n", line);

        free(line);
        free(line_split);
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

char **shell_parse(char *line) {
    int bufsize = 128;
    int count = 0;

    char *line_copy = malloc(sizeof(line));
    strcpy(line_copy, line);

    char **buffer = malloc(sizeof(char*) * bufsize);
    if (!buffer) {
        perror("Memory allocation failed");
        exit(1);
    }

    char *token;
    token = strtok(line, " ");

    while (token) {
        buffer[count++] = strdup(token);
        token = strtok(NULL, " ");

        if (count >= bufsize) {
            bufsize *= 2;
            buffer = realloc(buffer, sizeof(char*) * bufsize);
            if (!buffer) {
                perror("Memory reallocation failed");
                exit(1);
            }
        }
    }

    free(line_copy);
    return buffer;
}
