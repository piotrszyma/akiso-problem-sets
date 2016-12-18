#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFFSIZE 256

void myscanf(char *format, ...);

int convStringToInt(char *string);

int convStringHexToInt(char *string);

int convStringBinToInt(char *string);

char **substractBuffer(char *buffer);

int power(int a, int b);

int main(void) {

    char a[10];

    myscanf("abc%s", &a);

    printf("%s\n", a);
    return 0;
}

void myscanf(char *format, ...) {

//    printf("%s", format);
    int n;
    int p_ptr = 0;
    int b_ptr = 0;
    char c;
    char *p = (char *) &format + sizeof format;
    int in;
    char buffer[256];
    char pattern[256];
    char temp = '\0';

    bzero(buffer, 256);

    for (int i = 0; i < strlen(format); i++) {
        b_ptr = 0;
        p_ptr = 0;
        if (format[i] == '%') {
            i++;
            if (format[i] == 'd') {
                i++;
                while (b_ptr < 255) {
                    in = read(STDIN_FILENO, &c, 1);
                    if (c == EOF || c == '\0' || c == '\n' || c == EOF || c == ' ' || c == '\t') {
                        break;
                    } else if (c < 48 || c > 57) {
                        temp = c;
                        break;
                    }
                    buffer[b_ptr] = c;
                    b_ptr++;
                }

                buffer[b_ptr] = '\0';
                **((int **) p) = convStringToInt(buffer);
                p += sizeof(int);

            } else if (format[i] == 'b') {
                i++;
                while (b_ptr < 255) {
                    in = read(STDIN_FILENO, &c, 1);
                    if (c == EOF || c == '\0' || c == '\n' || c == EOF || c == ' ' || c == '\t') {
                        break;
                    } else if (c < 48 || c > 57) {
                        temp = c;
                        break;
                    }
                    buffer[b_ptr] = c;
                    b_ptr++;
                }

                buffer[b_ptr] = '\0';
                **((int **) p) = convStringBinToInt(buffer);
                p += sizeof(int);

            } else if (format[i] == 'x') {
                i++;


                while (b_ptr < 255) {
                    in = read(STDIN_FILENO, &c, 1);
                    if (c == EOF || c == '\0' || c == '\n' || c == EOF || c == ' ' || c == '\t') {
                        break;
                    } else if ((c < 48 || c > 57) && (c < 65 || c > 70) && (c < 97 || c > 102)) {
                        temp = c;
                        break;
                    }
                    buffer[b_ptr] = c;
                    b_ptr++;
                }

                buffer[b_ptr] = '\0';
                **((int **) p) = convStringHexToInt(buffer);
                p += sizeof(int);

            } else if (format[i] == 's') {

                i++;


                while (b_ptr < 255) {
                    in = read(STDIN_FILENO, &c, 1);
                    if (c == EOF || c == '\0' || c == '\n' || c == ' ' || c == '\t') {
                        break;
                    }

                    buffer[b_ptr] = c;
                    b_ptr++;
                }

                buffer[b_ptr] = '\0';
                char *output = *((char **) p);
                strcpy(output, buffer);
                p += sizeof(char);

            }

        } else {
            if (temp != '\0') {
                pattern[p_ptr] = temp;
                p_ptr++;
            }
            while (!(format[i] == EOF || format[i] == '\0' || format[i] == '\n' || format[i] == ' ' ||
                     format[i] == '%')) {
                in = read(STDIN_FILENO, &c, 1);
                if (c < 48 || c > 59) break;
                buffer[b_ptr] = c;
                pattern[p_ptr] = format[i];
                b_ptr++;
                p_ptr++;
                i++;
            }

            buffer[b_ptr] = '\0';
            pattern[p_ptr] = '\0';
            if (strcmp(buffer, pattern) != 0) {
                break;
            }
        }
    }


}

int convStringToInt(char *string) {
    int d = 0;
    int m = 1;
    for (int i = strlen(string) - 1; i >= 0; i--) {
        d += m * (string[i] - '0');
        m *= 10;
    }

    return d;
}

int convStringBinToInt(char *string) {
    int dec = 0;
    int rate = 1;
    if (string[0] == '-') {
        string = string + 1;
        rate = -1;
    }

    while (*string) {
        dec *= 2;
        if (*string++ == '1') dec += 1;
    }
    return dec * rate;
}

int convStringHexToInt(char *string) {
    int rate = 1;
    if (string[0] == '-') {
        string = string + 1;
        rate = -1;
    }
    char hex[16] = "0123456789ABCDEF";
    char *bin = malloc(BUFFSIZE * sizeof(char));
    int result = 0;
    int ptr = 0;
    for (int i = strlen(string) - 1; i >= 0; i--) {
        ptr = 0;
        for (; hex[ptr] != string[i]; ptr++);
        int pow = power(16, strlen(string) - 1 - i);
        result += ptr * pow;
    }
    return result * rate;
}

int power(int a, int b) {
    if (b == 0) {
        return 1;
    } else if (b == 1) {
        return a;
    } else {
        return a * power(a, b - 1);
    }
}