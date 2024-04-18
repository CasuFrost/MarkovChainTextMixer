CC := gcc

ALL: singleProcess clean

singleProcess : src/stringOperation.o src/graph.o src/main.o src/ioOperation.o src/compito1.o src/compito2.o src/stringOperation.o
	$(CC) $^ -o $@ -w

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ -w

clean:
	rm -f src/*.o