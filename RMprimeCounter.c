

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

#define MAX_QUEUE_SIZE 6000 // Maximum size of the queue
#define BATCH_SIZE 12000 // Batch size for processing numbers

typedef struct {
    int data[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool done;
} Queue;

typedef struct {
    Queue* queue;
    int* total_counter;
    pthread_mutex_t* counter_mutex;
} ThreadArgs;

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = q->count = 0;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->cond, NULL);
    q->done = false;
    return q;
}

void enqueue(Queue* q, int data) {
    pthread_mutex_lock(&q->mutex);
    while (q->count == MAX_QUEUE_SIZE) {
        pthread_cond_wait(&q->cond, &q->mutex);
    }
    q->data[q->rear] = data;
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->count++;
    pthread_cond_signal(&q->cond);
    pthread_mutex_unlock(&q->mutex);
}

int dequeue(Queue* q, int batch[], int* batch_count) {
    pthread_mutex_lock(&q->mutex);
    while (q->count == 0 && !q->done) {
        pthread_cond_wait(&q->cond, &q->mutex);
    }
    if (q->count == 0) {
        pthread_mutex_unlock(&q->mutex);
        return -1; // Queue is empty and done
    }
    *batch_count = 0;
    while (*batch_count < BATCH_SIZE && q->count > 0) {
        batch[*batch_count] = q->data[q->front];
        q->front = (q->front + 1) % MAX_QUEUE_SIZE;
        q->count--;
        (*batch_count)++;
    }
    pthread_cond_signal(&q->cond);
    pthread_mutex_unlock(&q->mutex);
    return 0;
}


//better function:
bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    if (n <= 3) {
        return true;
    }
    if (n % 2 == 0 || n % 3 == 0) {
        return false;
    }
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}
// original Function to check if a number is prime
// bool isPrime(int n) {
//     if (n <= 1) {
//         return false;
//     }
//     for (int i = 2; i * i <= n; i++) {
//         if (n % i == 0) {
//             return false;
//         }
//     }
//     return true;
// }

void* worker(void* args) {
    ThreadArgs* threadArgs = (ThreadArgs*)args;
    Queue* queue = threadArgs->queue;
    int total_counter = 0;

    while (true) {
        int batch[BATCH_SIZE];
        int count;
        if (dequeue(queue, batch, &count) == -1) {
            break;
        }
        for (int i = 0; i < count; i++) {
            if (isPrime(batch[i])) {
                total_counter++;
            }
        }
    }

    pthread_mutex_lock(threadArgs->counter_mutex);
    *(threadArgs->total_counter) += total_counter;
    pthread_mutex_unlock(threadArgs->counter_mutex);

    return NULL;
}

int main() {
    int num;
    int total_counter = 0;
    int num_threads = omp_get_num_procs();
    pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    ThreadArgs *threadArgs = (ThreadArgs *)malloc(num_threads * sizeof(ThreadArgs));
    Queue* queue = createQueue();
    pthread_mutex_t counter_mutex;
    pthread_mutex_init(&counter_mutex, NULL);

    for (int i = 0; i < num_threads; i++) {
        threadArgs[i].queue = queue;
        threadArgs[i].total_counter = &total_counter;
        threadArgs[i].counter_mutex = &counter_mutex;
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, worker, (void *)&threadArgs[i]);
    }

    while (scanf("%d", &num) != EOF) {
        enqueue(queue, num);
    }
    queue->done = true;
    pthread_cond_broadcast(&queue->cond);

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d total primes.\n", total_counter);

    pthread_mutex_destroy(&counter_mutex);
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);
    free(queue);
    free(threads);
    free(threadArgs);

    return 0;
}

