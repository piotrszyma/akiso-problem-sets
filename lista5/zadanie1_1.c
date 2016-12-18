#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 256

void myprintf(char *, ...);

char *convIntToString(int num);

char *convDecToBin(int num);

char *convDecToHex(int num);

int main(void) {
    char tab[] = "abc";
    myprintf("Liczba DEC: %d %d %d %d", 123, 123, 123, 123);
    myprintf("\nLiczba HEX: %x", 123);
    myprintf("\nLiczba BIN: %b", 123);
    myprintf("\nString: %s\n", "abc");
    return 0;
}


void myprintf(char *format, ...) {
    int n;

    int i = 0;
    char *p = (char *) &format + sizeof format;
    while (*(format + i) != '\0') {

        if (*(format + i) != '%') {
            write(STDOUT_FILENO, format + i, sizeof(char));
        } else {
            i++;

            if (*(format + i) == '%') {
                write(STDOUT_FILENO, format + i, sizeof(char));
            } else if (*(format + i) == 'd') {
                int number = *((int *) p);
                p += sizeof(int);
                write(STDOUT_FILENO, convIntToString(number), sizeof(int));
            } else if (*(format + i) == 's') {
                char *word = *((char **) p);
                p += sizeof(word);
                write(STDOUT_FILENO, word, sizeof(int));
            } else if (*(format + i) == 'b') {
                int number = *((int *) p);
                p += sizeof(int);
                write(STDOUT_FILENO, convDecToBin(number), sizeof(char) * 32);
            } else if (*(format + i) == 'x') {
                int number = *((int *) p);
                p += sizeof(int);
                write(STDOUT_FILENO, convDecToHex(number), sizeof(char) * 8);
            }

        }
        i++;

    }
}


char *convIntToString(int num) {
    int n = num;
    int i = 0;
    int isNeg = 0;
    int tmp = num;
    if (tmp < 0) tmp *= -1;
    int size = 0;

    while (tmp > 0) {
        tmp /= 10;
        size++;
    }

    char *buffer = malloc(sizeof(char) * size);

    unsigned int n1 = n;


    if (n < 0) {
        isNeg = 1;
        n1 = n * -1;
    }


    while (n1 != 0) {
        buffer[i++] = n1 % 10 + '0';
        n1 = n1 / 10;
    }

    if (isNeg)
        buffer[i++] = '-';

    buffer[i] = '\0';

    for (int t = 0; t < i / 2; t++) {
        buffer[t] ^= buffer[i - t - 1];
        buffer[i - t - 1] ^= buffer[t];
        buffer[t] ^= buffer[i - t - 1];
    }

    if (n == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
    }

    return buffer;
}

char *convDecToBin(int num) {

    //Initialize mask (2^32)
    unsigned int mask = 0x80000000;
    size_t bits = 32;
    char *output = malloc(sizeof(char) * 33);

    int count;

    for (count = 0; count < bits; count++) {

        //add
        (mask & num) ? (*(output + count) = '1') : (*(output + count) = '0');
        //shift one to the right
        mask = mask >> 1;
    }
    *(output + count) = '\0';
    return output;
}

char *convDecToHex(int num) {
    static char hex[] = "0123456789ABCDEF";
    char *bin = convDecToBin(num);

    char *output = malloc(sizeof(char) * 8);
    int j = 0;
    for (int i = 0; i < 31; i += 4, j++) {
        int num = 0;
        char *tmp = malloc(sizeof(char) * 5);

        *(tmp) = *(bin + i);
        *(tmp + 1) = *(bin + i + 1);
        *(tmp + 2) = *(bin + i + 2);
        *(tmp + 3) = *(bin + i + 3);
        *(tmp + 4) = '\0';

        if (*(tmp) == '1') {
            num += 8;
        }
        if (*(tmp + 1) == '1') {
            num += 4;
        }
        if (*(tmp + 2) == '1') {
            num += 2;
        }
        if (*(tmp + 3) == '1') {
            num += 1;
        }

        *(output + j) = hex[num];
        free(tmp);

    }
    *(output + j) = '\0';

    return output;


}




