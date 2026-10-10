build: burrow.c
	gcc burrow.c -o burrow

run: burrow
	./burrow

clean:
	rm burrow