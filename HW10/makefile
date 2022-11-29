CC = gcc
PROGRAM_FILE = program

all:run

run:
	$(CC) -c $(PROGRAM_FILE).c
	$(CC) -o $(PROGRAM_FILE) $(PROGRAM_FILE).o
	clear

	# instruction for HW10
	# 	program <M> <R> <N> <B>

clean:
	rm -f $(PROGRAM_FILE) $(PROGRAM_FILE).o
