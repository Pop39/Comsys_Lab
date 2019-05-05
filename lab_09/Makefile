
FLAGS = -Wall -pthread
INCLUDES = ./include

all: cv1 client

clean:
	rm -f cv1 *~

cv1: cv1.c
	gcc -I $(INCLUDES) -o cv1 cv1.c $(FLAGS)

client: client.c sbuf.c
	gcc -I $(INCLUDES) -o client client.c sbuf.c $(FLAGS)
