#include <stdint.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>

sem_t sb1;
sem_t se1;
sem_t sb2;
sem_t se2;

int X = 0;
int Y = 0;
int r1 = 0;
int r2 = 0;

int xx = 0;
int yy = 0;

void *th1(void *args)
{
    for (;;)
    {
        sem_wait(&sb1);

        while (rand() % 8);

        X = 1;
        __asm__ volatile ("" : : : "memory");
        __asm__ volatile ("mfence" ::: "memory");
        r1 = Y;

        sem_post(&se1);

        ++xx;
    }

    return NULL;
}

void *th2(void *args)
{
    for (;;)
    {
        sem_wait(&sb2);

        while (rand() % 8);

        Y = 1;
        __asm__ volatile ("" : : : "memory");
        __asm__ volatile ("mfence" ::: "memory");
        r2 = X;

        sem_post(&se2);

        ++yy;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    sem_init(&sb1, 0, 0);
    sem_init(&se1, 0, 0);
    sem_init(&sb2, 0, 0);
    sem_init(&se2, 0, 0);

    pthread_t tid[2];

    pthread_create(&tid[0], NULL, th1, NULL);
    pthread_create(&tid[1], NULL, th2, NULL);

    int detected = 0;
    for (;;)
    {
        X = 0;
        Y = 0;
        sem_post(&sb1);
        sem_post(&sb2);
        sem_wait(&se1);
        sem_wait(&se2);

        if (r1 == 0 && r2 == 0)
        {
            ++detected;
            printf("detected: %d, xx=%d, yy=%d\n", detected, xx, yy);
            if (detected >= 100)
            {
                break;
            }
        }
    }

    return 0;
}
