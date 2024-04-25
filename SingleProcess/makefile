CC := gcc

ALL: singleProcess 

singleProcess : lib/ioOperation.o lib/stringOperation.o src/graph.o lib/wordArray.o  src/compito1.o src/compito2.o src/main.o 
	$(CC) $^ -o $@  

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@  

clean:
	rm -f src/*.o
	rm -f lib/*.o