
FLAGS = -Wall -pthread
INCLUDES = ./include

all: simulate

clean:
	rm -f simulate *~

simulate: simulate.c account.c account.h
	gcc -I $(INCLUDES) -o simulate simulate.c account.c $(FLAGS)
