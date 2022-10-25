CC = gcc
44_1_FILE = 44_1
44_2_FILE = 44_2

all:run

run:
	CC -c $(44_1_FILE).c $(44_2_FILE).c
	CC -o $(44_1_FILE) $(44_1_FILE).o
	CC -o $(44_2_FILE) $(44_2_FILE).o

	clear
	# instruction of HW6
	# 1. 
	#	./44_1
	# 2.
	# 	./44_2 <command> <type>

clean:
	rm -f $(44_1_FILE) $(44_1_FILE).o
	rm -f $(44_2_FILE) $(44_2_FILE).o
