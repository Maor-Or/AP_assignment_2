.PHONY: all
all: generator primeCounter RMprimeCounter

RMprimeCounter: RMprimeCounter.c
	gcc -g -o RMprimeCounter RMprimeCounter.c -pthread

generator:  generator.c
	gcc -o randomGenerator generator.c

primeCounter:	primeCounter.c
	gcc -g -o primeCounter primeCounter.c

.PHONY: clean
clean:
	-rm randomGenerator primeCounter 2>/dev/null RMprimeCounter
