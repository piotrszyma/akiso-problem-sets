#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#include <limits.h>

#define MAXSIZE 256

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    struct stat file;
    int size;

    char buffer[256];

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);

    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname(argv[1]);

    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }
    char username[MAXSIZE];
    char password[MAXSIZE];

    do {
        printf("[ftp] # Username: ");
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);
        strcpy(username, buffer);
        n = write(sockfd, username, 256);

        printf("[ftp] # Password: ");
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);

        strcpy(password, buffer);

        n = write(sockfd, password, 256);

        n = read(sockfd, buffer, 1);

        if(buffer[0] != '-') {
            printf("[ftp] # Wrong login/pass, try again.\n");
        }
    } while (buffer[0] != '-');




    /* Now ask for a message from the user, this message
       * will be read by server
    */
    while (1) {
        printf("[ftp] # ");
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);

        //wyluskanie komendy
        char command[4];
        strncpy(command, buffer, 3);
        command[3] = '\0';

        //jesli komenda do lsc to printnij pliki klienta
        if (strcmp(command, "lsc") == 0) {
            system("ls -pa");

            //jesli komenda to end to zakoncz dzialanie klienta
        } else if (strcmp(command, "end") == 0) {
            n = write(sockfd, buffer, 256);

            if (n < 0) {
                perror("ERROR writing to socket");
                exit(1);
            }
            break;
        } else {
            //jesli zadne z powyzszych to wyslij buffer do serwera i...

            n = write(sockfd, buffer, 256);

            if (n < 0) {
                perror("ERROR writing to socket");
                exit(1);
            }

            //jesli komenda to put to...
            if (strcmp(command, "put") == 0) {
                //zaimplementuj put

                //bierze z komendy nazwe pliku
                //wpisuje do filename nazwe pliku
                int filehandle;
                char filename[MAXSIZE];
                sscanf(buffer + 4, "%s", filename);



                //sprawdz czy taki plik istnieje na kliencie
                //szuka pliku na kliencie
                int err = stat(filename, &file);
                //blad jesli nie znajduje, zakoncz
                if (err == -1) {
                    perror("ERROR no such a file");
                } else {
                    //jesli jednak jest, to:

                    filehandle = open(filename, O_RDONLY);
                    size = file.st_size;
                    if (filehandle == -1) {
                        perror("ERROR opening file on server... ?");
                        size = 0;
                        bzero(buffer, 256);
                        buffer[0] = '-';
                        write(sockfd, buffer, 256);
                        //jesli jest to wysyla rozmiar
                    } else {
                        //wyslij rozmiar do serwera
//                        printf("\nROZMIAR PLIKU: %s\n", size)
                        printf("\n FILE SIZE: %d \n", size);

                        sprintf(buffer, "%d", size);
                        fflush(stdout);
                        write(sockfd, buffer, 256);
//
//
//                        //wysyla max 256 bitowe paczki z plikiem
//
                        int i = 0;

                        int last = size % 256;
                        while (i < size) {
                            if (i + 256 > size) {
                                lseek(filehandle, i, SEEK_SET);
                                n = read(filehandle, buffer, last);
                                n = write(sockfd, buffer, last);
                            } else {
                                lseek(filehandle, i, SEEK_SET);
                                n = read(filehandle, buffer, 256);
                                n = write(sockfd, buffer, 256);
                            }

                            i += 256;
                        }

                    }

                    //koniec putu


                }

                //jesli komenda to get to...
            } else if (strcmp(command, "get") == 0) {

                //bierze z komendy nazwe pliku
                //wpisuje do filename nazwe pliku
                int filehandle;
                char filename[MAXSIZE];
                sscanf(buffer + 4, "%s", filename);

                //pobiera rozmiar pliku
                n = read(sockfd, buffer, 256);

                if (n < 0) {
                    perror("ERROR reading from socket");
                    exit(1);
                } else if (buffer[0] == '-') {
                    printf("No such a file on server\n");
                    fflush(stdout);
                } else {
                    //parsuje rozmiar na inta
                    sscanf(buffer, "%s", command);
                    size = atoi(command);

                    //sprawdzenie duplikatow
                    int j = 1;
                    int filenamesize = strlen(filename);
                    fflush(stdout);
                    //jesli jest duplikatem to dokleja _1 _2 ... itd
                    do {
                        filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
                        filename[filenamesize] = '\0';
                        char duplicate[MAXSIZE];
                        sprintf(duplicate, "_%d", j);
                        strcat(filename, duplicate);
                        j++;
                    } while (filehandle == -1);


                    //odbieraj 256 bitowe paczki rozmiar/256 + 1razy
                    int i = 0;

                    int last = size % 256;
                    while (i < size) {
                        if (i + 256 > size) {
                            n = read(sockfd, buffer, last);
                            lseek(filehandle, i, SEEK_SET);
                            n = write(filehandle, buffer, last);
                        } else {
                            n = read(sockfd, buffer, 256);
                            lseek(filehandle, i, SEEK_SET);
                            n = write(filehandle, buffer, 256);

                        }

                        i += 256;
                    }
                }

                //jesli komenda to ls to
            } else if (command[0] == 'l' && command[1] == 's') {
                //pobierz z serwera wynik ls -p

                n = read(sockfd, buffer, 255);

                if (n < 0) {
                    perror("ERROR reading from socket");
                    exit(1);
                } else {
                    //wypisz ls z serwera
                    printf("%s", buffer);
                }
            } else if (command[0] == 'c' && command[1] == 'd') {
                n = read(sockfd, buffer, 255);

                if (n < 0) {
                    perror("ERROR reading from socket");
                    exit(1);
                } else {
                    //wypisz ls z serwera
                    printf("%s", buffer);
                }
            }
        }


    }
    close(sockfd);
    return 0;
}