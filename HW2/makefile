CC = gcc
TAIL_FILE = hw2_tail

all:run
run:
	$(CC) -c $(TAIL_FILE).c
	$(CC) -o $(TAIL_FILE) $(TAIL_FILE).o
	#
	# instruction of hw2
	# 1. tail
	# 		./hw2_tail [-n num] <file>

clean:
	rm -f $(TAIL_FILE) $(TAIL_FILE).o
