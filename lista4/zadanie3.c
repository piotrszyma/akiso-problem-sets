#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#define INPUT_BUFFER 1024
#define TOK_BUFFER 64
#define DELIMETERS " \t\r\n\a"

void read_loop(void);
char *input(void);
char **commands(char *line);
char ***convert(char **commands);
void execute(char ***cmd);
int check(char *line);
int checkIn(char** command);
int checkOut(char** command);
int checkErr(char** command);


int main(int argc, char **argv) {
    read_loop();
    return 0;
}



void execute(char ***cmd) {
    int p[2];
    pid_t pid;
    int fd_in = 0;

    while (*cmd != NULL) {
        pipe(p);
        if ((pid = fork()) == -1) {
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {

            dup2(fd_in, 0);
            if (*(cmd + 1) != NULL) {

                dup2(p[1], 1);
            }

            int check_in = checkIn(*cmd);
            int check_out = checkOut(*cmd);
            int check_err = checkErr(*cmd);

            if(check_in != '\0')
            {
              char* file = *(*cmd + check_in);
              int in = open(file, O_RDWR|O_CREAT, 0644);
              if(in == - 1) {
                perror("open check in");
              }
              dup2(in, 0);
            }

            if(check_out != '\0')
            {
              char* file = *(*cmd + check_out);
              int out = open(file, O_RDWR|O_CREAT, 0644);
              if(out == - 1) {
                perror("open check out");
              }
              dup2(out, 1);
            }

            if(check_err != '\0')
            {
              char* file = *(*cmd + check_err);
              int err = open(file, O_RDWR|O_CREAT, 0644);
              if(err == - 1) {
                perror("err check out");
              }
              dup2(err, 2);
            }

            if(check_in != '\0') {
              *(*cmd + check_in - 1) = NULL;
            }

            if(check_out != '\0') {
              *(*cmd + check_out - 1) = NULL;
            }

            if(check_err != '\0')
            {
              *(*cmd + check_err - 1) = NULL;
            }
            execvp((*cmd)[0], *cmd);
            exit(EXIT_FAILURE);
        }
        else {
            wait(NULL);
            close(p[1]);
            fd_in = p[0];
            cmd++;
        }
    }
}

void read_loop(void) {
    int ampersand = 0;
    char *line;
    char **div_line;
    char ***cmds_list;
    printf("> ");
    for (;;) {
        line = input();

        int i = 0;
        while(line[i]!='\0') i++;

        if(line[i-1]=='&')
        {
          fprintf(stderr, "%s", "> ");
          line[i-1] = '\0';
          ampersand = 1;
        }

        if (check(line) == 0) {
          div_line = commands(line);
          cmds_list = convert(div_line);
          execute(cmds_list);
        free(line);
        }
        if(ampersand == 0)
        {
            printf("> ");
        } else
        {
          ampersand = 0;
        }

    }

}

char **commands(char *line) {
    int size = TOK_BUFFER, index = 0;
    char **commands = (char **) malloc(size * sizeof(char *));
    char *token;

    if (!commands) {
        fprintf(stderr, "Error while allocating memory for commands");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, DELIMETERS);
    while (token != NULL) {
        commands[index] = token;
        index++;

        if (index >= size) {
            size += TOK_BUFFER;
            commands = (char **) realloc(commands, size * sizeof(char *));
            if (!commands) {
                fprintf(stderr, "Error while reallocating memory for commands");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, DELIMETERS);

    }
    commands[index] = NULL;
    return commands;

}

char *input(void) {
    size_t size = INPUT_BUFFER;
    int index = 0;
    char *buff = (char *) malloc(sizeof(char) * size);
    int ch;

    if (!buff) {
        fprintf(stderr, "Error while allocating memory for input");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        ch = getchar();

        if (ch == EOF) {
            printf("\n");
            exit(0);
        }

        if (ch == '\n') {
            buff[index] = '\0';
            return buff;
        } else {
            buff[index] = ch;
        }
        index++;

        if (index >= size) {
            size += INPUT_BUFFER;
            buff = (char *) realloc(buff, size);
            if (!buff) {
                fprintf(stderr, "Error while allocating memory for input");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char ***convert(char **commands) {


    int n = 0;
    int size = 1;
    for (int i = 0; *(commands + i) != NULL; i++) {
        if (strcmp(*(commands + i), "|") == 0) {
            size++;
        }
    }

    char ***cmds = (char ***) malloc(size * sizeof(char ***));
    int k = 0;
    while (*(commands + n) != NULL) {
        if (strcmp(*(commands + n), "|") == 0) {

            *(commands + n) = NULL;
            *(cmds + k) = commands;
            commands = commands + n + 1;
            n = 0;
            k++;
        }
        n++;
    }

    *(cmds + k) = commands;
    *(cmds + k + 1) = NULL;

    if (strcmp(**cmds, "exit") == 0) {
        exit(0);
    }

    return cmds;
}


int check(char *line) {
    int i = strlen(line);

    while (i--) {
        if (line[i] != 32) {
            return 0;
        }
    }

    return 1;
}


int checkIn(char** command)
{
    int i = 0;
    while(*(command + i) != NULL)
    {
      if(*(command + i)[0] == '<')
      {
        return i + 1;
      }

      i++;
    }
    return '\0';
}

int checkOut(char** command)
{
    int i = 0;
    while(*(command + i) != NULL)
    {
      if(*(command + i)[0] == '>')
      {
        return i + 1;
      }

      i++;
    }
    return '\0';
}

int checkErr(char** command)
{
  int i = 0;
  while(*(command + i) != NULL)
  {
    if(*(*(command + i)) == '2')
    {
        if(*(*(command + i) + 1) == '>') {
          return i + 1;
        }

    }

    i++;
  }
  return '\0';
}
