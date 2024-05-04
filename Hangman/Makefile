# This is a template makefile, generated to bind all the objects for HW#2. 
# Use this template to replace the placeholder text with the name of your specific program.
# Please note that you can either choose to use the example given below as it is, or make 
# a custom makefile by modifying the code.


CC = gcc
ARGS = -Wall


# Compiling all the dependencies
all: hangman_client hangman_server

# Replace <"your_program"> with the name of your specififc program. 

hangman_client: hangman_client.c
	$(CC) $(ARGS) -o hangman_client hangman_client.c

hangman_server: hangman_server.c
	$(CC) $(ARGS) -o hangman_server hangman_server.c


clean:
	rm -f *.o hangman_client *~
	rm -f *.o hangman_server *~
