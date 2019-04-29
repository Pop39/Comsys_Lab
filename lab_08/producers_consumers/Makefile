FLAGS = -Wall -pthread

all: client

clean:
	rm -f client *~

client: client.c smoker.c smoker.h
	gcc -o client client.c smoker.c $(FLAGS)

