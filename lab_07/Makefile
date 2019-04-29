
FLAGS = -Wall -pthread
INCLUDES = ./include

all: t0 t1 sharing badcnt goodcnt client race norace

clean:
	rm -f t0 t1 sharing badcnt goodcnt client race norace *~

t0: t0.c
	gcc -I $(INCLUDES) -o t0 t0.c $(FLAGS)

t1: t1.c
	gcc -I $(INCLUDES) -o t1 t1.c $(FLAGS)

sharing: sharing.c
	gcc -I $(INCLUDES) -o sharing sharing.c $(FLAGS)

badcnt: badcnt.c
	gcc -I $(INCLUDES) -o badcnt badcnt.c $(FLAGS)

goodcnt: goodcnt.c
	gcc -I $(INCLUDES) -o goodcnt goodcnt.c $(FLAGS)

race: race.c
	gcc -I $(INCLUDES) -o race race.c $(FLAGS)

norace: norace.c
	gcc -I $(INCLUDES) -o norace norace.c $(FLAGS)

client: client.c sbuf.c
	gcc -I $(INCLUDES) -o client client.c sbuf.c $(FLAGS)

