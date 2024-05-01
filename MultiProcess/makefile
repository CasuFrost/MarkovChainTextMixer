CC := gcc

ALL: multiProcess clean

multiProcess :  src/fileReader2.o src/graph.o src/compito2.o  src/main.o 
	$(CC) $^ -o $@  

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f src/*.o
	rm -f lib/*.o