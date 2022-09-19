CC = gcc
TEE_FILE = hw1_tee
CP_FILE = hw1_cp
HOLE_CODE_FILE = hole_file_generator
HOLE_FILE = hw1_hole_file

all:run

run:
	$(CC) -c $(TEE_FILE).c $(CP_FILE).c $(HOLE_CODE_FILE).c
	$(CC) -o $(TEE_FILE) $(TEE_FILE).o
	$(CC) -o $(CP_FILE) $(CP_FILE).o
	$(CC) -o $(HOLE_CODE_FILE) $(HOLE_CODE_FILE).o
	#
	# instruction for commands in hw1:
	# 1. tee
	# 	echo text | ./hw1_tee [-a] <filename>
	# 2. cp
	# 	./hole_file_generator
	# 	cp <file> <newfile>

clean:
	rm -f $(TEE_FILE) $(TEE_FILE).o
	rm -f $(CP_FILE) $(CP_FILE).o
	rm -f $(HOLE_CODE_FILE) $(HOLE_CODE_FILE).o $(HOLE_FILE)
