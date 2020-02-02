main: $(foo)
	gcc -Wall -o3 $(foo).c -o $(foo)

clean:
	rm $(foo)
