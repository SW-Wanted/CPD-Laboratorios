CC     = gcc
CFLAGS = -g -I ./includes

cpd-terminal: main.o list.o commandlinereader.o
	$(CC) -o cpd-terminal main.o list.o commandlinereader.o -lpthread 

main.o: main.c ./includes/list.h ./includes/commandlinereader.h
	$(CC) $(CFLAGS) -c main.c

list.o: ./srcs/list.c ./includes/list.h
	$(CC) $(CFLAGS) -c ./srcs/list.c

commandlinereader.o: ./srcs/commandlinereader.c ./includes/commandlinereader.h
	$(CC) $(CFLAGS) -c ./srcs/commandlinereader.c

fibonacci: ./srcs/fibonacci.c
	$(CC) $(CFLAGS) -o fibonacci ./srcs/fibonacci.c

test: cpd-terminal fibonacci
	@echo "=== Teste 1: paralelo (3 fibonacci) ==="
	./cpd-terminal < tests/input.txt
	@echo ""
	@echo "=== Teste 2: sequencial (1 fibonacci) ==="
	./cpd-terminal < tests/input1.txt
	@echo ""
	@echo "=== Teste 3: comando invalido ==="
	./cpd-terminal < tests/input2.txt
	@echo ""
	@echo "=== Teste 4: exit imediato sem filhos ==="
	./cpd-terminal < tests/input3.txt

clean:
	rm -f *.o cpd-terminal fibonacci