main: task.o main.o
	gcc -o main task.o main.o

task.o: task.c task.h
	gcc -g -c task.c

main.o: main.c task.h
	gcc -g -c main.c

clean:
	rm -f *.o main