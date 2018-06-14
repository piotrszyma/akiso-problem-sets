#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


#define RND_NUM 10

struct arguments {
    int **fmatrix;
    int **smatrix;
    int **result;
    int start;
    int step;
    int size;
};

void showMatrix(int **matrix, int size);

int **createRandomMatrix(int size);

int **multiplyMatrix(int size, int **fm, int **sm);

void *thread_function(void *args) {
    struct arguments *a = args;

    int m = 0;

    for (int i = 0; i < a->size; i++) {
        for (int j = 0; j < a->size; j++) {
            if(m == a->start)
            {
                a->result[i][j] = 0;
                for (int z = 0; z < a->size; z++) {
                    a->result[i][j] += a->fmatrix[i][z] * a->smatrix[z][j];
                }
                a->start+=a->step;
            }
            m++;
        }
    }

    free(args);
    return NULL;
}


int main(void) {
    srand(time(NULL));
    int size;
    int threads_num;



    int **matrix;
    int **smatrix;
    int **result;


    printf("What's the size of the matrix?\n");
    scanf("%d", &size);

    printf("Type in number of threads:\n");
    scanf("%d", &threads_num);

    if(threads_num > size * size)
    {
        threads_num = size * size;
    }


    matrix = createRandomMatrix(size);
    smatrix = createRandomMatrix(size);
    result = createRandomMatrix(size);

    pthread_t threads[threads_num];

    for (int i = 0; i < threads_num; i++) {

        struct arguments *args = malloc(sizeof(struct arguments));

        args->fmatrix = matrix;
        args->smatrix = smatrix;
        args->result = result;
        args->start = i;
        args->step = threads_num;
        args->size = size;

        pthread_create(&threads[i], NULL, &thread_function, (void *) args);


    }

    for (int i = 0; i < threads_num; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nFactor 1:\n\n");

    showMatrix(matrix, size);


    printf("\nFactor 2:\n\n");

    showMatrix(smatrix, size);

    printf("\n");

    printf("Product:\n\n");

    showMatrix(result, size);

    printf("\n");

    return 0;
}

int **multiplyMatrix(int size, int **fm, int **sm) {
    int **matrix = (int **) malloc(size * sizeof(int *));

    for (int i = 0; i < size; i++) {
        int *row = malloc(size * sizeof(int));
        matrix[i] = row;
        for (int j = 0; j < size; j++) {
            matrix[i][j] = 0;
            for (int k = 0; k < size; k++) {
                matrix[i][j] += fm[i][k] * sm[k][j];
            }
        }
    }

    return matrix;
}

int **createRandomMatrix(int size) {

    int **matrix = (int **) malloc(size * sizeof(int *));

    for (int i = 0; i < size; i++) {
        int *row = malloc(size * sizeof(int));
        matrix[i] = row;
        for (int j = 0; j < size; j++) {
            matrix[i][j] = (rand() % RND_NUM);
        }

    }

    return matrix;
}

void showMatrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%10d", matrix[i][j]);
        }
        printf("\n");
    }
}

