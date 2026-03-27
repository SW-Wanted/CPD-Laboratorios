# Exercício 01: Implementação de uma função

Implementar a função `update_terminated_process` que deve percorrer uma lista ligada de processos e atualizar o campo `endtime` de um processo específico, identificado pelo seu `PID`.

---

## 🛠️ Guia de Uso

### 1. Compilação e Execução
Para compilar com símbolos de depuração (necessário para o GDB):
```bash
gcc -g -c list.c main.c
gcc -o main list.o main.o
./main
```
---

## 🛠️ Guia de Uso

### 1. Compilação e Execução
Para compilar com símbolos de depuração (necessário para o GDB):
```bash
make
./main
```

### 2. Depuração com GDB
Para analisar o comportamento da lista:

```bash
gdb ./main
(gdb) b list.c:44        # Define breakpoint na inserção
(gdb) r                  # Inicia a execução
(gdb) p *item            # Inspeciona o conteúdo real da estrutura
```
