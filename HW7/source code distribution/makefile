CC = gcc
SIG_SPEED_FILE = sig_speed_sigsuspend

all:run

run:
	CC -c $(SIG_SPEED_FILE).c
	CC -o $(SIG_SPEED_FILE) $(SIG_SPEED_FILE).o

	clear
	# instruction of HW7
	# 1. 
	# 	from code distribution
	# 	./sig_speed_sigsuspend <sig_num>

clean:
	rm -f $(SIG_SPEED_FILE) $(SIG_SPEED_FILE).o
