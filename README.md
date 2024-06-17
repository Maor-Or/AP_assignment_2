# AP_assignment_2

## How to Run
1) clone this repo into your local machine
2) run "make" in terminal
3) to see with time - run "time ./randomGenerator <seed> <num_of_numbers> | ./RMprimeCounter"
4) to see ram usage - run "./randomGenerator 10 10000000 | valgrind --tool=massif ./RMprimeCounter"
