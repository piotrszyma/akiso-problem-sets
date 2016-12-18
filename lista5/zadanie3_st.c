#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sendfile.h>
#include <fcntl.h>

#include <errno.h>

#define MAXSIZE 512


int main(int argc, char *argv[]) {
    char username[] = "admin";
    char password[] = "pass";

    char password_tmp[256];
    char username_tmp[256];

    int sockfd, newsockfd, portno;
    unsigned int clilen;
    char buffer[MAXSIZE];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    char command[5];
    struct stat file;
    int filehandle, size;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    /* Now start listening for the clients, here
       * process will go in sleep mode and will wait
       * for the incoming connection
    */

    listen(sockfd, 5);

    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    int isOk = 0;

    while (isOk == 0) {
        if (newsockfd < 0) {
            perror("ERROR on accept");
            exit(1);
        }

        n = recv(newsockfd, buffer, MAXSIZE, 0);


        if (n < 0) {
            perror("ERROR reading from socket");
            break;
        }

        sscanf(buffer, "%s", username_tmp);
        bzero(buffer, 256);
        n = recv(newsockfd, buffer, MAXSIZE, 0);

        if (n < 0) {
            perror("ERROR reading from socket");
            break;
        }

        sscanf(buffer, "%s", password_tmp);
        bzero(buffer, 256);
        if (strcmp(password, password_tmp) == 0 && strcmp(username, username_tmp) == 0) {
            n = write(newsockfd, "-", 1);
            isOk = 1;
        } else {
            n = write(newsockfd, "0", 1);
        }
    }


    while (1) {

        n = recv(newsockfd, buffer, MAXSIZE, 0);

        if (n < 0) {
            perror("ERROR reading from socket");
            break;
        }

        sscanf(buffer, "%s", command);

        if (!strcmp(command, "ls")) {

            char buffer[MAXSIZE];
            char output[MAXSIZE];

            bzero(buffer, MAXSIZE);
            bzero(output, MAXSIZE);
            int b_ptr = 0, output_size = 0;
            FILE *fp = popen("ls -pa", "r");
            while (fgets(buffer, MAXSIZE, fp)) {
                strcat(output, buffer);
                while (buffer[b_ptr] != '\0') {
                    b_ptr++;
                }
                output_size += b_ptr;
                b_ptr = 0;
            }

            n = write(newsockfd, output, output_size);

        } else if (command[0] == 'c' && command[1] == 'd') {
            char newdir[MAXSIZE];
            char cwdir[MAXSIZE];

            int err;
            getcwd(cwdir, MAXSIZE);
            sscanf(buffer + 3, "%s", newdir);


            if (newdir[0] == '.' && newdir[1] == '.') {
                char *todel = strrchr(cwdir, 47);
                (*todel) = '\0';
                err = chdir(cwdir);
            } else {
                err = chdir(newdir);
            }

//            printf("%s", getcwd())

            if (err == 0) {
                n = write(newsockfd, "Directory changed...\n", 22);
//                getcwd(cwdir, MAXSIZE);
//                printf("\nCurrent: %s\n", cwdir);
//                fflush(stdout);
            } else {
                n = write(newsockfd, "Directory doesn't exist...\n", 28);
            }
            //implement directory change

        } else if (command[0] == 'p' && command[1] == 'u' && command[2] == 't') {

            //bierze nazwe pliku z komendy get nazwa
            char filename[MAXSIZE];
            sscanf(buffer + 4, "%s", filename);
            //pobiera rozmiar pliku
            n = read(newsockfd, buffer, 256);
            //czekaj na rozmiar - jesli "-" to przerwij

            if (n < 0) {
                perror("ERROR reading size from socket");
                exit(1);
            } else if (buffer[0] == '-') {
                printf("No such a file on server\n");
                fflush(stdout);
                //jesli jednak jest to stworz plik na serwerze

            } else {
                int size = 0;
                //parsuje rozmiar na inta
                sscanf(buffer, "%s", command);
                size = atoi(command);
                printf("\n FILE SIZE: %d \n", size);

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


                //zczytaj party od klienta
                int i = 0;

                int last = size % 256;
                while (i < size) {
                    if (i + 256 > size) {
                        n = read(newsockfd, buffer, last);
                        lseek(filehandle, i, SEEK_SET);
                        n = write(filehandle, buffer, last);
                    } else {
                        n = read(newsockfd, buffer, 256);
                        lseek(filehandle, i, SEEK_SET);
                        n = write(filehandle, buffer, 256);
                    }

                    i += 256;
                }
            }
//zakoncz puta
        } else if (command[0] == 'g' && command[1] == 'e' && command[2] == 't') {



            //bierze nazwe pliku z komendy get nazwa
            char filename[MAXSIZE];
            sscanf(buffer + 4, "%s", filename);

            //szuka pliku na serwerze
            int err = stat(filename, &file);
            //blad jesli nie znajduje
            if (err == -1) {
                perror("ERROR while reading file to get");
                size = 0;
                bzero(buffer, 256);
                buffer[0] = '-';
                write(newsockfd, buffer, 256);
            } else {
                //otwiera plik
                filehandle = open(filename, O_RDONLY);
                size = file.st_size;

                //jesli nie ma pliku to przesyla -
                if (filehandle == -1) {
                    perror("ERROR opening file on server... ?");
                    size = 0;
                    bzero(buffer, 256);
                    buffer[0] = '-';
                    write(newsockfd, buffer, 256);
                    //jesli jest to wysyla rozmiar
                } else {
                    sprintf(buffer, "%d", size);
                    fflush(stdout);
                    write(newsockfd, buffer, 256);


                    //wysyla max 256 bitowe paczki z plikiem

                    int i = 0;

                    int last = size % 256;

                    while (i < size) {
                        if (i + 256 > size) {
                            lseek(filehandle, i, SEEK_SET);
                            n = read(filehandle, buffer, last);
                            n = write(newsockfd, buffer, last);
                        } else {
                            lseek(filehandle, i, SEEK_SET);
                            n = read(filehandle, buffer, 256);
                            n = write(newsockfd, buffer, 256);
                        }

                        i += 256;
                    }
                    //jesli plik jest wiekszy niz zero to przesyla go do klienta


                }
            }


        } else if (strcmp("end", command) == 0) {
            break;
        } else {
            n = write(newsockfd, "Unknown command\n", 16);
        }

        if (n < 0) {
            perror("ERROR writing to socket");
            printf("Client disconnected... listening for another client...");
        }
    }

}
