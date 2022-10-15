CC = gcc
SHELL_FILE = shell
PARSE_FILE = parse
BUILTIN_FILE = builtin
RUN_COMMAND_FILE = run_command
IS_BACKGROUND = is_background
PIPE_COMMAND = pipe_command
PIPE_PRESENT = pipe_present
REDIRECT_IN = redirect_in
REDIRECT_OUT = redirect_out


all:run
run:
	$(CC) -c $(SHELL_FILE).c $(PARSE_FILE).c $(BUILTIN_FILE).c $(RUN_COMMAND_FILE).c $(IS_BACKGROUND).c $(PIPE_COMMAND).c $(PIPE_PRESENT).c $(REDIRECT_IN).c $(REDIRECT_OUT).c
	$(CC) -o $(SHELL_FILE) $(SHELL_FILE).o $(PARSE_FILE).o $(BUILTIN_FILE).o $(RUN_COMMAND_FILE).o $(IS_BACKGROUND).o $(PIPE_COMMAND).o $(PIPE_PRESENT).o $(REDIRECT_IN).o $(REDIRECT_OUT).o
	clear
	#
	# instruction of hw5_part2
	#  ./shell

clean:
	rm -f $(SHELL_FILE) $(SHELL_FILE).o $(PARSE_FILE).o $(BUILTIN_FILE).o $(RUN_COMMAND_FILE).o $(IS_BACKGROUND).o $(PIPE_COMMAND).o $(PIPE_PRESENT).o $(REDIRECT_IN).o $(REDIRECT_OUT).o