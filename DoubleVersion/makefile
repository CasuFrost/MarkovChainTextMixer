CC := gcc

ALL: MCtextMixer clean

MCtextMixer :  src/fileReader2.o src/hashTable.o src/stringOperation.o src/graph.o src/ioOperation.o src/wordArray.o src/compito2.o  src/compito1.o src/compito2_multi.o  src/compito1_multi.o   src/main.o 
	$(CC) $^ -o $@  

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f src/*.o
	rm -f lib/*.o