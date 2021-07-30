PROJECT = blackwood
HEADER 	= $(PROJECT).h
OBJ	= $(PROJECT).o get_teams.o globals.o timecode.o graph.o get_nuts.o

C99 	= cc -std=c99
CFLAGS 	= -Wall -pedantic -Werror

$(PROJECT) : $(OBJ) 
	$(C99) $(CFLAGS) -o $(PROJECT) \
		$(OBJ)

%.o : %.c $(HEADER)
	$(C99) $(CFLAGS) -c $< 

clean:
	rm -f $(PROJECT) $(OBJ)

