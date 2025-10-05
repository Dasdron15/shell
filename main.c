#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void shell_loop(void);
char *shell_read(void);
char **shell_parse(char *line);
void shell_launch(char **args);

int main(void) {
    shell_loop();

    return 0;
}

void shell_loop(void) {
    while (1) {
        printf("> ");

        char *line = shell_read();
        char **line_split = shell_parse(line);

        shell_launch(line_split);

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

    char **buffer = malloc(sizeof(char*) * bufsize);
    if (!buffer) {
        perror("Memory allocation failed");
        exit(1);
    }

    char *token;
    token = strtok(line, " ");

    while (token) {
        buffer[count] = token;
        count++;

        if (count >= bufsize) {
            bufsize *= 2;
            buffer = realloc(buffer, sizeof(char*) * bufsize);
            if (!buffer) {
                perror("Memory reallocation failed");
                exit(1);
            }
        }

        token = strtok(NULL, " ");
    }

    return buffer;
}

void shell_launch(char **args) {
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    pid_t pid, wpid;
    int status;
    
    pid = fork();

    if (pid == 0) {
        execvp(args[0], args);
        perror("lsh");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("lsh");
        exit(1);
    }
}
