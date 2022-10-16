CC = gcc
CONVERT = convert
LOOKUP1 = lookup1
MAIN = main
FILE_LOOKUP = file_lookup

all:run

run:
	$(CC) -c $(CONVERT).c
	$(CC) -o $(CONVERT) $(CONVERT).o

	$(CC) -c $(MAIN).c $(LOOKUP1).c
	$(CC) -o $(FILE_LOOKUP) $(MAIN).o $(LOOKUP1).o

	clear

	# instruction of HW5_part3
	# ./convert <src_dict> <des_dict>
	# ./file_lookup <src_dict>

clean:
	rm -f $(CONVERT) $(CONVERT).o
	rm -f $(FILE_LOOKUP) $(MAIN).o $(LOOKUP1).o
