FLAGS = -Wall -pthread

all: origin_simulate ex_01_simulate

origin_simulate: origin_simulate.c origin_account.c origin_account.h
	gcc -o origin_simulate origin_simulate.c origin_account.c $(FLAGS)

ex_01_simulate: ex_01_simulate.c ex_01_account.c ex_01_account.h
	gcc -o ex_01_simulate ex_01_simulate.c ex_01_account.c $(FLAGS)
