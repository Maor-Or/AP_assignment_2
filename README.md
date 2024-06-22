# AP_assignment_2

## How to Run
* clone this repo into your local machine
* run "make" in terminal
### Running our code:
* to see with time, run: "time ./randomGenerator {seed} {num_of_numbers} | ./RMprimeCounter"
* to see ram usage, run: "./randomGenerator {seed} {num_of_numbers} | valgrind --tool=massif ./RMprimeCounter"
### Example Usage:
* time ./randomGenerator 10 10000000 | ./RMprimeCounter
* ./randomGenerator 10 10000000 | valgrind --tool=massif ./RMprimeCounter

## Results from our machine:

### Base-line (orignal teacher code):
![base_line_code_by_teacher](https://github.com/Maor-Or/AP_assignment_2/assets/43138073/7ce4133a-50ea-48fb-ae84-42410afd9528)

### Our multithread sulation (original prime function):
![multithread_original_prime_function](https://github.com/Maor-Or/AP_assignment_2/assets/43138073/29a35256-cf1a-467f-8c68-3f9e6da09321)

### Our multithread sulation (better prime function):
![multithread_better_prime_function](https://github.com/Maor-Or/AP_assignment_2/assets/43138073/c1ede42b-e990-4607-ad22-a60efbb72db2)

### Proof of RAM useage (below 2 MB):
![proof_of_RAM_usage](https://github.com/Maor-Or/AP_assignment_2/assets/43138073/5b35f773-3552-48e2-bb1c-43e10fc18051)
