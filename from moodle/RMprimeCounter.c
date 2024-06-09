#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // For sleep function (for demonstration)

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool done;
} Queue;


Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->cond, NULL);
    return q;
}

void enqueue(Queue* q, int data) {
    //printf("in enque\n");
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    pthread_mutex_lock(&q->mutex);
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    //printf("enqued, number: %d\n",newNode->data);
    pthread_cond_signal(&q->cond);
    pthread_mutex_unlock(&q->mutex);
}

int dequeue(Queue* q) {
    //printf("in dequeue\n");

    pthread_mutex_lock(&q->mutex);
    while (q->front == NULL && !q->done) {
        pthread_cond_wait(&q->cond, &q->mutex);
    }
    if (q->front == NULL) {
        pthread_mutex_unlock(&q->mutex);
            //printf("queue is empty - returning -1\n");

        return -1; // Queue is empty and done
    }
    Node* temp = q->front;
    int data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    pthread_mutex_unlock(&q->mutex);
        //printf("dequed, number: %d\n",data);

    return data;
}



bool isQueueEmpty(Queue* q) {
    pthread_mutex_lock(&q->mutex);
    bool isEmpty = (q->front == NULL);
    pthread_mutex_unlock(&q->mutex);
    return isEmpty;
}



// Function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

typedef struct {
    Queue* queue;
    int* total_counter;
    pthread_mutex_t* counter_mutex;
} ThreadArgs;

void* worker(void* args) {
    ThreadArgs* threadArgs = (ThreadArgs*)args;
    Queue* queue = threadArgs->queue;
    int* total_counter = threadArgs->total_counter;
    pthread_mutex_t* counter_mutex = threadArgs->counter_mutex;

    while (true) {
        int num = dequeue(queue);
        if (num == -1) {
            // Queue is empty and done, exit
            break;
        }
        // Check if the dequeued number is prime
        if (isPrime(num)) {
            // Increment the total counter under the protection of the counter mutex
            pthread_mutex_lock(counter_mutex);
            (*total_counter)++;
            pthread_mutex_unlock(counter_mutex);
        }
    }
    return NULL;
}



int main() {
    //printf("start\n");
    int num;
    int total_counter = 0;
    int num_threads = 10; // Adjust the number of threads as needed
    pthread_t threads[num_threads];
    ThreadArgs threadArgs;
    Queue* queue = createQueue();
    pthread_mutex_t counter_mutex;
    pthread_mutex_init(&counter_mutex, NULL);
    clock_t start, end;
    double cpu_time_used;

    threadArgs.queue = queue;
    threadArgs.total_counter = &total_counter;
    threadArgs.counter_mutex = &counter_mutex;

    // Start measuring time
    start = clock();

    printf("creating workers\n");

    // Create worker threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, worker, &threadArgs);
    }

    printf("done creating workers, started scanf\n");

    // Read numbers from stdin and add them to the queue
    while (scanf("%d", &num) != EOF) {
        enqueue(queue, num);
    }
    queue->done = true;

    // Join worker threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // End measuring time
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("%d total primes.\n", total_counter);
    printf("Time taken to execute: %f seconds\n", cpu_time_used);

    // Cleanup
    pthread_mutex_destroy(&counter_mutex);
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);
    free(queue);

    return 0;
}
