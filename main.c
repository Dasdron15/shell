#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>

void shell_loop(void);
char *shell_read(void);
char **shell_parse(char *line);
void shell_launch(char **args);

int main(void) {
    shell_loop();

    return 0;
}

void shell_loop(void) {
    char cwd[PATH_MAX];

    while (1) {
        getcwd(cwd, sizeof(cwd));

        printf("%s\n", cwd);
        printf("> ");

        char *line = shell_read();
        char **line_split = shell_parse(line);

        shell_launch(line_split);

        free(line);
        free(line_split);
        printf("\n");
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

    buffer[position] = '\0';

    return buffer;
}

char **shell_parse(char *line) {
    int bufsize = 128;
    int position = 0;

    char **args = malloc(sizeof(char*) * bufsize);
    if (!args) {
        perror("Memory allocation failed");
        exit(1);
    }

    char *token;
    token = strtok(line, " ");

    while (token) {
        args[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize *= 2;
            args = realloc(args, sizeof(char*) * bufsize);
            if (!args) {
                perror("Memory reallocation failed");
                exit(1);
            }
        }

        token = strtok(NULL, " ");
    }

    args[position] = NULL;
    return args;
}

void shell_cd(char **args) {
    char *path = args[1];

    if (path == NULL) {
        struct passwd *pw = getpwuid(getuid());
        if (!pw) {
            perror("type shit");
        }
    
        path = pw->pw_dir;
    }

    if (chdir(path) != 0) {
        perror("cd");
    }
}

void shell_launch(char **args) {
    if (!args[0]) {
        return;
    }

    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }
    if (strcmp(args[0], "cd") == 0) {
        shell_cd(args);
        return;
    }

    pid_t pid, wpid;
    int status;
    
    pid = fork();

    if (pid == 0) {
        execvp(args[0], args);
        fprintf(stderr, "dsh: command not found: %s\n", args[0]);
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        fprintf(stderr, "dsh: command not found: %s\n", args[0]);
        exit(1);
    }
}
