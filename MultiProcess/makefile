CC := gcc

ALL: multiProcess clean

multiProcess :  src/compito2.o  src/main.o 
	$(CC) $^ -o $@  

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@  -w

clean:
	rm -f src/*.o
	rm -f lib/*.o