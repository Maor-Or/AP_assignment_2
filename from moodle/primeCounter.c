#include <stdio.h>
#include <stdbool.h>
#include <time.h>

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

int main() {
    int num;
    int total_counter = 0;
    clock_t start, end;
    double cpu_time_used;

    // Start measuring time
    start = clock();

    // Read numbers from stdin until end of file
    while (scanf("%d", &num) != EOF) {   
        if (isPrime(num)) {
            total_counter++;
        }
    }

    // End measuring time
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("%d total primes.\n", total_counter);
    printf("Time taken to execute: %f seconds\n", cpu_time_used);

    return 0;
}
