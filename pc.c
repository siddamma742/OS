#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define SIZE 5

int buffer[SIZE];
int in = 0, out = 0;
int item = 0;

sem_t mutex, empty, full;

void produce() {
    if (sem_trywait(&empty) != 0) {
        printf("Buffer is full!\n");
        return;
    }

    sem_wait(&mutex);

    item++;
    buffer[in] = item;
    printf("Producer has produced: Item %d\n", item);
    in = (in + 1) % SIZE;

    sem_post(&mutex);
    sem_post(&full);
}

void consume() {
    if (sem_trywait(&full) != 0) {
        printf("Buffer is empty!\n");
        return;
    }

    sem_wait(&mutex);

    int consumed = buffer[out];
    printf("Consumermhas consumed: Item %d\n", consumed);
    out = (out + 1) % SIZE;

    sem_post(&mutex);
    sem_post(&empty);
}

int main() {
    int choice;

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);

    while (1) {
        printf("Enter \n");
        printf("1. Produce 2.Consume. 3.Exit \n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                produce();
                break;

            case 2:
                consume();
                break;

            case 3:
                sem_destroy(&mutex);
                sem_destroy(&empty);
                sem_destroy(&full);
                exit(0);

            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}
