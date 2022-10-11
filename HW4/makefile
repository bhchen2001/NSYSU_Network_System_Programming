CC = gcc
12_1_FILE = 12_1
12_2_FILE = 12_2
12_3_FILE = 12_3


all:run
run:
	$(CC) -c $(12_1_FILE).c $(12_2_FILE).c $(12_3_FILE).c
	$(CC) -o $(12_1_FILE) $(12_1_FILE).o
	$(CC) -o $(12_2_FILE) $(12_2_FILE).o
	$(CC) -o $(12_3_FILE) $(12_3_FILE).o
	clear
	#
	# instruction of hw4
	# 1.
	# 	./12_1 <username>
	# 2.
	#	./12_2
	# 3.
	#	./12_3 <filename> (absolute pathname for <filename>)

clean:
	rm -f $(12_1_FILE) $(12_1_FILE).o
	rm -f $(12_2_FILE) $(12_2_FILE).o
	rm -f $(12_3_FILE) $(12_3_FILE).o
